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
unsigned short set_resp_timeout_us(unsigned short timeout_bit, unsigned int timeout_us);

/// Message
SpiritMsg create_spirit_msg(unsigned char sender, unsigned char receiver, unsigned char title, unsigned int size, void * data); 
// Pack Spirit Message into char buf. Returns buf size of -1 if failed.
//int pack_spirit_msg(const SpiritMsg * msg, char * buf);
// Unpack Spirit Message from char buf. Returns 0 in success, -1 if failed.
//int unpack_spirit_msg(const char * buf, unsigned int size, SpiritMsg * msg);

// None. msg(0,0,0,0) is NoneMsg. 
// spirit_msg_is_null returns 1 if msg is null and 0 if is not.
// spirit_msg_check returns 0 is msg is good and >0 if smth is wrong in msg.
unsigned int spirit_msg_check(SpiritMsg* smsg);
SpiritMsg create_spirit_null_msg();
unsigned int spirit_msg_is_null(SpiritMsg* smsg);

// Make format
void spirit_msg_set_ptr(SpiritMsg* smsg, unsigned char ptr);
void spirit_msg_set_dir(SpiritMsg* smsg, unsigned char dir);
void spirit_msg_set_type(SpiritMsg* smsg, unsigned char type);
void spirit_msg_set_ask(SpiritMsg * smsg);

void spirit_msg_make_ans(SpiritMsg* smsg);
void spirit_msg_make_req(SpiritMsg* smsg);
void spirit_msg_make_cho(SpiritMsg* smsg); // set req and ask bit.
void spirit_msg_make_accepted(SpiritMsg* smsg);
void spirit_msg_make_denied(SpiritMsg* smsg);
void spirit_msg_make_done(SpiritMsg* smsg);

// Functions is_ask, is_ans, etc - if not - returns 0, if it is - returns > 0.
unsigned char spirit_msg_get_ptr(SpiritMsg* smsg);
unsigned char spirit_msg_get_type(SpiritMsg* smsg);
unsigned char spirit_msg_is_ask(SpiritMsg* smsg);

// Make special spirit messages. Ptr bits won't be set.
SpiritMsg spirit_msg_make_req_state(unsigned char sender, unsigned char receiver);
SpiritMsg spirit_msg_make_ans_state(unsigned char sender, unsigned char receiver, SpiritState* state);

/// Names
//const char * spirit_make_name();
const char * spirit_role2str(unsigned char roleid);
const char * spirit_state2str(SpiritState * state);

/// Note
// spirit_note_is_null returns 1 if note is null and 0 if is not.
SpiritNote create_spirit_null_note();
unsigned int spirit_note_is_null();
//SpiritNote create_spirit_note(unsigned char role, unsigned char way)
void set_spirit_note_title(SpiritNote * note, unsigned char title);

/// Master
SpiritNote get_spirit_master_note();

#if defined __cplusplus
}
#endif 

#endif //_SPIRIT_BASE_TOOLS_H_
