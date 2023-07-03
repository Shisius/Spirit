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
	SpiritNote d_note;

	std::unique_ptr<MqReceiver> d_receiver;

	SpiritMsg d_cur_ans;
	SpiritMsg d_cur_req;

	template<class T>
	void answer_custom(unsigned char title, T & data);
	{
		// Create obj here
	}
	template<class T>
	void answer_done(unsigned char title, T & data);

	void answer_accepted(unsigned char title, unsigned int timeout_us = 0);
	void answer_denied(unsigned char title);
	void answer_done(unsigned char title);

	/**
	 * Get data from last request. Use mutex if T& is your internal storage with multiple access.
	 */
	template<class T>
	int get_req_data(T & data);

	/**
	 * MqReceiver handler. Use answer_* to make answer on custom title.
	 * Should use mutex to protect your data.
	 */
	int answer(SpiritMsg & msg)
	{
		// Check if it is req. Check role
		bool isans = static_cast<bool>(spirit_msg_is_ans(&msg));
		unsigned char sender = msg.sender;
		int result = 0;
		d_cur_req = msg;
		if (isans && (msg.receiver == get_role()))
		{
			switch (msg.title)
			{
				case SPIRIT_TITLE_STATE:
					SpiritState ss = get_state();
					answer_custom(msg.title, ss);
					break;
				default:
					result = handler(title, msg.size);
			}
			if (result < 0) {
				answer_denied(msg.title);
			} 
			// d_ans -> msg
			return 0;
		}
		return -1; // No answer or denied?
	}

	void set_state(unsigned char bit, bool value)
	{
		SpiritState ss = d_state.load();

	}

	virtual int setup();
	virtual void destroy();
	virtual void mainloop();
	/**
	 * Your part of logic. You can get data here if size > 0;
	 */
	virtual int handler(unsigned char title, unsigned int size);

public:

	SpiritBase();
	~SpiritBase();

	void run()
	{
		while (spirit_state_get(d_state.load().system, SPIRIT_STATE_ALIVE) != 0)
		{
			mainloop();
		}
	}

	int setup_sp(const SpiritNote & sp_note)
	{
		d_note = sp_note;
		std::string sp_name(d_note.name);
		d_receiver = std::make_unique<MqReceiver>(std::string(d_name), &SpiritBase::answer, this);

		int result = setup();

		d_receiver->setup();
		// Set ALIVE
	}

	void destroy_sp()
	{
		destroy();
	}

	virtual unsigned char get_role();


	SpiritState get_state() {return d_state.load();}

};

#endif //_SPIRIT_BASE_H_
