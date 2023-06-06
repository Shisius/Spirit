
#include "daemonlib.h"

namespace spirit {

	int write_pid_file(const char * full_path)
	{

	}

	pid_t read_pid_file(const char * full_path)
	{

	}

	int del_pid_file(const char * full_path)
	{

	}

	int kill_by_pid(pid_t pid, int sig_no)
	{

	}

	int doublefork()
	{
		// First fork
		pid_t pid = fork();
		if (pid < 0) return -1;
		if (pid > 0) exit(0);

		// Reset environment
		if ((chdir("/")) < 0) return -2;
		pid_t sid = setsid();
		if (sid < 0) return -3;
	    umask(0);

	    // Second fork
	    pid = fork();
		if (pid < 0) return -4;
		if (pid > 0) exit(0);

		// Done
		return 0;
	}

	int redirect_std(const char * full_path)
	{

	}

} // spirit
