#ifndef _IPCMQLIB_H_
#define _IPCMQLIB_H_

#include <fcntl.h>    /* Defines O_* constants */
#include <sys/stat.h> /* Defines mode constants */
#include <mqueue.h>
#include <pthread.h>
#include <signal.h>
#include <string>
#include <functional>

#include "spirit_msg.h"

namespace spirit {

	/**
	 * onetime_mqreq
	 * Creates ans mq, opens req mq, sends msg, waits for answer, returned none_msg if no answer on timeout
	 */
	SpiritMsg onetime_mqreq(const std::string & req_name, const std::string & ans_name, const SpiritMsg & msg, unsigned int timeout_us);

	/**
	 * onetime_mqsend
	 * Opens mq, sends msg, closes mq, returnes 0 on success
	 */
	int onetime_mqsend(const std::string & send_name, const SpiritMsg & msg, unsigned int msg_prio = 0);

} //spirit

class MqReceiver
{

protected:

	std::string d_mqname;
	std::function<SpiritMsg(const SpiritMsg&)> d_msg_handler;

public:

	template< class _MsgHandler, class _Master>
	MqReceiver(const std::string & mqname, _MsgHandler&& msg_handler, _Master&& master_ptr) : d_mqname(mqname)
	{
		d_msg_handler = std::bind(msg_handler, master_ptr, std::placeholders::_1);
	}
	~MqReceiver();

};


#endif //_IPCMQLIB_H_
