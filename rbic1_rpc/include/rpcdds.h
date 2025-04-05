#ifndef RPCDDS_H
#define RPCDDS_H

#include "RpcDdsIfc.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

template <typename ReaderType, typename WriterType, typename ReaderPubSubType, typename WriterPubSubType>
class RpcDds : public RpcDdsIfc<ReaderType, WriterType> {
public:
    RpcDds() = default;

    ~RpcDds() override {
        cleanup();
    }

    void init(const std::string& reader_topic_name, const std::string& writer_topic_name) override {
        // Create DomainParticipant
        participant_ = std::unique_ptr<eprosima::fastdds::dds::DomainParticipant>(
            eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->create_participant(
                0, eprosima::fastdds::dds::PARTICIPANT_QOS_DEFAULT));
        if (!participant_) {
            throw std::runtime_error("Failed to create DomainParticipant");
        }

        try {
            // Register types
            eprosima::fastdds::dds::TypeSupport reader_type(new ReaderPubSubType());
            eprosima::fastdds::dds::TypeSupport writer_type(new WriterPubSubType());
            reader_type.register_type(participant_.get());
            writer_type.register_type(participant_.get());

            // Create Topics
            reader_topic_ = std::unique_ptr<eprosima::fastdds::dds::Topic>(
                participant_->create_topic(reader_topic_name, reader_type.get_type_name(), eprosima::fastdds::dds::TOPIC_QOS_DEFAULT));
            writer_topic_ = std::unique_ptr<eprosima::fastdds::dds::Topic>(
                participant_->create_topic(writer_topic_name, writer_type.get_type_name(), eprosima::fastdds::dds::TOPIC_QOS_DEFAULT));

            if (!reader_topic_ || !writer_topic_) {
                throw std::runtime_error("Failed to create topics");
            }

            // Create Subscriber and DataReader
            subscriber_ = std::unique_ptr<eprosima::fastdds::dds::Subscriber>(
                participant_->create_subscriber(eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT));
            if (!subscriber_) {
                throw std::runtime_error("Failed to create Subscriber");
            }
            reader_ = std::unique_ptr<eprosima::fastdds::dds::DataReader>(
                subscriber_->create_datareader(reader_topic_.get(), eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT));
            if (!reader_) {
                throw std::runtime_error("Failed to create DataReader");
            }

            // Create Publisher and DataWriter
            publisher_ = std::unique_ptr<eprosima::fastdds::dds::Publisher>(
                participant_->create_publisher(eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT));
            if (!publisher_) {
                throw std::runtime_error("Failed to create Publisher");
            }
            writer_ = std::unique_ptr<eprosima::fastdds::dds::DataWriter>(
                publisher_->create_datawriter(writer_topic_.get(), eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT));
            if (!writer_) {
                throw std::runtime_error("Failed to create DataWriter");
            }

            std::cout << "RpcDds: Initialization complete." << std::endl;
        } catch (...) {
            cleanup();
            throw;
        }
    }

    eprosima::fastdds::dds::DataWriter* getWriter() const override {
        if (!writer_) {
            throw std::runtime_error("DataWriter is not initialized");
        }
        return writer_.get();
    }

    eprosima::fastdds::dds::DataReader* getReader() const override {
        if (!reader_) {
            throw std::runtime_error("DataReader is not initialized");
        }
        return reader_.get();
    }

    void cleanup() override {
        if (participant_) {
            if (publisher_) {
                publisher_->delete_datawriter(writer_.release());
            }
            if (subscriber_) {
                subscriber_->delete_datareader(reader_.release());
            }
            if (reader_topic_) {
                participant_->delete_topic(reader_topic_.release());
            }
            if (writer_topic_) {
                participant_->delete_topic(writer_topic_.release());
            }
            eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->delete_participant(participant_.release());
        }
    }

private:
    std::unique_ptr<eprosima::fastdds::dds::DomainParticipant> participant_;
    std::unique_ptr<eprosima::fastdds::dds::Publisher> publisher_;
    std::unique_ptr<eprosima::fastdds::dds::Subscriber> subscriber_;
    std::unique_ptr<eprosima::fastdds::dds::Topic> reader_topic_;
    std::unique_ptr<eprosima::fastdds::dds::Topic> writer_topic_;
    std::unique_ptr<eprosima::fastdds::dds::DataWriter> writer_;
    std::unique_ptr<eprosima::fastdds::dds::DataReader> reader_;
};

#endif // RPCDDS_H