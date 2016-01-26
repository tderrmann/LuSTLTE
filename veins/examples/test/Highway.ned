//
// Copyright (C) 2008 Christoph Sommer <christoph.sommer@informatik.uni-erlangen.de>
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


import org.car2x.veins.base.connectionManager.ConnectionManager;
import org.car2x.veins.base.modules.BaseWorldUtility;
import org.car2x.veins.modules.mobility.traci.TraCIScenarioManagerLaunchd;
import org.car2x.veins.modules.obstacle.ObstacleControl;
import org.car2x.veins.modules.world.annotations.AnnotationManager;

import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import inet.nodes.inet.AdhocHost;
import inet.world.radio.ChannelControl;
import inet.nodes.inet.Router;
import inet.nodes.inet.StandardHost;
import inet.nodes.ethernet.Eth10G;
import inet.networklayer.ipv4.RoutingTableRecorder;

import lte.corenetwork.binder.*;
import lte.corenetwork.nodes.*;

module Highway
{
    parameters:
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        @display("bgb=732,483");
        int numUe = default(100);

    submodules:
        obstacles: ObstacleControl {
            @display("p=261,35");
        }
        annotations: AnnotationManager {
            @display("p=485,35");
        }
        connectionManager: ConnectionManager {
            parameters:
                @display("p=146,31;i=abstract/multicast");
        }
        world: BaseWorldUtility {
            parameters:
                playgroundSizeX = playgroundSizeX;
                playgroundSizeY = playgroundSizeY;
                playgroundSizeZ = playgroundSizeZ;
                @display("p=34,31;i=misc/globe");
        }
        manager: TraCIScenarioManagerLaunchd {
            parameters:
                @display("p=34,206");
        }

        // needed for INET
        channelControl: ChannelControl {
            @display("p=361,34");
        }
        configurator: IPv4NetworkConfigurator {
            @display("p=34,97");
        }
        // LTE stuff
        binder: LteBinder {
            @display("p=34,152;is=s");
        }
        eNodeB1: eNodeB {
            @display("p=200,200;is=vl");
        }
        eNodeB2: eNodeB {
            @display("p=100,100;is=vl");
        }
        eNodeB3: eNodeB {
            @display("p=320,320;is=vl");
        }
        server: StandardHost {
            @display("p=146,152;is=n;i=device/server");
        }
        routingRecorder: RoutingTableRecorder {
            @display("p=34,274;is=s");
        }
    connections allowunconnected:
        server.pppg++ <--> Eth10G <--> eNodeB1.ppp;
}

