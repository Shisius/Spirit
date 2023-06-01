#ifndef _SHAMAN_H_
#define _SHAMAN_H_

#include <memory>

#include "spirit_base_tools.h"

class Shaman;

namespace spirit
{

	int create_shaman(const SpiritNote & note, std::unique_ptr<Shaman> shaman);

} // spirit

class Shaman
{
protected:

	SpiritNote d_note;

public:

	Shaman(const SpiritNote & note) : d_note(note) {}
	~Shaman();

	int req(unsigned char title, void * data);

};

#endif //_SHAMAN_H_
