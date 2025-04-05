#ifndef RBIC1INVOKERIFC_H
#define RBIC1INVOKERIFC_H

#include <string>

// Interface class for RBIC1Invoker
class RBIC1InvokerIfc {
public:
    virtual ~RBIC1InvokerIfc() = default;

    // Pure virtual methods
    virtual void init(const std::string& reader_topic, const std::string& writer_topic) = 0;
    virtual void run() = 0;
};

#endif // RBIC1INVOKERIFC_H