
#include "spirit_base_tools.h"
/// State
unsigned char spirit_state_set(unsigned char state, unsigned char bit, unsigned char value)
{
	if (value) return state | (1 << (bit % 8));
	return state & ~(1<<bit);
}
unsigned char spirit_state_get(unsigned char state, unsigned char bit)
{
	return (state >> (bit % 8)) & 0x1;
}
unsigned char spirit_state_alive(unsigned char state)
{
	return spirit_state_get(state, SPIRIT_STATE_ALIVE);
}
/// Timeout
unsigned int get_resp_timeout_us(unsigned short timeout_bit)
{
	unsigned char pos = 0;
	// Check mask
	if (SPIRIT_TIMEOUT_MASK_RESPONSE == 0) return SPIRIT_TIMEOUT_BASE_RESPONSE;
	// Calc mask position
	while ((SPIRIT_TIMEOUT_MASK_RESPONSE >> pos)&1 == 0) pos++;
	// Calc timeout
	return SPIRIT_TIMEOUT_BASE_RESPONSE << ( (timeout_bit & SPIRIT_TIMEOUT_MASK_RESPONSE) >> pos );
}
unsigned short set_resp_timeout_us(unsigned short timeout_bit, unsigned int timeout_us)
{
	unsigned char pos = 0;
	unsigned char ext = 0;
	// Check mask
	if (SPIRIT_TIMEOUT_MASK_RESPONSE == 0) return timeout_bit;
	// Calc mask position
	while ((SPIRIT_TIMEOUT_MASK_RESPONSE >> pos)&1 == 0) pos++;
	// Calc timeout bit value: (base << value) >= timeout
	while ((SPIRIT_TIMEOUT_BASE_RESPONSE << ext) <= timeout_us) ext++;
	// Check if bit value fits the mask
	if (ext > (SPIRIT_TIMEOUT_MASK_RESPONSE >> pos)) ext = SPIRIT_TIMEOUT_MASK_RESPONSE;
	// Write value to mask
	return (timeout_bit & ~(SPIRIT_TIMEOUT_MASK_RESPONSE)) | ((ext << pos) & SPIRIT_TIMEOUT_MASK_RESPONSE);
}
/// Message
SpiritMsg create_spirit_msg(unsigned char sender, unsigned char receiver, unsigned char title, unsigned int size, void * data);
{
	SpiritMsg spmsg;

	spmsg.sender = sender;
	spmsg.receiver = receiver;
	spmsg.title = title;
	spmsg.format = 0;

	spmsg.size = size;
	spmsg.data = data;
	return spmsg;
}
// None
unsigned int spirit_msg_check(SpiritMsg* smsg)
{
	// Sender and receiver must be different
	if (smsg.sender == smsg.receiver) return 1;
	// Some else conditions?
	return 0;
}
SpiritMsg create_spirit_null_msg()
{
	return create_spirit_msg(0,0,0,0,NULL);
}
unsigned int spirit_msg_is_null(SpiritMsg* smsg)
{
	if (smsg.sender == 0 && smsg.receiver == 0 && smsg.title == 0 && smsg.format == 0) return 1;
	return 0;
}
// Make format
void spirit_msg_set_ptr(SpiritMsg* smsg, unsigned char ptr)
{
	// Clear format field
	smsg->format &= ~(SPIRIT_FMT_PTR_MASK);
	// Set format field
	smsg->format |= (ptr << SPIRIT_FMT_PTR_MASK_POS) & SPIRIT_FMT_PTR_MASK;
}
void spirit_msg_set_dir(SpiritMsg* smsg, unsigned char dir)
{
	if (dir) smsg->format |= SPIRIT_FMT_DIR_MASK;
	else smsg->format &= ~(SPIRIT_FMT_DIR_MASK);
}
void spirit_msg_set_type(SpiritMsg* smsg, unsigned char type)
{
	// Clear format field
	smsg->format &= ~(SPIRIT_FMT_TYPE_MASK);
	// Set format field
	smsg->format |= (type << SPIRIT_FMT_TYPE_MASK_POS) & SPIRIT_FMT_TYPE_MASK;
}
void spirit_msg_set_ask(SpiritMsg * smsg)
{
	smsg->format |= SPIRIT_FMT_ASK_MASK;
}

void spirit_msg_make_ans(SpiritMsg* smsg)
{
	spirit_msg_set_dir(smsg, SPIRIT_FMT_ANS);
}
void spirit_msg_make_req(SpiritMsg* smsg)
{
	spirit_msg_set_dir(smsg, SPIRIT_FMT_REQ);
}
void spirit_msg_make_cho(SpiritMsg* smsg)
{
	spirit_msg_make_req(smsg);
	spirit_msg_set_ask(smsg);
}
void spirit_msg_make_accepted(SpiritMsg* smsg)
{
	spirit_msg_make_ans(smsg);
	spirit_msg_set_type(smsg, SPIRIT_FMT_ANS_ACCEPTED);
}
void spirit_msg_make_denied(SpiritMsg* smsg)
{
	spirit_msg_make_ans(smsg);
	spirit_msg_set_type(smsg, SPIRIT_FMT_ANS_DENIED);
}
void spirit_msg_make_done(SpiritMsg* smsg)
{
	spirit_msg_make_ans(smsg);
	spirit_msg_set_type(smsg, SPIRIT_FMT_ANS_DONE);
}

