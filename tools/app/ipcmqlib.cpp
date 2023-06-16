
#include "ipcmqlib.h"

namespace spirit
{

	ssize_t mqget_msgsize(mqd_t mq_id)
	{
		struct mq_attr attr;
		if (mq_getattr(mq_id, &attr) < 0)
		{
		    return -1;
		}
		return attr.mq_msgsize;
	}

	int mqsend_spmsg(SpiritMsg & msg, mqd_t mq_id, ssize_t max_size, unsigned int msg_prio)
	{
		// Check size
		ssize_t packsize = sizeof(SpiritMsg) + msg.size - sizeof(msg.data);
		if (packsize > max_size) return MQSIZE_ERROR;
		// Check data
		if (msg.size > 0) {
			if (malloc_usable_size(msg.data) < msg.size) return MQDATA_ERROR;
		}
		// Pack
		char buf[packsize];
		memcpy(buf, &msg, sizeof(SpiritMsg) - sizeof(msg.data));
		if (msg.size > 0)
			memcpy(buf + sizeof(SpiritMsg) - sizeof(msg.data), msg.data, msg.size);
		// Send
		if (mq_send(mq_id, buf, packsize, msg_prio) < 0) return MQLIB_ERROR;
		// Free msg data
		if (msg.data != nullptr)
			delete msg.data;
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
		if (static_cast<size_t>(result) < (sizeof(SpiritMsg) + msg.size - sizeof(msg.data))) return MQSIZE_ERROR;
		if (msg.size > 0) {
			msg.data = ::operator new(msg.size);
			if (msg.data == nullptr) return MQDATA_ERROR;
			memcpy(msg.data, buf + sizeof(SpiritMsg) - sizeof(msg.data), msg.size);
		} 
		return 0;
	}

	int onetime_mqsend(const std::string & send_name, SpiritMsg & msg, unsigned int msg_prio)
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

	int onetime_mqreq(const std::string & req_name, const std::string & ans_name, SpiritMsg & msg, unsigned int timeout_us)
	{
		// Create mq

		// Send req

		// Get answer

		// Close mq

		return 0;
	}

	mqd_t mq_create_default(const std::string & name)
	{
		mode_t perm =  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; //7777; 

		int oflag = O_RDWR | O_NONBLOCK | O_CREAT | O_EXCL;

		return mq_open(name.c_str(), oflag, perm, nullptr/*d_pmqAttr*/);
	}

} //spirit

int MqReceiver::setup()
{
	// Create mq
	d_mqid = spirit::mq_create_default(d_mqname);
	if (d_mqid == static_cast<mqd_t>(-1)) {
		printf("MQ create error %s\n", d_mqname.c_str());
		return spirit::MQLIB_ERROR;
	}
	// Get attr
	d_maxsize = spirit::mqget_msgsize(d_mqid);
	// Notify
	notify_setup(this);
	if (d_error.load() != 0) return d_error.load();
    // Start thread
    d_isalive.store(true);
	d_recv_thread = std::thread(&MqReceiver::recv_loop, this);
	return 0;
}

void MqReceiver::notify_setup(MqReceiver * _mqrecevier)
{
	struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD; /* Notify via thread */
    sev.sigev_notify_function = &MqReceiver::mq_event;
    sev.sigev_notify_attributes = nullptr; /* Could be pointer to pthread_attr_t structure */
    sev.sigev_value.sival_ptr = _mqrecevier; /* Argument to threadFunc() */

    if (mq_notify(_mqrecevier->d_mqid, &sev) < 0)
    {
        printf("MQ notify error %s\n", _mqrecevier->d_mqname.c_str());
        _mqrecevier->d_error.store(spirit::MQLIB_ERROR);
        return;
    }
}

void MqReceiver::mq_event(union sigval sv)
{
	MqReceiver * mq_recever_ptr = (MqReceiver *)(sv.sival_ptr);
	lock_guard<mutex> lk(mq_recever_ptr->d_mut);
	notify_setup(mq_recever_ptr);
	mq_recever_ptr->d_cond.notify_one();
	pthread_exit(nullptr);
}

void MqReceiver::recv_loop()
{
	SpiritMsg spmsg;
	int result;
	while (d_isalive.load()) {
		lock_guard<mutex> lk(d_mut);
		// Wait for event

		while ( result >= 0 )
		{
			result = mqrecv_spmsg(spmsg, d_mqid, d_maxsize, nullptr);
			if (result >= 0) {

			}
		}
	}
	if (spmsg.data != nullptr)
		delete spmsg.data;
}

MqReceiver::~MqReceiver()
{
	d_isalive.store(false);
	if (d_recv_thread.joinable())
		d_recv_thread.join();
	// Close mq
	if (mq_close(d_mqid) < 0)
		printf("MQ close error %s\n", d_mqname.c_str());
	if (mq_unlink(d_mqname) < 0)
		printf("MQ unlink error %s\n", d_mqname.c_str());;	
}
