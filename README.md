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
