
#include "daemon_origin.h"

DaemonOrigin::DaemonOrigin(std::unique_ptr<SpiritBase> spirit_ptr) : d_spirit(spirit_ptr)
{
	d_cmd_map.emplace(std::string(SPIRIT_START_WORD), std::bind(&DaemonOrigin::start, this, std::placeholders::_1));
	d_cmd_map.emplace(std::string(SPIRIT_STOP_WORD), std::bind(&DaemonOrigin::stop, this, std::placeholders::_1));
	d_cmd_map.emplace(std::string(SPIRIT_RESTART_WORD), std::bind(&DaemonOrigin::restart, this, std::placeholders::_1));
	d_cmd_map.emplace(std::string(SPIRIT_CHECK_WORD), std::bind(&DaemonOrigin::check, this, std::placeholders::_1));

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
    // Check spirit
    if (d_spirit == nullptr) {
		printf("No spirit detected!\n");
		return -1;
	}
	// Get note
	d_note = spirit::get_note(d_spirit->get_role());

    return result;
}

void DaemonOrigin::sigint(int sigval)
{
	lock_guard<mutex> lg(d_alive_mutex);
	printf("SIGINT %d. Spirit %s will be destroyed\n", sigval, d_note.name);
	// destroy spirit here
	if (d_spirit)
		d_spirit->destroy_sp();
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

	// Check if running

	// Fork here
	result = spirit::doublefork();
	if (result != 0) {
		printf("Doublefork failed on step %d (fork|chdir|setsid|fork)\n", -1*result);
	}
	// Create log file
	if (spirit::check_create_folder(spirit::LOG_FILE_PATH) < 0)
		printf("Create log folder failed!\n");
	d_logfd = spirit::redirect_std(spirit::make_log_file_name(d_note.name));
	if (d_logfd < 0)
		printf("Redirect std failed!\n");
	// Write pid file
	if (spirit::check_create_folder(spirit::PID_FILE_PATH) < 0)
		printf("Create pid folder failed!\n");
	if (spirit::write_pid_file(spirit::make_pid_file_name(d_note.name)) < 0)
	{
		printf("Write pid file failed!\n");
		return -1;
	}
	// Run daemon here
	result = d_spirit->setup_sp(d_note);
	if (result != 0) {
		printf("Spirit %s setup failed with code %d!\n", d_note.name, result);
		return -1;
	}
	// Alive cycle. Main thread
	d_spirit->run();
	// Exit
	spirit::del_pid_file(spirit::make_pid_file_name(d_note.name));
	return 0;
}

int DaemonOrigin::stop(const std::string & args)
{
	// check pid, sigint, msg shutdown
	pid_t pid = read_pid_file(spirit::make_pid_file_name(d_note.name));
	if (pid < 0)
		return -1;
	// If stop failed - check if stopped - sig kill, del pid file
	return 0;
}

int DaemonOrigin::force_stop()
{
	pid_t pid = read_pid_file(spirit::make_pid_file_name(d_note.name));
	if (pid < 0) return -1;
	// Kill by name maybe?
	if (kill_by_pid(pid, SIGTERM) < 0) return -1;
	if (spirit::del_pid_file(spirit::make_pid_file_name(d_note.name)) < 0) return -1;
	return 0;
}

int DaemonOrigin::restart(const std::string & args)
{
	if (stop(args) == 0) {
		return start(args);
	}
	return -1;
}

int DaemonOrigin::check(const std::string & args)
{
	// Check pid
	bool pid_file_ok = false;
	if (read_pid_file(spirit::make_pid_file_name(d_note.name)) > 0) pid_file_ok = true;
	// read pid file, send state msg
	return 0;
}


