#ifndef _DAEMONLIB_H_
#define _DAEMONLIB_H_

#include <string>
#include <csignal>

typedef struct std_ioe_fd
{
	int in;
	int out;
	int err;
} StdIoeFd;

namespace spirit {

	static const std::string PID_FILE_PATH = "/var/run/spirit/"
	static const std::string LOG_FILE_PATH = "/var/log/spirit/"

	int write_pid_file(const char * sp_name);

	pid_t read_pid_file(const char * sp_name);

	int del_pid_file(const char * sp_name);

	int doublefork();

	StdIoeFd redirect_std(const char * name);

} // spirit

#endif //_DAEMONLIB_H_
