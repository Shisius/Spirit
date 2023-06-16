#ifndef _IPCMQLIB_H_
#define _IPCMQLIB_H_

#include <fcntl.h>    /* Defines O_* constants */
#include <sys/stat.h> /* Defines mode constants */
#include <mqueue.h>
#include <pthread.h>
#include <signal.h>
#include <string>
#include <functional>
#include <condition_variable>
#include <atomic>
#include <cstring>
#include <malloc.h>

#include "spirit_msg.h"

namespace spirit {

	enum IpcMqResult {
		MQDATA_ERROR = -3,	// Wrong spirit_msg.data pointer
		MQSIZE_ERROR = -2,	// SpiritMsg pack size > max mq size
		MQLIB_ERROR = -1,	// -1 returned after mq call
		MQ_SUCCESS = 0
	};

	/**
	 * Base functions - send and recv SpiritMsg by MQ
	 */
	ssize_t mqget_msgsize(mqd_t mq_id);
	int mqsend_spmsg(SpiritMsg & msg, mqd_t mq_id, ssize_t max_size, unsigned int msg_prio = 0);
	int mqrecv_spmsg(SpiritMsg & msg, mqd_t mq_id, ssize_t max_size, unsigned int * msg_prio = nullptr);
	mqd_t mq_create_default(const std::string & name);
	// void mq_delete_default(con)

	/**
	 * onetime_mqreq
	 * Creates ans mq, opens req mq, sends msg, waits for answer, ans in msg, returned 0 on success
	 */
	int onetime_mqreq(const std::string & req_name, const std::string & ans_name, SpiritMsg & msg, unsigned int timeout_us);

	/**
	 * onetime_mqsend
	 * Opens mq, sends msg, closes mq, returnes 0 on success. Deletes msg.data.
	 */
	int onetime_mqsend(const std::string & send_name, SpiritMsg & msg, unsigned int msg_prio = 0);

} //spirit

class MqReceiver
{

protected:

	std::string d_mqname;
	mqd_t d_mqid;
	ssize_t d_maxsize;
	std::function<int(SpiritMsg&)> d_msg_handler;

	std::condition_variable d_cond;
	std::mutex d_mut;
	std::thread d_recv_thread;
	std::atomic<bool> d_isalive;
	std::atomic<int> d_error; 
	//std::function<void()> d_notify_setup();
	static void notify_setup(MqReceiver*);
	static void mq_event(union sigval sv);

	void recv_loop();

public:

	int setup();

	int get_error() {return d_error.load();}

	template< class _MsgHandler, class _Master>
	MqReceiver(const std::string & mqname, _MsgHandler&& msg_handler, _Master&& master_ptr) : d_mqname(mqname)
	{
		d_msg_handler = std::bind(msg_handler, master_ptr, std::placeholders::_1);
		d_error.store(0);
	}
	~MqReceiver();

};


#endif //_IPCMQLIB_H_
