#ifndef RPCDDSIFC_H
#define RPCDDSIFC_H

#include <string>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>

// Interface class for RpcDds
template <typename ReaderType, typename WriterType>
class RpcDdsIfc {
public:
    virtual ~RpcDdsIfc() = default;

    // Pure virtual methods
    virtual void init(const std::string& reader_topic_name, const std::string& writer_topic_name) = 0;
    virtual eprosima::fastdds::dds::DataWriter* getWriter() const = 0;
    virtual eprosima::fastdds::dds::DataReader* getReader() const = 0;
    virtual void cleanup() = 0;
};

#endif // RPCDDSIFC_H