#ifndef _SPIRIT_BASE_H_
#define _SPIRIT_BASE_H_

#include "spirit_base_tools.h"
#include <string>
#include <atomic>
#include <memory>

class SpiritBase
{
protected:

	std::atomic<SpiritState> d_state;

	std::unique_ptr<MqReceiver> d_receiver;

	int answer(SpiritMsg & msg);

public:

	SpiritBase();
	~SpiritBase();

	void run()
	{
		while (spirit_state_get(d_spirit.get_state().system, SPIRIT_STATE_ALIVE) != 0)
		{
			mainloop();
		}
	}

	virtual int setup(const SpiritNote & sp_note);
	virtual void destroy();
	virtual void mainloop();

	virtual unsigned char get_role();


	SpiritState get_state() {return d_state.load();}

};

#endif //_SPIRIT_BASE_H_
