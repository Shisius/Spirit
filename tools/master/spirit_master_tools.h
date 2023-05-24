#ifndef _SPIRIT_MASTER_TOOLS_H_
#define _SPIRIT_MASTER_TOOLS_H_

#include "spirit_types.h"
#include <string>
#include <vector>
#include "writer.h"
#include "document.h"


/**
 * SpiritSystemUnit
 * Describe spirit init file content.
 */
struct SpiritSystemUnit
{
	unsigned char role;
	SPIRIT_FMT_WAY way;
	std::string name;
	std::vector<unsigned char> titles;
};

namespace spirit
{
	// Constatns
	const std::string SPIRIT_FOLDER_PATH = "/etc/spirit.d";
	
	// Parse & pack
	bool to_json(const SpiritSystemUnit & unit, std::string & str);
	bool from_json(const std::string & str, SpiritSystemUnit & unit);

	// File managing
	bool create_spirit_folder();
	bool get_spirit_file_notes(std::vector<std::string> & filenames);

	// Data conversions
	SpiritNote get_spirit_note(SpiritSystemUnit & unit);

} // spirit

#endif //_SPIRIT_MASTER_TOOLS_H_
