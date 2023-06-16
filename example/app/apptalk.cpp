
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "ipcmqlib.h"
#include "iostream"

class Hep
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

int main()
{
	Hep hep;
	MqReceiver mq_recv(hep.d_name, &Hep::handler, &hep);

	return 0;
}
