#ifndef RPCDDS_H
#define RPCDDS_H

#include <iostream>
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
#include <fastdds/dds/topic/TypeSupport.hpp>

template <typename ReaderType, typename WriterType, typename ReaderPubSubType, typename WriterPubSubType>
class RpcDds {
public:
    RpcDds() : participant_(nullptr), writer_(nullptr), reader_(nullptr) {}

    ~RpcDds() {
        cleanup();
    }

    // Initialize common components
    void init(const std::string& reader_topic_name, const std::string& writer_topic_name) {
        // Create DomainParticipant
        participant_ = eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->create_participant(
            0, eprosima::fastdds::dds::PARTICIPANT_QOS_DEFAULT);
        if (!participant_) {
            throw std::runtime_error("Failed to create DomainParticipant");
        }

        // Register types
        eprosima::fastdds::dds::TypeSupport reader_type(new ReaderPubSubType());
        eprosima::fastdds::dds::TypeSupport writer_type(new WriterPubSubType());
        reader_type.register_type(participant_);
        writer_type.register_type(participant_);

        // Create Topics
        reader_topic_ = participant_->create_topic(
            reader_topic_name, reader_type.get_type_name(), eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);
        writer_topic_ = participant_->create_topic(
            writer_topic_name, writer_type.get_type_name(), eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);

        if (!reader_topic_ || !writer_topic_) {
            throw std::runtime_error("Failed to create topics");
        }

        // Create Subscriber and DataReader
        subscriber_ = participant_->create_subscriber(eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT);
        if (!subscriber_) {
            throw std::runtime_error("Failed to create Subscriber");
        }
        reader_ = subscriber_->create_datareader(reader_topic_, eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);
        if (!reader_) {
            throw std::runtime_error("Failed to create DataReader");
        }

        // Create Publisher and DataWriter
        publisher_ = participant_->create_publisher(eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT);
        if (!publisher_) {
            throw std::runtime_error("Failed to create Publisher");
        }
        writer_ = publisher_->create_datawriter(writer_topic_, eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT);
        if (!writer_) {
            throw std::runtime_error("Failed to create DataWriter");
        }

        std::cout << "RpcDds: Initialization complete." << std::endl;
    }

    // Accessors
    eprosima::fastdds::dds::DataWriter* getWriter() const {
        return writer_;
    }

    eprosima::fastdds::dds::DataReader* getReader() const {
        return reader_;
    }

    eprosima::fastdds::dds::DomainParticipant* getParticipant() const {
        return participant_;
    }

    // Cleanup resources
    void cleanup() {
        if (participant_) {
            participant_->delete_contained_entities();
            eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->delete_participant(participant_);
            participant_ = nullptr;
        }
    }

private:
    eprosima::fastdds::dds::DomainParticipant* participant_;
    eprosima::fastdds::dds::Publisher* publisher_;
    eprosima::fastdds::dds::Subscriber* subscriber_;
    eprosima::fastdds::dds::Topic* reader_topic_;
    eprosima::fastdds::dds::Topic* writer_topic_;
    eprosima::fastdds::dds::DataWriter* writer_;
    eprosima::fastdds::dds::DataReader* reader_;
};

#endif // RPCDDS_H