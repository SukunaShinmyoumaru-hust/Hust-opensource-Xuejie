#pragma once
#include "RdtReceiver.h"
class SRRdtReceiver :
	public RdtReceiver
{
private:
	//sr协议接收方也需要缓冲区和滑动窗口
	const int wndsize;//窗口大小
	const int seqsize;
	Packet lastAckPkt;//上一个收到的ACK
	Packet* const recvBuf;//分组缓存区
	bool* const bufStatus;//分组的状态
	int base;//int nextSeqnum;	

private:
	void Init();
	void printSlideWindow();
	bool isInWindow(int seqnum);

public:
	SRRdtReceiver();
	SRRdtReceiver(int sSize, int wsize);
	void receive(const Packet& packet);
	virtual ~SRRdtReceiver();
};