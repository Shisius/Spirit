/**
 * Spiritd main
 * 
 */

#include "spiritd.h"

int main(int argc, char ** argv)
{
	DaemonOrigin<SpiritMaster> daemon_origin;
	if (daemon_origin.setup() != 0) {
		printf("DaemonOrigin setup failed!\n");
		return -1;
	}
	return daemon_origin.run(argc, argv);
}
