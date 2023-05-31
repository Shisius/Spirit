#ifndef _SPIRIT_BASE_H_
#define _SPIRIT_BASE_H_

#include "spirit_base_tools.h"
#include <string>

class SpiritBase
{
protected:

	SpiritState d_state;

public:

	SpiritBase(const std::string & args);
	~SpiritBase();

	virtual int setup();

	virtual SpiritNote get_note();


	SpiritState get_state() {return d_state;}

};

#endif //_SPIRIT_BASE_H_
