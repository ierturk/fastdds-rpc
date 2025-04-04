#include "RBIC1Invoker.h"

RBIC1Invoker::RBIC1Invoker() : rpc_() {}

RBIC1Invoker::~RBIC1Invoker() {}

void RBIC1Invoker::init(const std::string& reader_topic, const std::string& writer_topic) {
    rpc_.init(reader_topic, writer_topic);
}

void RBIC1Invoker::run() {
    std::cout << "RBIC1Invoker: Starting..." << std::endl;

    // Check publication matching
    eprosima::fastdds::dds::PublicationMatchedStatus matched_status;
    while (rpc_.getWriter()->get_publication_matched_status(matched_status) == eprosima::fastrtps::types::ReturnCode_t::RETCODE_OK &&
           matched_status.current_count == 0) {
        std::cout << "RBIC1Invoker: Waiting for publication matching..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "RBIC1Invoker: Publication matched!" << std::endl;

    // Send a request
    RBIC1Rpc::GenericRequest request;
    request.requestId(1);
    request.request().dllVersionRequest(RBIC1Rpc::DLLVersionRequest());
    rpc_.getWriter()->write(&request);
    std::cout << "RBIC1Invoker: Sent request with ID 1" << std::endl;

    // Wait for a reply
    eprosima::fastdds::dds::WaitSet wait_set;
    eprosima::fastdds::dds::ReadCondition* read_condition = rpc_.getReader()->create_readcondition(
        eprosima::fastdds::dds::ANY_SAMPLE_STATE,
        eprosima::fastdds::dds::ANY_VIEW_STATE,
        eprosima::fastdds::dds::ANY_INSTANCE_STATE);
    wait_set.attach_condition(*read_condition);

    eprosima::fastdds::dds::ConditionSeq active_conditions;
    wait_set.wait(active_conditions, eprosima::fastrtps::c_TimeInfinite);

    // Take the reply
    RBIC1Rpc::GenericReply reply;
    eprosima::fastdds::dds::SampleInfo info;
    if (rpc_.getReader()->take_next_sample(&reply, &info) == eprosima::fastrtps::types::ReturnCode_t::RETCODE_OK && info.valid_data) {
        if (reply.reply()._d() == RBIC1Rpc::DLLVERSION_REPLY) {
            std::string version = reply.reply().dllVersionReply().version();
            std::cout << "RBIC1Invoker: Received reply with version: " << version << std::endl;

            // Emit the signal
            replyReceived(version);
        }
    }

    // Clean up
    rpc_.getReader()->delete_readcondition(read_condition);
}