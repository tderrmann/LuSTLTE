#include "BasicDecisionMaker.h"

class ExampleDecisionMaker: public BasicDecisionMaker{
	public:
		ExampleDecisionMaker();
		~ExampleDecisionMaker();

	protected:
		virtual void sendLteMessage(HeterogeneousMessage* msg);
        virtual void sendDSRCMessage(HeterogeneousMessage* msg);
        virtual void sendDontCareMessage(HeterogeneousMessage* msg);
};
