#ifndef CINSTREAMDATACHANNEL_H
#define CINSTREAMDATACHANNEL_H

#include "rdr/BufferedInStream.h"
#include "Channel.h"

class CInStreamDataChannel : public rdr::BufferedInStream
{
public:
    CInStreamDataChannel(CChannel* pDataChannel);
    
private:
    // rdr::BufferedInStream interface
    virtual bool fillBuffer(size_t maxSize) override;
    
    CChannel* m_pDataChannel;
};

#endif // CINSTREAMDATACHANNEL_H
