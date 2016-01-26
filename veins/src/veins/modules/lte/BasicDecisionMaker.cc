//
// Copyright (C) 2006-2014 Florian Hagenauer <hagenauer@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "BasicDecisionMaker.h"
#include "LtePhyUe.h"
/*
 * A simple implementation of the decision maker. In case of LTE or DSRC it sends the packet via the
 * associated technology. In case of DONTCARE it randomly chooses one of the two channels.
 */

BasicDecisionMaker::BasicDecisionMaker(){
	// Empty
}

BasicDecisionMaker::~BasicDecisionMaker(){
	// Empty
}

void BasicDecisionMaker::initialize(int stage) {
    BaseApplLayer::initialize(stage);
    if (stage == 0) {
        DSRCMessagesSent = registerSignal("DSRCMessagesSent");
        lteMessagesSent = registerSignal("lteMessagesSent");
        dontCareMessagesSent = registerSignal("dontCareMessagesSent");
        DSRCMessagesReceived = registerSignal("DSRCMessagesReceived");
        lteMessagesReceived = registerSignal("lteMessagesReceived");
            fromApplication = findGate("fromApplication");
        toApplication = findGate("toApplication");
        fromLte = findGate("fromLte");
        toLte = findGate("toLte");
        fromDSRC = findGate("lowerLayerIn");
        toDSRC = findGate("lowerLayerOut");
        cModule *tmpMobility = getParentModule()->getSubmodule("veinsmobility");
        Veins::TraCIMobility* mobility = dynamic_cast<Veins::TraCIMobility *>(tmpMobility);
        ASSERT(mobility);
        id = mobility->getExternalId();

        maxOffset = par("maxOffset").doubleValue();
        individualOffset = dblrand() * maxOffset;
        findHost()->subscribe(mobilityStateChangedSignal, this);
    }
}

void BasicDecisionMaker::finish() {
    // Empty
}

void BasicDecisionMaker::handleMessage(cMessage* msg){
    int arrivalGate = msg->getArrivalGateId();
    if (arrivalGate == fromApplication) {
        HeterogeneousMessage *heterogeneousMessage =
                dynamic_cast<HeterogeneousMessage *>(msg);
        if (!heterogeneousMessage) {
            std::cout << "Message " << msg->getFullName()
                      << " is not a HeterogeneousMessage, but a "
                      << msg->getClassName() << std::endl;
            delete msg;
            return;
        }
        switch (heterogeneousMessage->getNetworkType()) {
            case DONTCARE:
                emit(dontCareMessagesSent, 1);
                sendDontCareMessage(heterogeneousMessage);
                break;
            case LTE:
            	emit(lteMessagesSent, 1);
                sendLteMessage(heterogeneousMessage);
                break;
            case DSRC:
            	emit(DSRCMessagesSent, 1);
                sendDSRCMessage(heterogeneousMessage);
                break;

        }
    } else {
        handleLowerMessage(msg);
    }
}

