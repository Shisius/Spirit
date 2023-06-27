#ifndef _SPIRIT_MSG_H_
#define _SPIRIT_MSG_H_

#if defined __cplusplus
extern "C" {
#endif 

#define SPIRIT_MSG_VERSION 0x01020300

/**
 * Spirit format structure (from LSB to MSB):
 * DIR(1 bit) TYPE(3 bits) ASK(1 bit) PTR(3 bits)
 * DIR can be request or answer
 * TYPE can be CUSTOM (role specific message, depends on title) or one of universal for every role messages.
 * PTR means where the data is stored
 */

/**
 * SPIRIT_FMT_DIR_MASK
 * If last (format&SPIRIT_FMT_DIR_MASK) bit == 1, sender's title should be used, 0 - receiver's title should be used
 */
#define SPIRIT_FMT_DIR_MASK 0x01

typedef enum
{
	SPIRIT_FMT_REQ = 0,
	SPIRIT_FMT_ANS = 1
} SPIRIT_FMT_DIR;

/**
 * SPIRIT_FMT_TYPES CUSTOM Messages is used for data transferring and depends of sender's/receiver's title
 * Other messages is universal for every spirit
 * ACCEPTED - cmd accepted, if size > 0 => wait for be finished. After this, DONE or CUSTOM will be sent. If time > 0, wait for time. If time = 0, just wait.
 */
#define SPIRIT_FMT_TYPE_MASK 0x0E
#define SPIRIT_FMT_TYPE_MASK_POS 0
typedef enum
{
	SPIRIT_FMT_ANS_CUSTOM = 0,
	SPIRIT_FMT_ANS_ACCEPTED = 1,
	SPIRIT_FMT_ANS_DENIED = 2,
	SPIRIT_FMT_ANS_DONE = 3
} SPIRIT_FMT_ANS_TYPE;
typedef enum
{
	SPIRIT_FMT_REQ_CUSTOM = 0

} SPIRIT_FMT_REQ_TYPE;

/**
 * ASK bit. 
 * If DIR = SPIRIT_FMT_ANS and ASK bit = 1, Spirit1 wants other spirit2 to ask him. 
 * Spirit2 should send empty request with ASK bit. 
 * Spirit1 answers by desired request.
 * REQ -> ANS+ASK -> EREQ+ASK -> REQ -> ANS.
 */
#define SPIRIT_FMT_ASK_MASK 0x10

#define SPIRIT_FMT_PTR_MASK 0xE0
#define SPIRIT_FMT_PTR_MASK_POS 5
typedef enum
{
	SPIRIT_FMT_PTR_SAME = 0, 	// Data is sent the same way as the message
	SPIRIT_FMT_PTR_SHM = 1,		// Data in shmem. Value is offset
	SPIRIT_FMT_PTR_PHM = 2,		// Data in physical memory.
	SPIRIT_FMT_PTR_FILE = 3 	// Data in file. Value is offset
} SPIRIT_FMT_PTR;

#define SPIRIT_TITLES_NUMBER 256
/**
 * Common titles. For custom titles use SPIRIT_TITLE_COMMON_FIRST as the first title number.
 * It is recommended to use the same title for request and answer for it.
 */
typedef enum
{
	SPIRIT_TITLE_STATE = 0,
	SPIRIT_TITLE_SHUTDOWN = 1,
	SPIRIT_TITLE_EVENT = 2,			// Event for state bit. Argument is state bit position?
	SPIRIT_TITLE_CUSTOM_FIRST = 16
} SPIRIT_COMMON_TITLES;

typedef struct SpiritMsg SpiritMsg;
struct __attribute__((packed, aligned(1))) SpiritMsg
{
	// Header 32 bit
	unsigned char sender; 	// sender's role
	unsigned char receiver; // receiver's role
	unsigned char title;	// role specific message header
	unsigned char format;

	// Data size 32 bit
	unsigned int size;
	// Data pointer. Can be array[]
	void * data;
};

#if defined __cplusplus
}
#endif 

#endif //_SPIRIT_MSG_H_
