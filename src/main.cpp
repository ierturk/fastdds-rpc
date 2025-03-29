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
#include <fastdds/dds/log/Log.hpp>  // Include Fast DDS logging
#include "RBIC1.h"                 // Generated from RBIC1.idl
#include "RBIC1PubSubTypes.h"      // Generated by fastddsgen for serialization

using namespace eprosima::fastdds::dds;

// Function prototypes
void run_proxy();
void run_invoker();

int main() {
    // Enable Fast DDS logging
    Log::SetVerbosity(Log::Kind::Info);  // Set logging level to Info
    // Log::SetCategoryFilter(std::regex("RTPS"));     // Optional: Filter logs by category (e.g., RTPS)

    // Run the proxy and invoker in separate threads
    std::thread proxy_thread(run_proxy);
    // Sleep for a short time to ensure the proxy is ready before starting the invoker
    std::thread invoker_thread(run_invoker);

    // Wait for both threads to finish
    proxy_thread.join();
    invoker_thread.join();

    // Clean up logging
    Log::Flush();
    Log::KillThread();

    return 0;
}

void run_proxy() {
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

    // Create Publisher and DataWriter
    Publisher* publisher = participant->create_publisher(PUBLISHER_QOS_DEFAULT);
    DataWriter* writer = publisher->create_datawriter(reply_topic, DATAWRITER_QOS_DEFAULT);

    // Listen for requests
    RBIC1::GenericRequest request;
    SampleInfo info;
    while (true) {
        if (reader->take_next_sample(&request, &info) == ReturnCode_t::RETCODE_OK) {
            if (info.valid_data) {
                // Check if the request is for DLL version
                if (request.request()._d() == RBIC1::DLLVERSION_REQUEST) {
                    std::cout << "Proxy: Received DLLVersionRequest" << std::endl;

                    // Create a reply
                    RBIC1::GenericReply reply;
                    reply.requestId(request.requestId());
                    reply.reply().dllVersionReply(RBIC1::DLLVersionReply());
                    reply.reply().dllVersionReply().success(true);
                    reply.reply().dllVersionReply().version("1.0.0");

                    // Send the reply
                    writer->write(&reply);
                    std::cout << "Proxy: Sent DLLVersionReply with version 1.0.0" << std::endl;
                }
            }
        }
    }

    // Clean up
    participant->delete_contained_entities();
    DomainParticipantFactory::get_instance()->delete_participant(participant);
}

void run_invoker() {
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

    // Create Subscriber and DataReader
    Subscriber* subscriber = participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT);
    DataReader* reader = subscriber->create_datareader(reply_topic, DATAREADER_QOS_DEFAULT);

    // Wait for publication match
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Send a DLLVersionRequest
    RBIC1::GenericRequest request;
    request.requestId(1);
    request.request().dllVersionRequest(RBIC1::DLLVersionRequest());
    writer->write(&request);
    std::cout << "Invoker: Sent DLLVersionRequest" << std::endl;

    // Wait for a reply
    RBIC1::GenericReply reply;
    SampleInfo info;
    while (true) {
        if (reader->take_next_sample(&reply, &info) == ReturnCode_t::RETCODE_OK) {
            if (info.valid_data && reply.requestId() == 1) {
                if (reply.reply()._d() == RBIC1::DLLVERSION_REPLY) {
                    std::cout << "Invoker: Received DLLVersionReply with version: "
                              << reply.reply().dllVersionReply().version() << std::endl;
                    break;
                }
            }
        }
    }

    // Clean up
    participant->delete_contained_entities();
    DomainParticipantFactory::get_instance()->delete_participant(participant);
}