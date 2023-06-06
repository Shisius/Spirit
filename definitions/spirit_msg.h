#ifndef _SPIRIT_MSG_H_
#define _SPIRIT_MSG_H_

#if defined __cplusplus
extern "C" {
#endif 

#define SPIRIT_MSG_VERSION 0x010102

/**
 * Spirit format structure (from LSB to MSB):
 * DIR(1 bit) TYPE(3 bit) WAY(4 bit)
 * DIR can be request or answer
 * TYPE can be CUSTOM (role specific message, depends on title) or one of universal for every role messages.
 * WAY means what kind of communication is used
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
 */
#define SPIRIT_FMT_TYPE_MASK 0x0E
typedef enum
{
	SPIRIT_FMT_ANS_CUSTOM = 0,
	SPIRIT_FMT_ANS_ACCEPTED = 1,
	SPIRIT_FMT_ANS_DENIED = 2,
	SPIRIT_FMT_ANS_STATE = 3
} SPIRIT_FMT_ANS_TYPE;
typedef enum
{
	SPIRIT_FMT_REQ_CUSTOM = 0,
	SPIRIT_FMT_REQ_STATE = 1,
	SPIRIT_FMT_REQ_SHUTDOWN = 2
} SPIRIT_FMT_REQ_TYPE;

/**
 * ASK bit. 
 * If DIR = SPIRIT_FMT_ANS and ASK bit = 1, Spirit1 wants other spirit2 to ask him. 
 * Spirit2 should send empty request with ASK bit. 
 * Spirit1 answers by desired request.
 * REQ -> ANS+ASK -> EREQ+ASK -> REQ -> ANS.
 */
#define SPIRIT_FMT_ASK_MASK 0x10

#define SPIRIT_FMT_WAY_MASK 0xE0
typedef enum
{
	SPIRIT_FMT_WAY_IPC = 0, // Inter process
	SPIRIT_FMT_WAY_ITHR,	// Inter threads
	SPIRIT_FMT_WAY_RPMSG,	// Remoteproc message
	SPIRIT_FMT_WAY_IOCTL	// IOCTL
} SPIRIT_FMT_WAY;

#define SPIRIT_TITLES_NUMBER 256
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
