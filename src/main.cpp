#include <iostream>
#include <thread>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/core/condition/WaitSet.hpp>
#include <fastdds/dds/subscriber/ReadCondition.hpp>
#include <fastdds/dds/log/Log.hpp>  // Include Fast DDS logging
#include "RBIC1.h"
#include "RBIC1PubSubTypes.h"

# include "../rbic1/RBIC1.h"

using namespace eprosima::fastdds::dds;

// Proxy function using WaitSet
void run_proxy() {
    std::cout << "Proxy: Starting..." << std::endl;

    // Create DomainParticipant
    DomainParticipant* participant = DomainParticipantFactory::get_instance()->create_participant(0, PARTICIPANT_QOS_DEFAULT);

    // Register types
    TypeSupport request_type(new RBIC1::GenericRequestPubSubType());
    TypeSupport reply_type(new RBIC1::GenericReplyPubSubType());
    request_type.register_type(participant);
    reply_type.register_type(participant);

    // Create Topics
    Topic* request_topic = participant->create_topic("RequestTopic", "RBIC1::GenericRequest", TOPIC_QOS_DEFAULT);
    Topic* reply_topic = participant->create_topic("ReplyTopic", "RBIC1::GenericReply", TOPIC_QOS_DEFAULT);

    // Create Subscriber and DataReader
    Subscriber* subscriber = participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT);
    DataReader* reader = subscriber->create_datareader(request_topic, DATAREADER_QOS_DEFAULT);

    // Check subscription matching
    SubscriptionMatchedStatus matched_status;
    while (reader->get_subscription_matched_status(matched_status) == ReturnCode_t::RETCODE_OK &&
           matched_status.current_count == 0) {
        std::cout << "Proxy: Waiting for subscription matching..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Proxy: Subscription matched!" << std::endl;

    // Create Publisher and DataWriter
    Publisher* publisher = participant->create_publisher(PUBLISHER_QOS_DEFAULT);
    DataWriter* writer = publisher->create_datawriter(reply_topic, DATAWRITER_QOS_DEFAULT);

    // Create a WaitSet and attach a ReadCondition
    WaitSet wait_set;
    ReadCondition* read_condition = reader->create_readcondition(
        eprosima::fastdds::dds::ANY_SAMPLE_STATE,
        eprosima::fastdds::dds::ANY_VIEW_STATE,
        eprosima::fastdds::dds::ANY_INSTANCE_STATE);
    wait_set.attach_condition(*read_condition);

    while (true) {
        // Wait for data to be available
        ConditionSeq active_conditions;
        wait_set.wait(active_conditions, eprosima::fastrtps::c_TimeInfinite);

        // Take the sample
        RBIC1::GenericRequest request;
        SampleInfo info;
        if (reader->take_next_sample(&request, &info) == ReturnCode_t::RETCODE_OK && info.valid_data) {
            std::cout << "Proxy: Received request with ID " << request.requestId() << std::endl;

            // Create a reply
            RBIC1::GenericReply reply;
            reply.requestId(request.requestId());
            reply.reply().dllVersionReply(RBIC1::DLLVersionReply());
            reply.reply().dllVersionReply().success(true);
            reply.reply().dllVersionReply().version("1.0.0");

            // Send the reply
            writer->write(&reply);
            std::cout << "Proxy: Sent reply with version 1.0.0" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }
    }

    // Clean up
    reader->delete_readcondition(read_condition);
    participant->delete_contained_entities();
    DomainParticipantFactory::get_instance()->delete_participant(participant);
}

