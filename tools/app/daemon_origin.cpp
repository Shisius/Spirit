
#include "daemon_origin.h"

DaemonOrigin::DaemonOrigin(std::unique_ptr<SpiritBase> spirit_ptr) : d_spirit(spirit_ptr)
{
	d_cmd_map.emplace(std::string(SPIRIT_START_WORD), std::bind(&DaemonOrigin::start, this, std::placeholders::_1));

	signal(SIGINT, &DaemonOrigin::sigint);
}

DaemonOrigin::~DaemonOrigin()
{
	;
}

int DaemonOrigin::setup()
{
	int result = 0;

	// Attach SIGINT
	struct sigaction _sigaction;
    _sigaction.sa_flags = SA_SIGINFO;
    DaemonOrigin::sigint_handler = std::bind(&DaemonOrigin::sigint, this, std::placeholders::_1);
    _sigaction.sa_sigaction = DaemonOrigin::sigint_routine;
    result = sigaction(SIGINT, &kocomm_sigaction, NULL);
    if (result < 0) {
    	printf("Attach signal failed\n");
    	return result;
    }

    if (d_spirit == nullptr) {
		printf("No spirit detected!\n");
		return -1;
	}

	d_note = d_spirit->get_note();

    return result;
}

void DaemonOrigin::sigint(int sigval)
{
	// destroy spirit here
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
	return 0;
}

int DaemonOrigin::start(const std::string & args)
{
	int result;

	// Fork here

	// Run daemon here
	
	result = d_spirit->setup(args);
	if (result != 0) {
		printf("Spirit %s setup failed with code %d!\n", d_note.name, result);
		return -1;
	}


	// Exit
	spirit::del_pid_file((PID_FILE_PATH + std::string(d_note.name)).c_str());
	return 0;
}

int DaemonOrigin::stop(const std::string & args)
{
	// check pid, sigint, msg shutdown
	return 0;
}