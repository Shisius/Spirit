/**
 * Spiritd main
 * 
 */

#include "spiritd.h"

std::unique_ptr<SpiritBase> spirit_obj;

int main(int argc, char ** argv)
{
	spirit_obj = std::make_unique<SpiritMaster>();
	DaemonOrigin daemon_origin(spirit_obj);
	if (daemon_origin.setup() != 0) {
		printf("DaemonOrigin setup failed!\n");
		return -1;
	}
	return daemon_origin.run(argc, argv);
}
