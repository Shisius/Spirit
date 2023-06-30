
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "daemon_origin.h"
#include "iostream"

class Hep : public SpiritBase
{

public:
	std::string d_name;
	Hep() {d_name = "hep";}
	~Hep() {}
	int handler(SpiritMsg& spmsg)
	{
		return 0;
	}
};

std::unique_ptr<SpiritBase> spirit_obj;

int main()
{
	spirit_obj = std::make_unique<Hep>();
	DaemonOrigin daemon_origin(spirit_obj);
	if (daemon_origin.setup() != 0) {
		printf("DaemonOrigin setup failed!\n");
		return -1;
	}
	return daemon_origin.run(argc, argv);
}
