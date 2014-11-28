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

#include "veins/modules/lte/DecisionMaker.h"

Define_Module(DecisionMaker);

/*
 * A simple implementation of the decision maker. In case of LTE or DSRC it sends the packet via the
 * associated technology. In case of DONTCARE it randomly chooses one of the two channels.
 */

DecisionMaker::DecisionMaker() {}

DecisionMaker::~DecisionMaker() {}

void DecisionMaker::sendLteMessage(HeterogeneousMessage* msg) {
	MobilityBase* eNodeBMobility = dynamic_cast<MobilityBase*>(
			getModuleByPath("scenario.eNodeB1")->getSubmodule("mobility")
			);
	ASSERT(eNodeBMobility);
	AnnotationManager* annotations = AnnotationManagerAccess().getIfExists();
	annotations->scheduleErase(0.25, annotations->drawLine(
			eNodeBMobility->getCurrentPosition(), getPosition(), "red")
			);
	send(msg, toLte);
}

void DecisionMaker::sendDSRCMessage(HeterogeneousMessage* msg) {
	msg->setBitLength(0);
	msg->addBitLength(headerLength);
	msg->addByteLength(64);
	msg->setChannelNumber(Channels::CCH);
	msg->setPsid(0);
	msg->setPriority(dataPriority);
	msg->setWsmVersion(2);
	msg->setTimestamp(simTime());
	msg->setRecipientAddress(BROADCAST);
	msg->setSenderPos(curPosition);
	msg->setSerial(2);
	sendWSM(msg);
}

void DecisionMaker::sendDontCareMessage(HeterogeneousMessage* msg) {
	srand(time(NULL));
	if ((rand() % 100) > 50) {
		sendLteMessage(msg);
	} else {
		sendDSRCMessage(msg);
	}
}
