#ifndef _SPIRIT_ROLES_H_
#define _SPIRIT_ROLES_H_

#if defined __cplusplus
extern "C" {
#endif 

/**
 * Spirit roles. Every spirit must choose one of the roles. 
 * At one time only one spirit with certain role can exist.
 * One spirit can take two or more roles
 */
typedef enum 
{
	SPIRIT_ROLE_MASTER = 0,
	SPIRIT_ROLE_CONTROL,
	SPIRIT_ROLE_FPGA,
	SPIRIT_ROLE_SUL,
	SPIRIT_ROLE_INS,
	SPIRIT_ROLE_POS_CTL,
	SPIRIT_ROLE_POS_SENSOR,
	SPIRIT_ROLE_FPGA_SYNCH,
	SPIRIT_ROLE_FPGA_DSP,
	SPIRIT_ROLE_FPGA_WATCH,
	SPIRIT_ROLE_POWER_CTL,
	SPIRIT_ROLE_MAX
} SPIRIT_ROLE;

#if defined __cplusplus
}
#endif 

#endif //_SPIRIT_ROLES_H_
