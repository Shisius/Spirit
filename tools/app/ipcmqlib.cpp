
#include "ipcmqlib.h"

namespace spirit
{

	ssize_t mqget_msgsize(const mqd_t mq_id)
	{
		struct mq_attr attr;
		if (mq_getattr(mq_id, &attr) < 0)
		{
		    return -1;
		}
		return attr.mq_msgsize;
	}

	int mqsend_spmsg(const SpiritMsg & msg, mqd_t mq_id, ssize_t max_size, unsigned int msg_prio = 0)
	{
		// Check size
		ssize_t packsize = sizeof(SpiritMsg) + msg.size - sizeof(msg.data);
		if (packsize > max_size) return MQSIZE_ERROR;
		// Check data
		if (malloc_usable_size(msg.data) < msg.size) return MQDATA_ERROR;
		// Pack
		char buf[packsize];
		memcpy(buf, msg, sizeof(SpiritMsg) - sizeof(msg.data));
		memcpy(buf + sizeof(SpiritMsg) - sizeof(msg.data), msg.data, msg.size);
		// Send
		if (mq_send(mq_id, buf, packsize, msg_prio) < 0) return MQLIB_ERROR;
		return 0;
	}

	int mqrecv_spmsg(SpiritMsg & msg, mqd_t mq_id, ssize_t max_size, unsigned int * msg_prio)
	{
		if (max_size <= 0) return MQLIB_ERROR;
		// Receive
		char buf[max_size];
		int result = mq_receive(mq_id, buf, max_size, msg_prio);
		if (result < 0) return MQLIB_ERROR;
		// Parse
		memcpy(&msg, buf, sizeof(SpiritMsg) - sizeof(msg.data));
		msg.data = nullptr;
		if (msg.size < 0) return MQSIZE_ERROR;
		if (result < (sizeof(SpiritMsg) + msg.size - sizeof(msg.data))) return MQSIZE_ERROR;
		if (msg.size > 0) {
			msg.data = ::operator new(msg.size);
			if (msg.data == nullptr) return MQDATA_ERROR;
			memcpy(msg.data, buf + sizeof(SpiritMsg) - sizeof(msg.data), msg.size);
		} 
		return 0;
	}

	int onetime_mqsend(const std::string & send_name, const SpiritMsg & msg, unsigned int msg_prio)
	{
		int result;
		// Open
		int oflag = O_RDWR | O_NONBLOCK;
		mqd_t mqsend_d = mq_open(send_name.c_str(), oflag);
		if (mqsend_d == static_cast<mqd_t>(-1)) {
			printf("MQ open error: %s\n", send_name.c_str());
			return -1;
		}
		// Send
		ssize_t max_size = mqget_msgsize(mqsend_d);
		result = mqsend_spmsg(msg, mqsend_d, max_size, msg_prio);
		if (result < 0) printf("MQ send error %d: %s\n", result, send_name.c_str());
		// Close
		if (mq_close(mqsend_d) < 0) {
			printf("MQ close error: %s\n", send_name.c_str());
			return -1;
		}
		return result;
	}

	SpiritMsg onetime_mqreq(const std::string & req_name, const std::string & ans_name, const SpiritMsg & msg, unsigned int timeout_us)
	{
		
	}

} //spirit
