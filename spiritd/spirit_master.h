#ifndef _SPIRIT_MASTER_H_
#define _SPIRIT_MASTER_H_

#include "spirit_master_tools.h"

class SpiritMaster
{

protected:

	std::vector<SpiritSystemUnit> d_spirit_units;
	MqReceiver d_receiver;

	

public:

	SpiritMaster();
	~SpiritMaster();

	int start();
	int stop();
	int restart();

};

#endif //_SPIRIT_MASTER_H_
