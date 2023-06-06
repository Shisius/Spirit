#ifndef _APP_DEFS_H_
#define _APP_DEFS_H_

/**
 * SpiritSystemUnit
 * Describe spirit init file content.
 */
struct SpiritSystemUnit
{
	unsigned char role;
	SPIRIT_FMT_WAY way;
	std::string name;
	unsigned int start_timeout_us;
	unsigned int stop_timeout_us;
	std::vector<unsigned char> titles;
	// std::string start_cmd;
};

#endif //_APP_DEFS_H_
