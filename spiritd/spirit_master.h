#ifndef _SPIRIT_MASTER_H_
#define _SPIRIT_MASTER_H_

#include "spirit_master_tools.h"
#include "spirit_master_defs.h"
#include "daemon_origin.h"

class SpiritMaster : public SpiritBase
{

protected:

	std::vector<SpiritSystemUnit> d_spirit_units;

	MqReceiver d_receiver;

	std::thread d_check_thread;
	void check_process();

	int check_spirits();
	SpiritMsg msg_handler(const SpiritMsg&);

public:

	SpiritMaster();
	~SpiritMaster();

	/**
	 * On setup: load spirit unit files
	 */
	int setup();

	int start();
	int stop();
	int restart();

};

#endif //_SPIRIT_MASTER_H_
