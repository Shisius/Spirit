#ifndef _SPIRIT_STATE_H_
#define _SPIRIT_STATE_H_

#if defined __cplusplus
extern "C" {
#endif 

typedef enum
{
	SPIRIT_STATE_ALIVE = 0,
	SPIRIT_STATE_ACTIVE,
	SPIRIT_STATE_BROKEN,
	SPIRIT_STATE_RESTART
} SpiritStateSystemBit;

typedef struct spirit_state
{
	unsigned char system;
	unsigned char role;
	unsigned char project;
	unsigned char product;
} __attribute__((packed, aligned(1))) SpiritState;

#if defined __cplusplus
}
#endif 

#endif //_SPIRIT_STATE_H_
