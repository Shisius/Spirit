#ifndef _SPIRIT_MASTER_TOOLS_H_
#define _SPIRIT_MASTER_TOOLS_H_

#include <string>
#include <vector>
#include "spirit_types.h"
#include "app_defs.h"
#include "json.h" // from third

namespace spirit
{
	// Constatns
	const std::string SPIRIT_FOLDER_PATH_CONST = "/etc/spirit.d";
	const std::string SPIRIT_FOLDER_PATH_TEMP = "/var/spirit";
	const std::string SPIRIT_FOLDER_PATH_STATE = SPIRIT_FOLDER_PATH_TEMP + "/state";
	const std::string SPIRIT_DEFAULT_TIMEOUT_US = 1000;
} // spirit

namespace spirit
{	
	// Parse & pack
	bool to_json(const SpiritSystemUnit & unit, std::string & str);
	bool from_json(const std::string & str, SpiritSystemUnit & unit);

	// File managing
	int create_const_spirit_folder();
	int create_temp_spirit_folder();
	int get_spirit_unit_file_names(std::vector<std::string> & filenames);
	int write_state_file(const SpiritSystemUnit & unit, const SpiritState & state);
	int read_spirit_unit_file(const std::string & filename, SpiritSystemUnit & unit);

	// Data conversions
	SpiritNote get_spirit_note(SpiritSystemUnit & unit);

	// Spirit manage
	int spirit_start(SpiritSystemUnit & unit);
	int spirit_stop(SpiritSystemUnit & unit);
	int spirit_check(SpiritSystemUnit & unit);
	int spirit_restart(SpiritSystemUnit & unit);

} // spirit

#endif //_SPIRIT_MASTER_TOOLS_H_
