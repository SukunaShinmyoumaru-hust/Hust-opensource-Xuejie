#include "stdafx.h"
#include "StopWaitRdtSender.h"
#include "Tool.h"
#include "Global.h"

void SRRdtSender::Init()
{
	base = nextSeqnum = 0;
	for (int i = 0; i < seqsize; i++)
	{
		bufStatus[i] = false;
	}
}

void SRRdtSender::printSlideWindow()
{
	int i;
	for (i = 0; i < seqsize; i++)
	{
		if (i == base)
			std::cout << "[";
		if (isInWindow(i) && i >= nextSeqnum)			//可用，未发送
			std::cout << "可用";
		else if (isInWindow(i) && bufStatus[i] == true)		//发送，已确认
			std::cout << "已确认";
		else if (isInWindow(i))						//已发送，未确认
			std::cout << "已发送";
		if (i == (base + wndsize) % seqsize)
			std::cout << "]";
		if (isInWindow(i) == false)
			std::cout << "不可用";						//不可用，窗口外
		std::cout << " ";
	}
	std::cout << std::endl;
}

//判断序号是否在窗口内
bool SRRdtSender::isInWindow(int seqnum)
{
	//return seqnum >= base && seqnum <= (base + wndsize) % seqsize;
	if (base < (base + wndsize) % seqsize)
	{
		return seqnum >= base && seqnum < (base + wndsize) % seqsize;
	}
	else
	{
		return seqnum >= base || seqnum < (base + wndsize) % seqsize;
	}
}

SRRdtSender::SRRdtSender(int sSize, int wsize) :
	seqsize(sSize), wndsize(wsize), sendBuf(new Packet[sSize]), bufStatus(new bool[sSize])
{
	Init();
}

SRRdtSender::SRRdtSender() :
	seqsize(8), wndsize(4), sendBuf(new Packet[8]), bufStatus(new bool[8])
{
	Init();
}

bool SRRdtSender::send(const Message& message)
{
	if (getWaitingState())
	{//缓冲区满，等待ack
		std::cout << "ERROR:窗口已满，发送数据请稍等\n\n";
		return false;
	}
	sendBuf[nextSeqnum].acknum = -1;
	sendBuf[nextSeqnum].seqnum = nextSeqnum;
	memcpy(sendBuf[nextSeqnum].payload, message.data, sizeof(message.data));
	sendBuf[nextSeqnum].checksum = pUtils->calculateCheckSum(sendBuf[nextSeqnum]);
	pUtils->printPacket("发送方发送报文", sendBuf[nextSeqnum]);
	//显示滑动窗口
	std::cout << "\n发送前窗口：";
	printSlideWindow();
	//发送报文
	pns->sendToNetworkLayer(RECEIVER, sendBuf[nextSeqnum]);
	//启动定时器，sr协议中每个分组对应一个定时器
	pns->startTimer(SENDER, Configuration::TIME_OUT, nextSeqnum);
	//发送完毕，更新状态
	nextSeqnum = (nextSeqnum + 1) % seqsize;//别忘了取模
	std::cout << "发送后窗口：";
	printSlideWindow();
	std::cout << std::endl;
	return true;
}


bool SRRdtSender::getWaitingState()
{
	WaitingState = (base + wndsize) % seqsize == (nextSeqnum) % seqsize;
	return WaitingState;
}

void SRRdtSender::receive(const Packet& ackPkt)
{
	int checksum = pUtils->calculateCheckSum(ackPkt);
	if (checksum != ackPkt.checksum)
	{
		pUtils->printPacket("ERROR:接收的ack损坏，校验和不相等", ackPkt);
		return;
	}
	else
	{
		pns->stopTimer(SENDER, ackPkt.acknum);
		if (isInWindow(ackPkt.acknum))
		{//更新窗口
			bufStatus[ackPkt.acknum] = true;
			while (bufStatus[base] == true)
			{//移动base
				bufStatus[base] = false;
				base = (base + 1) % seqsize;
			}
			std::cout << "收到ack，滑动窗口移动：\n";
			printSlideWindow();
			std::cout << std::endl;
		}
	}
}

void SRRdtSender::timeoutHandler(int seqnum)
{
	pUtils->printPacket("ERROR:发送方定时器时间到，重发上一次发送的报文", sendBuf[seqnum]);
	//不用管其他的序号，接收方有缓冲区，即使失序也不要紧
	pns->sendToNetworkLayer(RECEIVER, sendBuf[seqnum]);
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqnum);
	pUtils->printPacket("重发数据完毕", sendBuf[seqnum]);
}

SRRdtSender::~SRRdtSender()
{
	delete[] bufStatus;
	delete[] sendBuf;
}