// Invoker function using WaitSet
void run_invoker() {
    std::cout << "Invoker: Starting..." << std::endl;

    // Create DomainParticipant
    DomainParticipant* participant = DomainParticipantFactory::get_instance()->create_participant(0, PARTICIPANT_QOS_DEFAULT);

    // Register types
    TypeSupport request_type(new RBIC1::GenericRequestPubSubType());
    TypeSupport reply_type(new RBIC1::GenericReplyPubSubType());
    request_type.register_type(participant);
    reply_type.register_type(participant);

    // Create Topics
    Topic* request_topic = participant->create_topic("RequestTopic", "RBIC1::GenericRequest", TOPIC_QOS_DEFAULT);
    Topic* reply_topic = participant->create_topic("ReplyTopic", "RBIC1::GenericReply", TOPIC_QOS_DEFAULT);

    // Create Publisher and DataWriter
    Publisher* publisher = participant->create_publisher(PUBLISHER_QOS_DEFAULT);
    DataWriter* writer = publisher->create_datawriter(request_topic, DATAWRITER_QOS_DEFAULT);

    // Check publication matching
    PublicationMatchedStatus matched_status;
    while (writer->get_publication_matched_status(matched_status) == ReturnCode_t::RETCODE_OK &&
           matched_status.current_count == 0) {
        std::cout << "Invoker: Waiting for publication matching..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Invoker: Publication matched!" << std::endl;

    // Create Subscriber and DataReader
    Subscriber* subscriber = participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT);
    DataReader* reader = subscriber->create_datareader(reply_topic, DATAREADER_QOS_DEFAULT);

    // Create a WaitSet and attach a ReadCondition
    WaitSet wait_set;
    ReadCondition* read_condition = reader->create_readcondition(
        eprosima::fastdds::dds::ANY_SAMPLE_STATE,
        eprosima::fastdds::dds::ANY_VIEW_STATE,
        eprosima::fastdds::dds::ANY_INSTANCE_STATE);
    wait_set.attach_condition(*read_condition);

    // Send a request
    RBIC1::GenericRequest request;
    request.requestId(1);
    request.request().dllVersionRequest(RBIC1::DLLVersionRequest());
    writer->write(&request);
    std::cout << "Invoker: Sent request with ID 1" << std::endl;

    // Wait for a reply
    ConditionSeq active_conditions;
    wait_set.wait(active_conditions, eprosima::fastrtps::c_TimeInfinite);

    std::cout << "Invoker: Received reply" << std::endl;

    // Take the reply
    RBIC1::GenericReply reply;
    SampleInfo info;
    if (reader->take_next_sample(&reply, &info) == ReturnCode_t::RETCODE_OK && info.valid_data) {
        if (reply.reply()._d() == RBIC1::DLLVERSION_REPLY) {
            std::cout << "Invoker: Received reply with version: " << reply.reply().dllVersionReply().version() << std::endl;
        }
    }

    // Clean up
    reader->delete_readcondition(read_condition);
    participant->delete_contained_entities();
    DomainParticipantFactory::get_instance()->delete_participant(participant);
}

int main() {
    std::cout << "Main: Starting..." << std::endl;

    // Initialize Fast DDS logging
    // Log::SetVerbosity(Log::Kind::Info);  // Set logging level to Info
    // Log::ReportFilenames(true);         // Include filenames in log output
    // Log::ReportFunctions(true);         // Include function names in log output


    float C0, C1;
    if (RBIC1::OutLin(1.0f, 2.0f, 3.0f, 4.0f, &C0, &C1)) {
        std::cout << "OutLin succeeded: C0 = " << C0 << ", C1 = " << C1 << std::endl;
    } else {
        std::cout << "OutLin failed!" << std::endl;
    }

    // Test DLL version
    char version[256];
    if (RBIC1::DLLVersion(version)) {
        std::cout << "DLL Version: " << version << std::endl;
    } else {
        std::cout << "Failed to get DLL version!" << std::endl;
    }


    // Run the proxy and invoker in separate threads
    std::jthread proxy_thread([] {
        run_proxy();
    });

    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // Ensure proxy is ready

    std::jthread invoker_thread([] {
        run_invoker();
    });

    std::cout << "Main: Waiting for threads to finish..." << std::endl;

    // Flush logs before exiting
    // Log::Flush();

    return 0;
}