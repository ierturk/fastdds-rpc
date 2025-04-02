#include "RBIC1Proxy.h"
#include "RBIC1.h"

RBIC1Proxy::RBIC1Proxy() : rpc_() {}

RBIC1Proxy::~RBIC1Proxy() {}

void RBIC1Proxy::init(const std::string& reader_topic, const std::string& writer_topic) {
    rpc_.init(reader_topic, writer_topic);
}

void RBIC1Proxy::run() {
    std::cout << "RBIC1Proxy: Starting..." << std::endl;

    // Check subscription matching
    eprosima::fastdds::dds::SubscriptionMatchedStatus matched_status;
    while (rpc_.getReader()->get_subscription_matched_status(matched_status) == eprosima::fastrtps::types::ReturnCode_t::RETCODE_OK &&
           matched_status.current_count == 0) {
        std::cout << "RBIC1Proxy: Waiting for subscription matching..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "RBIC1Proxy: Subscription matched!" << std::endl;

    // Create a WaitSet and attach a ReadCondition
    eprosima::fastdds::dds::WaitSet wait_set;
    eprosima::fastdds::dds::ReadCondition* read_condition = rpc_.getReader()->create_readcondition(
        eprosima::fastdds::dds::ANY_SAMPLE_STATE,
        eprosima::fastdds::dds::ANY_VIEW_STATE,
        eprosima::fastdds::dds::ANY_INSTANCE_STATE);
    wait_set.attach_condition(*read_condition);

    while (true) {
        // Wait for data to be available
        eprosima::fastdds::dds::ConditionSeq active_conditions;
        wait_set.wait(active_conditions, eprosima::fastrtps::c_TimeInfinite);

        // Take the sample
        RBIC1Rpc::GenericRequest request;
        eprosima::fastdds::dds::SampleInfo info;
        if (rpc_.getReader()->take_next_sample(&request, &info) == eprosima::fastrtps::types::ReturnCode_t::RETCODE_OK && info.valid_data) {
            std::cout << "RBIC1Proxy: Received request with ID " << request.requestId() << std::endl;

            // get dllversion from dll
            char version[256] = "1.0.0";
            if ( RBIC1::DLLVersion(version))
            {
                std::cout << "RBIC1Proxy: DLL version: " << version << std::endl;
            }
            else
            {
                std::cerr << "RBIC1Proxy: Failed to get DLL version" << std::endl;
            }

            // Create a reply
            RBIC1Rpc::GenericReply reply;
            reply.requestId(request.requestId());
            reply.reply().dllVersionReply(RBIC1Rpc::DLLVersionReply());
            reply.reply().dllVersionReply().success(true);
            reply.reply().dllVersionReply().version(version);


            // Send the reply
            std::cout << "RBIC1Proxy: Sending reply with version: " << version << std::endl;
            rpc_.getWriter()->write(&reply);

            // wait for sending to finish
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            break;
        }
    }

    // Clean up
    rpc_.getReader()->delete_readcondition(read_condition);
}