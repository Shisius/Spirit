#ifndef _SPIRIT_BASE_TOOLS_H_
#define _SPIRIT_BASE_TOOLS_H_

#include "spirit_types.h"
#include "spirit_master_defs.h"
#include "spirit_names.h"

#if defined __cplusplus
extern "C" {
#endif 

/// State
unsigned char spirit_state_set(unsigned char state, unsigned char bit, unsigned char value);
unsigned char spirit_state_get(unsigned char state, unsigned char bit);
unsigned char spirit_state_alive(unsigned char state);

/// Timeout
unsigned int get_resp_timeout_us(unsigned short timeout_bit);

/// Message
SpiritMsg create_spirit_msg(unsigned char sender, unsigned char receiver, unsigned char title, unsigned int size, void * data); 
// Pack Spirit Message into char buf. Returns buf size of -1 if failed.
//int pack_spirit_msg(const SpiritMsg * msg, char * buf);
// Unpack Spirit Message from char buf. Returns 0 in success, -1 if failed.
//int unpack_spirit_msg(const char * buf, unsigned int size, SpiritMsg * msg);

unsigned int spirit_msg_check(SpiritMsg* smsg);
SpiritMsg create_spirit_none_msg();
unsigned int spirit_msg_is_none();

// Make format
void spirit_msg_set_way(SpiritMsg* smsg, unsigned char way);
void spirit_msg_set_dir(SpiritMsg* smsg, unsigned char dir);
void spirit_msg_set_type(SpiritMsg* smsg, unsigned char type);
void spirit_msg_make_ans(SpiritMsg* smsg);
void spirit_msg_make_req(SpiritMsg* smsg);
void spirit_msg_set_ask(SpiritMsg * smsg);

// Make special spirit messages. Way bits won't be set.
SpiritMsg spirit_msg_make_req_state(unsigned char sender, unsigned char receiver);
SpiritMsg spirit_msg_make_ans_state(unsigned char sender, unsigned char receiver, SpiritState state);

/// Names
//const char * spirit_make_name();
const char * spirit_role2str(unsigned char roleid);
const char * spirit_state2str(SpiritState * state);

/// Note
SpiritNote create_spirit_none_note();
unsigned int spirit_note_is_none();

/// Master
SpiritNote get_spirit_master_note();

#if defined __cplusplus
}
#endif 

#endif //_SPIRIT_BASE_TOOLS_H_
