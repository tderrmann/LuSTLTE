#LuSTLTE
* LuSTLTE combines the LuST scenario and VeinsLTE, which in turn is based on SimuLTE for OMNeT++ and SUMO.
* LuSTLTE includes eNodeB locations for a single Luxembourg LTE carrier along with a simple handover mechanism implementation.
* The purpose of this fork of VeinsLTE is to provide a ready-to-use package for evaluating services on LTE and DSRC/802.11p in a more or less realistic setting

##Remarks:
* This version was tested w/ OMNeT++ 4.4.1 and SUMO 0.25.0; Please refer to the instructions of VeinsLTE installation below.
* Communication after handovers is working more or less, i.e. there is still some trouble while UEs are out of range of eNBs, so we have to set the usable band manually in the LteAmc::attachUser. Also, 0 byte grants are created, but deleted before being sent within LteMacEnb::sendGrants. These are workarounds that will be fixed in future versions.

##TODOs:
* Update eNB positions and transmission power levels


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
