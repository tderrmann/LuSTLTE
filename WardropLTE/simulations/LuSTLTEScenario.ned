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

package wardroplte.simulations;

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

module LuSTLTEScenario
{
    parameters:
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        @display("bgb=14000,12000");
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

        //TANGO LUXEMBOURG LTE

        eNodeB1: eNodeB {
            @display("p=1517,4336;is=vl");
        }
        eNodeB2: eNodeB {
            @display("p=2909,4744;is=vl");
        }
        eNodeB3: eNodeB {
            @display("p=8107,7179;is=vl");
        }
        eNodeB4: eNodeB {
            @display("p=8906,6485;is=vl");
        }
        eNodeB5: eNodeB {
            @display("p=8982,8575;is=vl");
        }
        eNodeB6: eNodeB {
            @display("p=9719,7438;is=vl");
        }
        eNodeB7: eNodeB {
            @display("p=4377,4729;is=vl");
        }
        eNodeB8: eNodeB {
            @display("p=5367,3893;is=vl");
        }
        eNodeB9: eNodeB {
            @display("p=6078,3059;is=vl");
        }
        eNodeB10: eNodeB {
            @display("p=6256,5646;is=vl");
        }
        eNodeB11: eNodeB {
            @display("p=6472,4505;is=vl");
        }
        eNodeB12: eNodeB {
            @display("p=6801,3758;is=vl");
        }
        eNodeB13: eNodeB {
            @display("p=6811,5847;is=vl");
        }
        eNodeB14: eNodeB {
            @display("p=7025,2966;is=vl");
        }
        eNodeB15: eNodeB {
            @display("p=7059,4272;is=vl");
        }
        eNodeB16: eNodeB {
            @display("p=7067,5384;is=vl");
        }
        eNodeB17: eNodeB {
            @display("p=7044,7941;is=vl");
        }
        eNodeB18: eNodeB {
            @display("p=7321,4451;is=vl");
        }
        eNodeB19: eNodeB {
            @display("p=7259,6329;is=vl");
        }
        eNodeB20: eNodeB {
            @display("p=7362,5747;is=vl");
        }
        eNodeB21: eNodeB {
            @display("p=7503,5278;is=vl");
        }
        eNodeB22: eNodeB {
            @display("p=7492,6091;is=vl");
        }
        eNodeB23: eNodeB {
            @display("p=7631,3244;is=vl");
        }
        eNodeB24: eNodeB {
            @display("p=7576,5566;is=vl");
        }
        eNodeB25: eNodeB {
            @display("p=8291,3761;is=vl");
        }
        eNodeB26: eNodeB {
            @display("p=8428,4622;is=vl");
        }
        eNodeB27: eNodeB {
            @display("p=9664,3709;is=vl");
        }
        eNodeB28: eNodeB {
            @display("p=9893,2281;is=vl");
        }
        eNodeB29: eNodeB {
            @display("p=10164,2311;is=vl");
        }
        eNodeB30: eNodeB {
            @display("p=10459,1871;is=vl");
        }
        eNodeB31: eNodeB {
            @display("p=10677,2445;is=vl");
        }
        eNodeB32: eNodeB {
			@display("p=-338,1030;is=vl");
		}
        eNodeB33: eNodeB {
            @display("p=13089,2254;is=vl");
        }
        eNodeB34: eNodeB {
			@display("p=6294,11878;is=vl");
		}
        eNodeB35: eNodeB {
            @display("p=14479,4099;is=vl");
        }
        eNodeB36: eNodeB {
            @display("p=1716,2857;is=vl");
        }
        eNodeB37: eNodeB {
        	@display("p=8864,-691;is=vl");
        }
        eNodeB38: eNodeB {
        	@display("p=10147,11851;is=vl");
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

