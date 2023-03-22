#ifndef STOP_WAIT_RDT_RECEIVER_H
#define STOP_WAIT_RDT_RECEIVER_H


#include "RdtReceiver.h"
class GBNReceiver :public RdtReceiver
{
private:
    int seq; //期待收到的下一个报文序号
    Packet lastAckPkt;            //上次发送的确认报文
public:
    GBNReceiver();
    virtual ~GBNReceiver();

public:
    void receive(const Packet& packet); //接受报文，被NetworkService调用
};
#endif //STOPWAIT_GBNRECEIVER_H

