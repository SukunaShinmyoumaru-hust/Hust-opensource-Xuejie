#include "stdafx.h"
#include "Global.h"
#include "RdtSender.h"
#include "RdtReceiver.h"
#include "StopWaitRdtSender.h"
#include "StopWaitRdtReceiver.h"


int main(int argc, char* argv[])
{
	RdtSender *ps = new GBNSender();
	RdtReceiver * pr = new GBNReceiver();
//	pns->setRunMode(0);  
	pns->setRunMode(1);  
	pns->init();
	pns->setRtdSender(ps);
	pns->setRtdReceiver(pr);
	pns->setInputFile("C:\\GBN\\input.txt");
	pns->setOutputFile("C:\\GBN\\output.txt");

	pns->start();

	delete ps;
	delete pr;
	delete pUtils;									
	delete pns;										
	return 0;
}

