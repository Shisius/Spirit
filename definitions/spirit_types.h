#ifndef _SPIRIT_TYPES_H_
#define _SPIRIT_TYPES_H_

#include "spirit_msg.h"
#include "spirit_state.h"

#if defined __cplusplus
extern "C" {
#endif

/**
 * Spirit description. For shaman creation
 * role - spirit role
 * way - communication way (IPC, RPMSG, IOCTL...)
 * titles - 32 x hex message words (32 * 8 = 256) - define available message titles (0x02 0x00 0x00 0x00 ... - means only title 0x01 is available)
 */
#define SPIRIT_TITLES_NOTE_SIZE 32
#define SPIRIT_SPECIAL_NAME_MAX_LENGTH 32
typedef struct spirit_note 
{
	unsigned char role; //SPIRIT_ROLE
	unsigned char way; //SPIRIT_FMT_WAY
	unsigned char[SPIRIT_TITLES_NOTE_SIZE] titles;
    unsigned char[SPIRIT_SPECIAL_NAME_MAX_LENGTH] name;
} __attribute__((packed, aligned(1))) SpiritNote;

static const char SPIRIT_START_WORD[] = "start";
static const char SPIRIT_STOP_WORD[] = "stop";
static const char SPIRIT_RESTART_WORD[] = "restart";
static const char SPIRIT_CHECK_WORD[] = "check";

static const char SPIRIT_MASTER_NAME[] = "spiritd";

#if defined __cplusplus
}
#endif 

#endif //_SPIRIT_TYPES_H_

