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

#include "SimpleApp.h"

Define_Module(SimpleApp);

void SimpleApp::initialize(int stage) {
	if (stage == 0) {
		toDecisionMaker = findGate("toDecisionMaker");
		fromDecisionMaker = findGate("fromDecisionMaker");
		cModule *tmpMobility = getParentModule()->getSubmodule("veinsmobility");
		Veins::TraCIMobility* mobility = dynamic_cast<Veins::TraCIMobility *>(tmpMobility);
		ASSERT(mobility);
		sumoId = mobility->getExternalId();
		scheduleAt(simTime() + 0.5, new cMessage("Send"));
	}
}

void SimpleApp::handleMessage(cMessage *msg) {
	if (msg->isSelfMessage()) {
		/*
		 * Send a message to a random node in the network. Note that only the most necessary values
		 * are set. Size of the message have to be set according to the real message (aka your used
		 * .msg file). The values here are just a temporary placeholder.
		 */
		HeterogeneousMessage *testMessage = new HeterogeneousMessage();
		testMessage->setNetworkType(DONTCARE);
		testMessage->setName("Heterogeneous Test Message");
		testMessage->setByteLength(10);

		/* choose a random other car to send the message to */
		TraCIScenarioManager* manager = TraCIScenarioManagerAccess().get();
		std::map<std::string, cModule*> hosts = manager->getManagedHosts();
		std::map<std::string, cModule*>::iterator it = hosts.begin();
		std::advance(it, intrand(hosts.size()));
		std::string destination("node[" + it->first + "]");
		std::cout << "[" << sumoId << ", " << simTime() <<  "] Sending message to " << destination << std::endl;
		testMessage->setDestinationAddress(destination.c_str());

		/* Finish the message and send it */
		testMessage->setSourceAddress(sumoId.c_str());
		send(testMessage, toDecisionMaker);

		/*
		 * At 25% of the time send also a message to the main server. This message is sent via LTE
		 * and is then simply handed to the decision maker.
		 */
		if(dblrand() < 0.25){
			std::cout << "[" << sumoId << ", " << simTime() <<  "] Sending message also to server" << std::endl;
			HeterogeneousMessage* serverMessage = new HeterogeneousMessage();
			serverMessage->setName("Server Message Test");
			testMessage->setByteLength(10);
			serverMessage->setNetworkType(LTE);
			serverMessage->setDestinationAddress("server");
			serverMessage->setSourceAddress(sumoId.c_str());
			send(serverMessage, toDecisionMaker);
		}

		scheduleAt(simTime() + 1, new cMessage("Send"));
	} else {
		HeterogeneousMessage *testMessage = dynamic_cast<HeterogeneousMessage *>(msg);
		std::cout << "[" << getParentModule()->getFullPath() << ", " << simTime() << "] Received message " << msg->getFullPath() << "< from " << testMessage->getSourceAddress() << std::endl;
	}
}
