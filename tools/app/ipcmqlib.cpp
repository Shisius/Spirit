
#include "ipcmqlib.h"

namespace spirit
{

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
		result = mq_send(mqsend_d, &msg, sizeof(msg), msg_prio);
		if (result < 0) printf("MQ send error: %s\n", send_name.c_str());
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