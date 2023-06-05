#ifndef _SHAMAN_H_
#define _SHAMAN_H_

#include <memory>

#include "spirit_base_tools.h"

class Shaman;

namespace spirit
{

	std::unique_ptr<Shaman> create_shaman(const SpiritNote & self_note, const SpiritNote & target_note);

} // spirit

class Shaman
{
protected:

	SpiritNote d_target_note;
	SpiritNote d_self_note;

public:

	Shaman(const SpiritNote & self_note, const SpiritNote & target_note) : d_target_note(target_note), d_self_note(self_note) {}
	~Shaman();

	int req(unsigned char title, void * data, unsigned int size);

};

#endif //_SHAMAN_H_
