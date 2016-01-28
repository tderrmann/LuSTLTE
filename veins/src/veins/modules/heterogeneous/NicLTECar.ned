// @author: Florian Hagenauer
// @date: 25.06.2014
// HeterogeneousCar
//
// This car module provides two interfaces for sending and receiving messages via LTE and/or DSRC.

package org.car2x.veins.modules.heterogeneous;

import org.car2x.veins.base.modules.*;

import org.car2x.veins.modules.lte.*;
import org.car2x.veins.modules.heterogeneous.application.*;
import lte.corenetwork.nodes.*;

import inet.nodes.inet.NetworkLayerNoConfig;
import lte.stack.phy.LteNicUe;
import inet.transport.udp.UDP;
import inet.transport.tcp.TCP;
import inet.applications.ITCPApp;

import inet.networklayer.ipv4.RoutingTable;
import inet.mobility.contract.IMobility;
import inet.base.NotificationBoard;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.autorouting.ipv4.HostAutoConfigurator;

module NicLTECar
{
    parameters:
        @node();

        string veinsmobilityType;  //type of the mobility module
        string mobilityType;       // INET mobility

        // Following Parameters taken from Ue.ned to make LTE work
        string nodeType = "UE";  // DO NOT CHANGE
        volatile int masterId = default(1);
        int macNodeId = default(0); // TODO: this is not a real parameter
        int macCellId = default(0); // TODO: this is not a real parameter

        @display("bgb=226,450");

    gates:
        input radioIn @directIn; //LTE input gate

    submodules:
        interfaceTable: InterfaceTable {
            @display("p=57,245;is=s");
        }

        mobility: <mobilityType> like IMobility {
            @display("p=57,345;is=s");
        }

        notificationBoard: NotificationBoard {
            @display("p=57,195;is=s");
        }

        routingTable: RoutingTable {
            @display("p=57,295;is=s");
        }

        application: SimpleApp {
            @display("p=161,31;i=block/app2");
        }

        veinsmobility: <veinsmobilityType> like org.car2x.veins.base.modules.IMobility {
            parameters:
                @display("p=57,31;i=block/cogwheel");
        }

        veinsToLTE: HeterogeneousToLTE {
            parameters:
                @display("p=161,124;i=abstract/accesspoint");
        }

        networkLayer: NetworkLayerNoConfig {
            @display("p=161,301");
        }

        // NOTE: instance must be named "nic", therefore the other nic is called nic80211p
        nic: LteNicUe {
            nodeType = nodeType;
            @display("p=161,392");
        }

        UDP: UDP {
            @display("p=161,212");
        }

        configurator: HostAutoConfigurator {
            @display("p=57,109");
        }

    connections allowunconnected:

        application.toDecisionMaker --> veinsToLTE.fromApplication;
        veinsToLTE.toApplication --> application.fromDecisionMaker;

        veinsToLTE.toLTE --> UDP.appIn++;
        UDP.ipOut --> networkLayer.transportIn++;

        veinsToLTE.fromLTE <-- UDP.appOut++;
        UDP.ipIn <-- networkLayer.transportOut++;

        networkLayer.ifOut++ --> nic.upperLayerIn;
        networkLayer.ifIn++ <-- nic.upperLayerOut;

        radioIn --> nic.radioIn;
}
