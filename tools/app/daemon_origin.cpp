
#include "daemon_origin.h"

DaemonOrigin::DaemonOrigin()
{
	d_cmd_map.emplace(std::string(SPIRIT_START_WORD), std::bind(&DaemonOrigin::start, this, std::placeholders::_1));

	signal(SIGINT, &DaemonOrigin::sigint);
}

int DaemonOrigin::setup()
{
	int result;

	// Attach SIGINT
	struct sigaction _sigaction;
    _sigaction.sa_flags = SA_SIGINFO;
    DaemonOrigin::sigint_handler = std::bind(&DaemonOrigin::sigint, this, std::placeholders::_1);
    _sigaction.sa_sigaction = DaemonOrigin::sigint_routine;
    result = sigaction(SIGINT, &kocomm_sigaction, NULL);

    return result;
}

void DaemonOrigin::sigint(int sigval)
{

}

void DaemonOrigin::help()
{
	printf("Usage:");
	for (auto & p : d_cmd_map) {
		printf("%s|", p.first);
	}
	printf("\n");
}

int DaemonOrigin::run(int argc, char ** argv)
{
	std::string cmd = "";
	std::string arg = "";

	if (argc > 1) {
		cmd = std::string(argv[1]);
		if (argc > 2) 
			arg = std::string(argv[2]);
		if (d_cmd_map.count(cmd) > 0) {
			return d_cmd_map.at(cmd)(arg);
		}
	}
	help(); 
	return -1;
}

int DaemonOrigin::start(const std::string & args)
{
	// Run daemon here
	d_spirit = std::make_unique<SpiritT>(args);
	return 0;
}
