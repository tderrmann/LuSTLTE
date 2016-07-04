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
#include "LtePhyUe.h"
#include "TraCIMobility.h"
/*
#include <iostream>   // std::cout
#include <string>     // std::string, std::stod
#include <stdlib.h>
#include <algorithm>
*/

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
/* hack for computing transformed coordinates of eNBs
	//TODO thierry: update masterId to nic.phy.masterId_
	//cModule *tmpMobility = getParentModule()->getSubmodule("veinsmobility");
	//Veins::TraCIMobility* mobility = dynamic_cast<Veins::TraCIMobility *>(tmpMobility);
	cModule *tmpPhy = getParentModule()->getSubmodule("nic")->getSubmodule("phy");
	LtePhyUe* phy = dynamic_cast<LtePhyUe *>(tmpPhy);
	cModule *tmpCar = getParentModule();
	//Veins::HeterogeneousCar* car = dynamic_cast<Veins::HeterogeneousCar *>(tmpCar);
	//car->updateMasterId(phy->getMasterId());
	tmpCar->par("masterId") = (int)phy->getMasterId();
		//thierry
		//hijack code here to generate omnet++ coordinates from our csv

		if(simTime()<100 && !std::ifstream("/home/thierry/eggsomnet.csv")){
			cModule *tmpMobility = getParentModule()->getSubmodule("veinsmobility");
			Veins::TraCIMobility* mobility = dynamic_cast<Veins::TraCIMobility *>(tmpMobility);
			TraCIScenarioManager* mgr = mobility->getManager();
			
			std::ifstream theFile("/home/thierry/eggs.csv");


			std::ofstream outFile("/home/thierry/eggsomnet.csv");
	
			std::string x, y, site_name, system, commune, carrier, lat, lon, sumo_x, sumo_y, line;
			double coord_sumo_x;
			double coord_sumo_y;
			Coord convCoords;
			//getline(theFile, line, '\n');
			getline(theFile, line);
			for (int i = 0; i < 948; i++) {
			    getline(theFile, line);
			    std::istringstream line_stream;
			    line_stream.str(line);
			    //outFile << "src_line:"+line << endl;
			    getline(line_stream, x, ',');
			    outFile << x + ',';
			    getline(line_stream, y, ',');
			    outFile << y + ',';
			    getline(line_stream, site_name, ',');
			    outFile << site_name + ',';
			    getline(line_stream, system, ',');
			    outFile << system + ',';
			    getline(line_stream, commune, ',');
			    outFile << commune + ',';
			    getline(line_stream, carrier, ',');
			    outFile << carrier + ',';
			    getline(line_stream, lat, ',');
			    outFile << lat + ',';
			    getline(line_stream, lon, ',');
			    outFile << lon + ',';
			    getline(line_stream, sumo_x, ',');
			    outFile << sumo_x + ',';
			    getline(line_stream, sumo_y, ',');
			    sumo_y.erase(std::remove(sumo_y.begin(), sumo_y.end(), '\n'), sumo_y.end());
			    sumo_y.erase(std::remove(sumo_y.begin(), sumo_y.end(), '\r'), sumo_y.end());
			    outFile << sumo_y + ',';
			    coord_sumo_x = atof(sumo_x.c_str());
			    coord_sumo_y = atof(sumo_y.c_str());
			    convCoords = mgr->getOmnetCoords(coord_sumo_x,coord_sumo_y);
			    outFile << convCoords.x << ',';
			    outFile << convCoords.y << endl;
			}

			theFile.close();
			outFile.close();
			

		}
	*/	
	if (msg->isSelfMessage()) {
		
		//TODO Activate this block only for dwelltimestudy
		cModule *tmpMobility = getParentModule()->getSubmodule("veinsmobility");
		Veins::TraCIMobility* mobility = dynamic_cast<Veins::TraCIMobility*>(tmpMobility);
		Coord position =  mobility->getCurrentPosition();
		double curSpeed = mobility->getSpeed();
		std::string sumoId = mobility->getExternalId();

		LtePhyUe* phy = dynamic_cast<LtePhyUe*>(getParentModule()->getSubmodule("nic")->getSubmodule("phy"));
		//type, timestamp, macId, sumoId, x, y, speed, rssi, master
		std::cout << "_P_," << NOW << "," << getAncestorPar("macNodeId").longValue() << "," << sumoId << "," << position.x << "," << position.y << "," << curSpeed << "," << phy->getRSSI() << "," << getAncestorPar("masterId").longValue()<< "\n";
		
		/*		
		delete msg;//
		scheduleAt(simTime() + 10, new cMessage("Send"));
		return;//handovers only
		*/

		/*
		 * Send a message to a random node in the network. Note that only the most necessary values
		 * are set. Size of the message have to be set according to the real message (aka your used
		 * .msg file). The values here are just a temporary placeholder.
		 */
		HeterogeneousMessage *testMessage = new HeterogeneousMessage();
		//testMessage->setNetworkType(DONTCARE);
		testMessage->setNetworkType(DSRC);
		testMessage->setName("Heterogeneous Test Message");
		testMessage->setByteLength(10);

		/* choose a random other car to send the message to */
		TraCIScenarioManager* manager = TraCIScenarioManagerAccess().get();
		std::map<std::string, cModule*> hosts = manager->getManagedHosts();
		std::map<std::string, cModule*>::iterator it = hosts.begin();
		std::advance(it, intrand(hosts.size()));
		std::string destination("" + it->second->getFullPath() + "");
		//std::string destination("node[" + it->first + "]");
		std::cout << "[" << sumoId << ", " << simTime() <<  "] Sending message to " << destination << std::endl;
		testMessage->setDestinationAddress(destination.c_str());

		/* Finish the message and send it */

		
		testMessage->setSourceAddress((getParentModule()->getFullPath()).c_str());//sumoId.c_str());
		send(testMessage, toDecisionMaker);
		

		/*
		 * At 25% of the time send also a message to the main server. This message is sent via LTE
		 * and is then simply handed to the decision maker.
		 */
		if(dblrand() < 1){
			cModule *tmpPhy = getParentModule()->getSubmodule("nic")->getSubmodule("phy");
			LtePhyUe* phy = dynamic_cast<LtePhyUe *>(tmpPhy);
			std::cout << "[" << sumoId << ", " << simTime() <<  "] Sending message to server through " << (int)(phy->getMasterId()) << std::endl;
			HeterogeneousMessage* serverMessage = new HeterogeneousMessage();
			serverMessage->setName("Server Message Test");
			testMessage->setByteLength(10);
			serverMessage->setNetworkType(LTE);
			/*std::map<std::string, cModule*>::iterator it2 = hosts.begin();
			std::advance(it2, intrand(hosts.size()));
			std::string destination2("" + it2->second->getFullPath() + "");*/
			serverMessage->setDestinationAddress("server");
			//serverMessage->setDestinationAddress(destination2.c_str());
			serverMessage->setSourceAddress((getParentModule()->getFullPath()).c_str());//sumoId.c_str());
			//serverMessage->setSourceAddress(sumoId.c_str());
			send(serverMessage, toDecisionMaker);
		}

		scheduleAt(simTime() + 1, new cMessage("Send"));
	} else {
		HeterogeneousMessage *testMessage = dynamic_cast<HeterogeneousMessage *>(msg);
		std::cout << "[" << getParentModule()->getFullPath() << ", " << simTime() << "] Received message " << msg->getFullPath() << "< from " << testMessage->getSourceAddress() << std::endl;
	}
	delete msg;
}
