#LuSTLTE

LuSTLTE combines the LuST scenario and VeinsLTE, which in turn uses SimuLTE on OMNet++ and SUMO.
LuSTLTE includes eNodeBs for a Luxembourg LTE provider along with a simple implementation of handovers.
The purpose of this project is to enable simulations of heterogeneous vehicular networks in a realistic setting

##TODOs:
* Fix communication after a handover! Currently, grants enable 0 Bytes of transmission...
* Include a scenario for VehiLux (covers all of Luxembourg for a half-day).

We have tested this version with OMNeT++ v. 4.4.1 and SUMO 0.25.0. Please refer to the VeinsLTE instructions for installation (see below).

## Veins LTE

Veins LTE adds LTE support to the vehicular network simulation framework [Veins](http://veins.car2x.org/).

### Prerequisites

* Install [OMNeT++](http://www.omnetpp.org/).
* Install [SUMO](http://www.dlr.de/ts/en/desktopdefault.aspx/tabid-9883/16931_read-41000/).
* Clone this repository (or download the zip file).
* If you are on Linux:
  * switch to the root of the repository and type
  * `make makefiles`
  * `make`
  * go to the veins subdirectory and start SUMO with sumo-launchd.py. For example `./sumo-launchd.py -p 9999 -vv -c sumo-gui`
  * go to the folder veins/examples/heterogeneous and run the example.
* If you are on Windows:
  * `TODO` *(Currently there seem to be several issues with Veins LTE on Windows, we are working on them)*

Further information can be found on the [Veins LTE Website](http://floxyz.at/veins-lte).
