
#include "ipcmqlib.h"

namespace spirit
{

	ssize_t mqget_msgsize(mqd_t mq_id)
	{
		struct mq_attr attr;
		if (mq_getattr(mq_id, &attr) < 0)
		{
		    return MQLIB_ERROR;
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
		if (mq_send(mq_id, buf, packsize, msg_prio) < 0) {
			if (result == EAGAIN) return MQ_FULL
			else return MQLIB_ERROR;
		}
		// Free msg data
		if (msg.data != nullptr)
			free(msg.data);
		return 0;
	}

	int mqrecv_spmsg(SpiritMsg & msg, mqd_t mq_id, ssize_t max_size, unsigned int * msg_prio, unsigned int timeout_us)
	{
		if (max_size <= 0) return MQLIB_ERROR;
		// Receive
		char buf[max_size];
		int result = 0;
		timespec ts;
		if (timeout_us == 0) {
			result = mq_receive(mq_id, buf, max_size, msg_prio);
		} else {
			clock_gettime(CLOCK_MONOTONIC, &ts);
			ts.tv_sec += timeout_us / 1000000;
			ts.tv_nsec += (timeout_us % 1000000) * 1000;
			if (ts.tv_nsec > 1000000000) {
				ts.tv_sec++;
				ts.tv_nsec -= 1000000000;
			} 
			result = mq_timedreceive(mq_id, buf, max_size, msg_prio, &ts);
		}
		if (result < 0) {
			if (result == EAGAIN || result == ETIMEDOUT) return MQ_EMPTY
			else return MQLIB_ERROR;
		}
		// Parse
		memcpy(&msg, buf, sizeof(SpiritMsg) - sizeof(msg.data));
		msg.data = nullptr;
		if (msg.size < 0) return MQSIZE_ERROR;
		if (static_cast<size_t>(result) < (sizeof(SpiritMsg) + msg.size - sizeof(msg.data))) return MQSIZE_ERROR;
		if (msg.size > 0) {
			msg.data = malloc(msg.size);
			if (msg.data == nullptr) return MQDATA_ERROR;
			memcpy(msg.data, buf + sizeof(SpiritMsg) - sizeof(msg.data), msg.size);
		} 
		return 0;
	}

	std::string make_ansmqname(const std::string & name, unsigned char role)
	{
		return name + std::string(spirit_role2str(role));
	}

	SpiritNote get_note(unsigned char role_id)
	{
		SpiritNote note = create_spirit_null_note();
		switch (role_id)
		{
			case SPIRIT_MASTER_ROLE:
				note = get_spirit_master_note();
				break;
			default:
				// Ask master for the note
				;
		}
		return note;
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
		int result
		// Create mq
		std::string ansname = make_ansmqname(req_name, msg.sender);
		mode_t perm =  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; //7777; 
		int oflag = O_RDONLY | O_CREAT | O_EXCL;
		mqd_t mqid = mq_open(ansname.c_str(), oflag, perm, nullptr/*d_pmqAttr*/);
		if (mqid == static_cast<mqd_t>(-1)) {
			printf("MQ create error: %s\n", ansname.c_str());
			return MQLIB_ERROR;
		}
		ssize_t maxsize = spirit::mqget_msgsize(mqid);
		if (maxsize < 0) return MQLIB_ERROR;
		// Send req
		result = onetime_mqsend(req_name, msg);
		if (result >= 0) {
			// Get answer
			result = mqrecv_spmsg(msg, mqid, maxsize, nullptr, timeout_us);
		}
		// Close mq
		if (mq_close(mqid) < 0)
			printf("MQ close error: %s\n", ansname.c_str());
		if (mq_unlink(mqid) < 0)
			printf("MQ unlink error %s\n", ansname.c_str());
		return result;
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
	if (d_maxsize < 0) return MQLIB_ERROR;
	// Notify
	notify_setup(this);
	if (d_error.load() != 0) return d_error.load();
    // Start thread
    d_isalive.store(true);
	d_recv_thread = std::thread(&MqReceiver::recv_loop, this);
	return 0;
}

void MqReceiver::notify_setup(MqReceiver * _mqreceiver)
{
	struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD; /* Notify via thread */
    sev.sigev_notify_function = &MqReceiver::mq_event;
    sev.sigev_notify_attributes = nullptr; /* Could be pointer to pthread_attr_t structure */
    sev.sigev_value.sival_ptr = _mqreceiver; /* Argument to threadFunc() */

    if (mq_notify(_mqreceiver->d_mqid, &sev) < 0)
    {
        printf("MQ notify error %s\n", _mqreceiver->d_mqname.c_str());
        _mqreceiver->d_error.store(spirit::MQLIB_ERROR);
        return;
    }
}

void MqReceiver::mq_event(union sigval sv)
{
	MqReceiver * mq_recever_ptr = (MqReceiver *)(sv.sival_ptr);
	lock_guard<mutex> lk(mq_recever_ptr->d_mut);
	notify_setup(mq_recever_ptr);
	d_isrecv.store(true);
	mq_recever_ptr->d_cond.notify_all();
	pthread_exit(nullptr);
}

int MqReceiver::timedrecv(SpiritMsg & spmsg, unsigned int timeout_us)
{
	int result = 0;
	lock_guard<mutex> lk(d_mut);
	bool waitres = d_cond.wait_for(lk, std::chrono::microseconds(timeout_us), [this]{return ( d_isrecv.load() || !(d_isalive.load()) );});
	if (waitres && d_isalive.load()) {
		result = mqrecv_spmsg(spmsg, d_mqid, d_maxsize, nullptr);
	} else {
		result = MQ_EMPTY;
	}
	d_isrecv.store(false);
	return result;
}

void MqReceiver::recv_loop()
{
	SpiritMsg spmsg;
	int result = 0;
	bool isans;
	while (d_isalive.load()) {
		lock_guard<mutex> lk(d_mut); // ???
		result = 0;
		// Wait for event
		d_cond.wait(lk,[this]{return ( ( d_isrecv.load() && (!d_ispaused.load()) ) || !(d_isalive.load()) );});
		while ( result >= 0 && d_isalive.load() )
		{
			result = mqrecv_spmsg(spmsg, d_mqid, d_maxsize, nullptr);
			isans = static_cast<bool>(spirit_msg_is_ans(&spmsg));
			if (result >= 0) {
				// Check if it is req or ans
				// Invoke handler
				result = d_msg_handler(spmsg);
				// If need answer, send it
				if (!isans) {
					if (result >= 0) {
						result = answer(spmsg);
					}
				}
			}
		}

		d_isrecv.store(false);
	}
	if (spmsg.data != nullptr)
		delete spmsg.data;
}

int MqReceiver::answer(SpiritMsg & spmsg)
{
	std::string ans_name = make_ansmqname(d_mqname, spmsg.receiver);
	// Set or check ANS bit here
	return onetime_mqsend(ans_name, spmsg);
}

void MqReceiver::pause(bool ispaused)
{
	d_ispaused.store(ispaused);
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
		printf("MQ unlink error %s\n", d_mqname.c_str());	
}
