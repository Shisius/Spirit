
#include "shaman.h"

namespace spirit
{

	std::unique_ptr<Shaman> create_shaman(const SpiritNote & self_note, const SpiritNote & target_note)
	{
		switch (target_note.way)
		{
			case SPIRIT_FMT_WAY_IPCMQ:
				return std::make_unique<MqShaman>(self_note, target_note);
				break;
			default:
				return nullptr;
		}
	}

} // spirit

MqShaman::MqShaman(const SpiritNote & self_note, const SpiritNote & target_note) : Shaman(self_note, target_note)
{
	// Check note maybe?

	// Create mqreceiver
	std::string ansname = spirit::make_ansmqname(std::string(d_target_note.name), d_self_note.role);
	d_ansmq = std::make_unique<MqReceiver>(ansname, &MqShaman::mq_handler, this);

}

int MqShaman::req(unsigned char title, void * data, unsigned int size, unsigned int flags)
{
	// Pause receiver

	// Send

	// Timed receive

	// Resume receiver

	// Process answer

	// If wait - choose way
}

int MqShaman::mq_handler(SpiritMsg& smsg)
{

}
