cplusplus {{
#include "veins/modules/messages/WaveShortMessage_m.h"
#define DONTCARE 0
#define LTE 1
#define DSRC 2

#define CAM_TYPE 42
#define CAM_ERROR_TYPE 43
#define BEACON_TYPE 44
}}

class WaveShortMessage;

// The basic message for the heterogeneous network - it is used by the decision maker.

message HeterogeneousMessage extends WaveShortMessage {
	string sourceAddress;
	string destinationAddress;
	int networkType;
	simtime_t sendingTime;
}
