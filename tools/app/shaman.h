#ifndef _SHAMAN_H_
#define _SHAMAN_H_

#include <memory>
#include <unordered_map>

#include "ipcmqlib.h"
#include "spirit_base_tools.h"

class Shaman;

namespace spirit
{

	std::unique_ptr<Shaman> create_shaman(const SpiritNote & self_note, const SpiritNote & target_note);
	SpiritNote get_note(unsigned char role_id);

} // spirit

enum class ShamanReqResult : int
{
	CONN_ERR = -3,
	TIMEOUT = -2,
	DENIED = -1,
	ACCEPTED = 0,
	WAIT = 1,
	DATA = 2
}

enum class ShamanReqFlags : unsigned int
{
	BLOCK = 1,		// If spirit returns WAIT, shaman waits for answer until it comming. Blocking request.
	STORE = 2		// If spirit returns WAIT, shaman stores the answer in the storage.
}

class Shaman
{
protected:

	SpiritNote d_target_note;
	SpiritNote d_self_note;

	SpiritMsg d_lastmsg;

	std::function<int(SpiritMsg&)> d_ans_handler = nullptr;

	std::unordered_map<unsigned char, SpiritMsg> d_storage;

public:

	Shaman(const SpiritNote & self_note, const SpiritNote & target_note) : d_target_note(target_note), d_self_note(self_note) {}
	~Shaman();

	virtual int req(unsigned char title, void * data, unsigned int size, unsigned int flags);

	// Returns Title
	unsigned char get_ans(void * data, unsigned int size); // ???

	SpiritState req_state();

	template< class _MsgHandler, class _Master>
	void set_handler(_MsgHandler&& msg_handler, _Master&& master_ptr) {d_ans_handler = std::bind(msg_handler, master_ptr, std::placeholders::_1);}

	//virtual int send_spmsg(SpiritMsg & msg) = 0;

};

class MqShaman : public Shaman
{
protected:

	std::unique_ptr<MqReceiver> d_ansmq;

public:

	MqShaman(const SpiritNote & self_note, const SpiritNote & target_note) : Shaman(self_note, target_note) {}
	~MqShaman();

	virtual int req(unsigned char title, void * data, unsigned int size, unsigned int flags);

};

#endif //_SHAMAN_H_
