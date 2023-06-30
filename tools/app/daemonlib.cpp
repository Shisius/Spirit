
#include "daemonlib.h"

namespace spirit {

	int check_create_folder(const char * path)
	{
		struct stat dir_stat;
		if (stat(path, &dir_stat) == 0)
			if (S_ISDIR(dir_stat.st_mode))
				return 0;
		// Create. Mode - rwx for user, group, others.
		if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) == 0)
			return 0;
		return -1;
	}

	const char * make_pid_file_name(const char * sp_name)
	{
		return (PID_FILE_PATH + std::string(sp_name) + ".pid").c_str();
	}
	const char * make_log_file_name(const char * sp_name)
	{
		return (LOG_FILE_PATH + std::string(sp_name) + ".log").c_str();
	}

	int write_pid_file(const char * full_path)
	{
		// Get pid str
		pid_t pid = getpid();
		char pid_s[8];
		sprintf(pid_s, "%d\n", pid);
		// Open file
		FILE* f = fopen(full_path, "w");
		if (f == NULL) return -1;
		// Write PID and \n
		size_t n_bytes = fwrite(pid_s, sizeof(char), strlen(pid_s), f);
		// Close file and check result
		fclose(f);
		if (n_bytes == strlen(pid_s)) return 0;
		return -1;
	}

	pid_t read_pid_file(const char * full_path)
	{
		FILE* f = fopen(full_path, "r");
		if (f == NULL) return -1;
		// Get content
		char pid_s[8];
		size_t n_bytes = fread(pid_s, sizeof(char), 8, f);
		fclose(f);
		if (n_bytes <= 0) return -1;
		// Read PID
		pid_t pid = atoi(pid_s);
		if (pid == 0) return -1;
		return pid;
	}

	int del_pid_file(const char * full_path)
	{
		return remove(full_path);
	}

	int kill_by_pid(pid_t pid, int sig_no)
	{
		return kill(pid, sig_no);
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
		// Open log
		int flags = O_RDWR | O_APPEND;
		int new_fd = open(full_path, flags);
		if (new_fd < 0) return new_fd;
		// Stdin
		close(STDIN_FILENO);
		// Stdout
		fsync(STDOUT_FILENO);
		close(STDOUT_FILENO);
		if (dup2(new_fd, STDOUT_FILENO) < 0) return -1;
		// Stderr
		fsync(STDERR_FILENO);
		close(STDERR_FILENO);
		if (dup2(new_fd, STDERR_FILENO) < 0) return -1;	
		return new_fd;
	}

} // spirit
