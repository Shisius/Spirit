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

typedef enum
{
	SPIRIT_STATE_ROLE_START_BIT = 8,
	SPIRIT_STATE_MODE_START_BIT = 16,
	SPIRIT_STATE_VAR_START_BIT = 24
} SpiritStateStartBit;

/**
 * Full state consist of
 * system - spirit state. SpiritStateSystemBits are here.
 * role - role specific state bits.
 * mode - in different modes or projects different states or events can occurs.
 * var - store some variable state bits here. Different bits for different variations.
 */
typedef struct _spirit_state_t
{
	unsigned char system;
	unsigned char role;
	unsigned char mode;
	unsigned char var;
} __attribute__((packed, aligned(1))) SpiritState;

#if defined __cplusplus
}
#endif 

#endif //_SPIRIT_STATE_H_