void BasicDecisionMaker::handleLowerMessage(cMessage* msg){
	int arrivalGate = msg->getArrivalGateId();
	if (arrivalGate == fromLte) {
	    int messageKind = msg->getKind();
	    if (messageKind == UDP_I_DATA) {
	        msg->setKind(CAM_TYPE);
	    } else if (messageKind == UDP_I_ERROR) {
	        msg->setKind(CAM_ERROR_TYPE);
	    } else {
	        WARN_DM("Unknown message kind " << messageKind);
	    }
	    emit(lteMessagesReceived, 1);
	
	std::stringstream ss;
	ss << "scenario.eNodeB" << getAncestorPar("masterId").longValue();

	std::string eNBpath = ss.str();
	MobilityBase* eNodeBMobility = dynamic_cast<MobilityBase*>(getModuleByPath(eNBpath.c_str())->getSubmodule("mobility"));
	ASSERT(eNodeBMobility);
	PRINT_DM(eNBpath);
	/*MobilityBase* eNodeBMobility = dynamic_cast<MobilityBase*>(
			getModuleByPath("scenario.eNodeB1")->getSubmodule("mobility")
			);
	ASSERT(eNodeBMobility);*/
	AnnotationManager* annotations = AnnotationManagerAccess().getIfExists();
	annotations->scheduleErase(2.5, annotations->drawLine(
			eNodeBMobility->getCurrentPosition(), getPosition(), "green")
			);
/*
		MobilityBase* eNodeBMobility = dynamic_cast<MobilityBase*>(getModuleByPath("scenario.eNodeB1")->getSubmodule("mobility"));
		ASSERT(eNodeBMobility);
		AnnotationManager* annotations = AnnotationManagerAccess().getIfExists();
		annotations->scheduleErase(0.25,annotations->drawLine(eNodeBMobility->getCurrentPosition(), getPosition(),"yellow"));*/
	    send(msg, toApplication);
	} else if (arrivalGate == fromDSRC) {
	      HeterogeneousMessage* tmpMessage = dynamic_cast<HeterogeneousMessage*>(msg);
	      ASSERT(tmpMessage);
	      AnnotationManager* annotations = AnnotationManagerAccess().getIfExists();
	      annotations->scheduleErase(0.5, annotations->drawLine(
	          tmpMessage->getSenderPos(), getPosition(), "blue")
	          );
		emit(DSRCMessagesReceived, 1);
	    send(msg, toApplication);
	} else {
	    std::cout << "Unknown arrival gate " << msg->getArrivalGate()->getFullName()
	            << std::endl;
	    delete msg;
	}
}

void BasicDecisionMaker::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj) {
    Enter_Method_Silent();
    if (signalID == mobilityStateChangedSignal) {
        handlePositionUpdate(obj);
    }
}

void BasicDecisionMaker::handlePositionUpdate(cObject* obj) {
   ChannelMobilityPtrType const mobility = check_and_cast<ChannelMobilityPtrType>(obj);
   currentPosition = mobility->getCurrentPosition();
}

void BasicDecisionMaker::sendWSM(WaveShortMessage* wsm) {
    sendDelayedDown(wsm,individualOffset);
}

void BasicDecisionMaker::sendLteMessage(HeterogeneousMessage* msg) {
	//draw line to current eNB
	/*cModule* senderModule = msg->getSenderModule();
	//HeterogeneousCar* car = dynamic_cast<HeterogeneousCar*>(senderModule);
	LtePhyUe* phy = dynamic_cast<LtePhyUe*>(senderModule->getSubmodule("nic")->getSubmodule("phy"));
	unsigned short masterId = phy->getMasterId();*/
	std::stringstream ss;
	ss << "scenario.eNodeB" << getAncestorPar("masterId").longValue();
	std::string eNBpath = ss.str();
	MobilityBase* eNodeBMobility = dynamic_cast<MobilityBase*>(getModuleByPath(eNBpath.c_str())->getSubmodule("mobility"));
	ASSERT(eNodeBMobility);
	PRINT_DM(eNBpath);
	/*MobilityBase* eNodeBMobility = dynamic_cast<MobilityBase*>(
			getModuleByPath("scenario.eNodeB1")->getSubmodule("mobility")
			);
	ASSERT(eNodeBMobility);*/
	AnnotationManager* annotations = AnnotationManagerAccess().getIfExists();
	annotations->scheduleErase(2.5, annotations->drawLine(
			eNodeBMobility->getCurrentPosition(), getPosition(), "green")
			);
	send(msg, toLte);
}

void BasicDecisionMaker::sendDSRCMessage(HeterogeneousMessage* msg) {
	msg->addBitLength(headerLength);
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

void BasicDecisionMaker::sendDontCareMessage(HeterogeneousMessage* msg) {
	if (dblrand() > 0.5) {
		sendLteMessage(msg);
	} else {
		sendDSRCMessage(msg);
	}
}
