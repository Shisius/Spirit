#ifndef _DAEMON_ORIGIN_H_
#define _DAEMON_ORIGIN_H_

#include "spirit_base.h"
#include "daemoblib.h"

template<class SpiritT>
class DaemonOrigin
{
protected:

	std::unique_ptr<SpiritBase> d_spirit;

	std::map<std::string, std::function<int(std::string)>> d_cmd_map;

	static std::function<void(int)> sigint_handler;
    static void sigint_routine(int sig_value, siginfo_t *info, void *data) { sigint_handler(info->si_int); }
    void sigint(int sigval);

public:

	DaemonOrigin();
	~DaemonOrigin();

	int run(int argc, char ** argv);
	int setup();

	//void add_cmd();

	int start(const std::string & args);
	int stop(const std::string & args);
	int restart(const std::string & args);
	int check(const std::string & args);
	void help();

};

#endif //_DAEMON_ORIGIN_H_
