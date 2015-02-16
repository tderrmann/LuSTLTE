#include "ExampleDecisionMaker.h"

Define_Module(ExampleDecisionMaker);

ExampleDecisionMaker::ExampleDecisionMaker(){
	// Empty
}

ExampleDecisionMaker::~ExampleDecisionMaker(){
	// Empty
}

void ExampleDecisionMaker::sendLteMessage(HeterogeneousMessage* msg) {
	// MobilityBase* eNodeBMobility = dynamic_cast<MobilityBase*>(getModuleByPath("scenario.eNodeB1")->getSubmodule("mobility"));
	// ASSERT(eNodeBMobility);
	// AnnotationManager* annotations = AnnotationManagerAccess().getIfExists();
	// annotations->scheduleErase(1, annotations->drawLine(eNodeBMobility->getCurrentPosition(), getPosition(), "yellow"));
	send(msg, toLte);
}

void ExampleDecisionMaker::sendDSRCMessage(HeterogeneousMessage* msg) {
	msg->setBitLength(0);
	msg->addBitLength(headerLength);
	msg->addByteLength(64);
	msg->setChannelNumber(Channels::CCH);
	msg->setPsid(0);
	// msg->setPriority(dataPriority);
	msg->setWsmVersion(2);
	msg->setTimestamp(simTime());
	msg->setRecipientAddress(BROADCAST);
	msg->setSenderPos(currentPosition);
	msg->setSerial(2);
	sendWSM(msg);
}

void ExampleDecisionMaker::sendDontCareMessage(HeterogeneousMessage* msg) {
	srand(time(NULL));
	if ((rand() % 100) > 50) {
		msg->setNetworkType(LTE);
		sendLteMessage(msg);
	} else {
		msg->setNetworkType(DSRC);
		sendDSRCMessage(msg);
	}
}
