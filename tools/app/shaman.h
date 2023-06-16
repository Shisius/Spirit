#ifndef _SHAMAN_H_
#define _SHAMAN_H_

#include <memory>

#include "spirit_base_tools.h"

class Shaman;

namespace spirit
{

	std::unique_ptr<Shaman> create_shaman(const SpiritNote & self_note, const SpiritNote & target_note);
	SpiritNote get_note(unsigned char role_id);

} // spirit

enum class ShamanReqResult : int
{
	CONN_ERR = -2,
	DENIED,
	ACCEPTED,
	DATA
}

enum class ShamanReqFlags : unsigned int
{

}

class Shaman
{
protected:

	SpiritNote d_target_note;
	SpiritNote d_self_note;

	SpiritMsg d_lastmsg;

public:

	Shaman(const SpiritNote & self_note, const SpiritNote & target_note) : d_target_note(target_note), d_self_note(self_note) {}
	~Shaman();

	virtual int req(unsigned char title, void * data, unsigned int size, unsigned int flags);

	// Returns Title
	unsigned char get_ans(void * data, unsigned int size); // ???

	SpiritState req_state();

	//virtual int send_spmsg(SpiritMsg & msg) = 0;

};

class MqShaman : public Shaman
{
protected:

	std::unique_ptr<MqReceiver> d_ansmq;

public:

	MqShaman(const SpiritNote & self_note, const SpiritNote & target_note) : Shaman(self_note, target_note) {}
	~MqShaman();

};

#endif //_SHAMAN_H_
