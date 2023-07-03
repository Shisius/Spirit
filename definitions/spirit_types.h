#ifndef _SPIRIT_TYPES_H_
#define _SPIRIT_TYPES_H_

#include "spirit_msg.h"
#include "spirit_state.h"

#if defined __cplusplus
extern "C" {
#endif

/**
 * Spirit protocol version. It is defined in spirit_msg.h.
 */
typedef unsigned int spirit_version_t;
/**
 * Time in microseconds. Usually used with ACCEPTED answer.
 */
typedef unsigned int spirit_time_t; 

/**
 * Spirit description. For shaman creation
 * role - spirit role
 * way - communication way (IPC, RPMSG, IOCTL...)
 * titles - 32 x hex message words (32 * 8 = 256) - define available message titles (0x02 0x00 0x00 0x00 ... - means only title 0x01 is available)
 * timeout - 4x4 bit fields. Timeout value in microseconds = base << field value 
 */
#define SPIRIT_TITLES_NOTE_SIZE 32
#define SPIRIT_SPECIAL_NAME_MAX_LENGTH 32
typedef enum
{
    SPIRIT_FMT_WAY_IPCMQ = 0,   // Inter process, data is sent by message queue
    SPIRIT_FMT_WAY_RPMSG = 1,   // Remoteproc message
    SPIRIT_FMT_WAY_IOCTL = 2,   // IOCTL
    SPIRIT_FMT_WAY_ITHR = 3,    // Inter threads
} SPIRIT_FMT_WAY;
typedef struct spirit_note 
{
	unsigned char role; //SPIRIT_ROLE
	unsigned char way; //SPIRIT_FMT_WAY
    unsigned short timeout; //Timeout complex word
	unsigned char[SPIRIT_TITLES_NOTE_SIZE] titles;
    unsigned char[SPIRIT_SPECIAL_NAME_MAX_LENGTH] name;
} __attribute__((packed, aligned(1))) SpiritNote;

/**
 * Response - means time between spirit request and fast answer(accepted/denied)
 */
typedef enum
{
    SPIRIT_TIMEOUT_MASK_RESPONSE = 0x000F
} SpiritTimeoutMask;

typedef enum
{
    SPIRIT_TIMEOUT_BASE_RESPONSE = 1
} SpiritTimeoutBase;

typedef struct _spirit_state_event_t
{
    spirit_time_t time;
} SpiritStateEvent;

#if defined __cplusplus
}
#endif 

#endif //_SPIRIT_TYPES_H_

