#ifndef _SPIRIT_MASTER_DEFS_H_
#define _SPIRIT_MASTER_DEFS_H_

#define SPIRIT_MASTER_ROLE 0

#if defined __cplusplus
extern "C" {
#endif 

typedef enum
{
	SPIRIT_MASTER_TITLE_NOTE = 0, 	// Ask for SpiritNote. Need ROLE_ID
	SPIRIT_MASTER_TITLE_VERSION,	// Ask for version. Returns spirit_version_t
	SPIRIT_MASTER_TITLE_UPDATE,		// Update one spirit. Need ROLE ID. For no parameters - update all
	SPIRIT_MASTER_TITLE_ENABLE,		// Enable one spirit. Need ROLE ID.
	SPIRIT_MASTER_TITLE_DISABLE		// Disable one spirit. Need ROLE ID.
} SPIRIT_MASTER_TITLE;

#if defined __cplusplus
}
#endif 

#endif //_SPIRIT_MASTER_DEFS_H_