unsigned char spirit_msg_get_ptr(SpiritMsg* smsg)
{
	return (smsg->format >> SPIRIT_FMT_PTR_MASK_POS) & SPIRIT_FMT_PTR_MASK;
}
unsigned char spirit_msg_get_type(SpiritMsg* smsg)
{
	return (smsg->format >> SPIRIT_FMT_TYPE_MASK_POS) & SPIRIT_FMT_TYPE_MASK;
}
unsigned char spirit_msg_is_ask(SpiritMsg* smsg)
{
	return smsg->format & SPIRIT_FMT_ASK_MASK;
}
unsigned char spirit_msg_is_ans(SpiritMsg* smsg)
{
	return smsg->format & SPIRIT_FMT_DIR_MASK;
}

SpiritMsg spirit_msg_make_req_state(unsigned char sender, unsigned char receiver)
{
	SpiritMsg msg = create_spirit_msg(sender, receiver, SPIRIT_TITLE_STATE, 0, NULL);
	spirit_msg_make_req(&msg);
	return msg;
}
SpiritMsg spirit_msg_make_ans_state(unsigned char sender, unsigned char receiver, SpiritState* state)
{
	SpiritMsg msg = create_spirit_msg(sender, receiver, SPIRIT_TITLE_STATE, sizeof(SpiritState), state);
	spirit_msg_make_ans(&msg);
	return msg;
}

/// Names
// const char * spirit_byte2str_hex(unsigned char byte)
// {

// }
const char * spirit_role2str(unsigned char roleid)
{
	char str[2];
	unsigned char part = roleid & 0xF;
	if (part < 0xA) str[0] = '0' + part;
	else str[0] = 'A' + part - 0xA;

	part = (roleid & 0xF0) >> 4;
	if (part < 0xA) str[1] = '0' + part;
	else str[1] = 'A' + part - 0xA;

	return str;
}
void spirit_byte2str_bin(char * str, unsigned char pos, unsigned char data)
{
	for (unsigned char i = 0; i < 8; i++){
		if ( (data >> i)&1 )
			str[pos] = '1';
		else
			str[pos] = '0';
		pos++;
	}
}
const char * spirit_state2str(SpiritState * state)
{
	char str[8*4 + 3];
	unsigned char pos = 0;
	// System
	spirit_byte2str_bin(str, pos, state->system);
	str[pos] = ' '; pos++;
	spirit_byte2str_bin(str, pos, state->role);
	str[pos] = ' '; pos++;
	spirit_byte2str_bin(str, pos, state->mode);
	str[pos] = ' '; pos++;
	spirit_byte2str_bin(str, pos, state->var);
	return str;
}

/// Note
SpiritNote create_spirit_null_note()
{
	SpiritNote note;
	note.role = 0;
	note.way = 0;
	note.timeout = 0;
	for (unsigned char i = 0; i < SPIRIT_TITLES_NOTE_SIZE; i++)
		note.titles[i] = 0;
	for (unsigned char i = 0; i < SPIRIT_SPECIAL_NAME_MAX_LENGTH; i++)
		note.name[i] = 0;
	return note;
}
unsigned int spirit_note_is_null()
{
	if (note.role == 0 && note.way == 0 && note.timeout == 0 && note.titles[0] == 0 && note.name[0] == 0) return 1;
	return 0;
}
void set_spirit_note_title(SpiritNote * note, unsigned char title)
{
	unsigned char i_byte = title >> 3; // /8
	unsigned char i_pos = title % 8;
	note->titles[i_byte] |= (1 << i_pos);
}

/// Master
SpiritNote get_spirit_master_note()
{
	SpiritNote note = create_spirit_null_note();
	
	note.role = SPIRIT_MASTER_ROLE;
	note.way = SPIRIT_FMT_WAY_IPCMQ;
	// Timeout
	note.timeout = set_resp_timeout_us(note.timeout, SPIRIT_MASTER_RESPONSE_DEFAULT);
	// Titles
	set_spirit_note_title(&note, SPIRIT_TITLE_STATE);
	set_spirit_note_title(&note, SPIRIT_TITLE_SHUTDOWN);
	set_spirit_note_title(&note, SPIRIT_MASTER_TITLE_NOTE);
	// name
	char spname[8] = 'spiritd';
	for (unsigned char i = 0; i < 8; i++)
		note.name[i] = spname[i];
	return note;
}
