#ifndef _DAEMON_ORIGIN_H_
#define _DAEMON_ORIGIN_H_

#include "spirit_base.h"
#include "daemoblib.h"

class DaemonOrigin
{
protected:

	std::unique_ptr<SpiritBase> d_spirit;
	SpiritNote d_note;
	std::mutex d_alive_mutex;
	int d_logfd;

	std::map<std::string, std::function<int(std::string)>> d_cmd_map;

	static std::function<void(int)> sigint_handler;
    static void sigint_routine(int sig_value, siginfo_t *info, void *data) { sigint_handler(info->si_int); }
    void sigint(int sigval);

public:

	DaemonOrigin(std::unique_ptr<SpiritBase> spirit_ptr);
	~DaemonOrigin();

	int run(int argc, char ** argv);
	int setup();

	void add_cmd(const std::string & name, std::function<int(std::string)> handler);

	int start(const std::string & args);
	int stop(const std::string & args);
	int restart(const std::string & args);
	int check(const std::string & args);
	void help();

	int send(SpiritMsg* smsg);
	int force_stop();

};

#endif //_DAEMON_ORIGIN_H_
