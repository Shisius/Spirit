#ifndef _DAEMONLIB_H_
#define _DAEMONLIB_H_

#include <string>
#include <csignal>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace spirit {

	static const std::string PID_FILE_PATH = "/var/run/spirit/";
	static const std::string LOG_FILE_PATH = "/var/log/spirit/";

	int check_create_folder(const char * path);
	const char * make_pid_file_name(const char * sp_name);
	const char * make_log_file_name(const char * sp_name);

	int write_pid_file(const char * full_path);

	pid_t read_pid_file(const char * full_path);

	int del_pid_file(const char * full_path);

	int kill_by_pid(pid_t pid, int sig_no);

	int doublefork();

	int redirect_std(const char * full_path);

} // spirit

#endif //_DAEMONLIB_H_
