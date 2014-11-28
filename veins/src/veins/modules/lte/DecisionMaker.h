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

#ifndef DECISIONMAKER_H
#define DECISIONMAKER_H

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "veins/modules/heterogeneous/messages/HeterogeneousMessage_m.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "UDPControlInfo_m.h"
#include "veins/modules/world/annotations/AnnotationManager.h"
#include "MobilityBase.h"

using Veins::AnnotationManager;
using Veins::AnnotationManagerAccess;

#define PRINT_DM(x) std::cout << "[DecisionMaker, " << simTime() << "] " << x << std::endl;
#define WARN_DM(x) std::cout << "[DecisionMaker, " << simTime() << " WARNING] " << x << std::endl;
#define AFTERWARMUP(x) if (simTime() >= simulation.getWarmupPeriod()){ x }

/**
 * @brief
 * The decision maker layer for the heterogeneous stack in a car.
 *
 * It either sends the message via LTE or DSRC.
 * Additionally the user can specify DONTCARE in which case the decision maker will decide 
 * how to send.
 *
 * @author Florian Hagenauer
 *
 */

class DecisionMaker: public BaseWaveApplLayer {

    protected:
        /** Various gates for the communication. */
        int fromApplication;
        int toApplication;
        int fromLte;
        int toLte;
        int fromDSRC;
        int toDSRC;
        std::string id;  /** The id of the car. */

        // statistic collection
        simsignal_t DSRCMessagesSent;
        simsignal_t lteMessagesSent;
        simsignal_t dontCareMessagesSent;
        simsignal_t DSRCMessagesReceived;
        simsignal_t lteMessagesReceived;

    public:
        DecisionMaker();
        virtual ~DecisionMaker();
        Coord getPosition() {
            return curPosition;
        }

    protected:
        virtual int numInitStages() const {
            return std::max(4, cSimpleModule::numInitStages());
        }

        void initialize(int stage) {
            if (stage == 0) {
            	DSRCMessagesSent = registerSignal("DSRCMessagesSent");
                lteMessagesSent = registerSignal("lteMessagesSent");
                dontCareMessagesSent = registerSignal("dontCareMessagesSent");
                DSRCMessagesReceived = registerSignal("DSRCMessagesReceived");
                lteMessagesReceived = registerSignal("lteMessagesReceived");
            }
            BaseWaveApplLayer::initialize(stage);
            fromApplication = findGate("fromApplication");
            toApplication = findGate("toApplication");
            fromLte = findGate("fromLte");
            toLte = findGate("toLte");
            fromDSRC = findGate("lowerLayerIn");
            toDSRC = findGate("lowerLayerOut");
            cModule *tmpMobility = getParentModule()->getSubmodule("veinsmobility");
            Veins::TraCIMobility* mobility = dynamic_cast<Veins::TraCIMobility *>(tmpMobility);
            if (mobility) {
                id = mobility->getExternalId();
            } else {
                std::cout << "ERROR: No mobility found!" << endl;
            }

        }

        void finish() {
        }

        void handleLowerMessage(cMessage* msg) {
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
                MobilityBase* eNodeBMobility = dynamic_cast<MobilityBase*>(getModuleByPath(
                        "scenario.eNodeB1")->getSubmodule("mobility"));
                ASSERT(eNodeBMobility);
                AnnotationManager* annotations = AnnotationManagerAccess().getIfExists();
                annotations->scheduleErase(0.25,
                       annotations->drawLine(eNodeBMobility->getCurrentPosition(), getPosition(),
                               "green"));
                send(msg, toApplication);
            } else if (arrivalGate == fromDSRC) {
            	emit(DSRCMessagesReceived, 1);
                send(msg, toApplication);
            } else {
                std::cout << "Unknown arrival gate " << msg->getArrivalGate()->getFullName()
                        << std::endl;
                delete msg;
            }
        }

        void handleMessage(cMessage* msg) {
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

        void onBeacon(WaveShortMessage *wsm) {
            delete wsm;
        }
        void onData(WaveShortMessage *wsm) {
            delete wsm;
        }

        virtual void sendLteMessage(HeterogeneousMessage* msg);
        virtual void sendDSRCMessage(HeterogeneousMessage* msg);
        virtual void sendDontCareMessage(HeterogeneousMessage* msg);
};

#endif /* DECISIONMAKER_H */
