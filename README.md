# Spirit
Spirit - like systemd but more useful features available.

1. Spiritrole.h, spirit role folder
2. On start - read note files.
3. For everyone - check state, check,restart if state exists
4. If no state - start, then restart
5. Shaman req, ans
6. use map for enum2word for states.

7. Spirit can create shamans and has its own ... - part of spirit.
8. Spiritd define note - for connect to spiritd.
9. check ipc multiple connections
10. shaman can handle accepted, denied, other reqs
11. Role specs shamans
12. ASk for QUestion for ioctl - special bit in ans - shaman handle it as a req for spirit. maybe 2 ioctl types? Ask What do you need for 2 type.

13. role2str
14. ipc: spirit creates "role" mq for listening and opens "role_other" for answers.
15. spirit_ipc lib
16. ipc: shaman opens "role" mq for sendind and creates "role_other" for reading answers.
17. Consider use OneTimeIPC, EveryAnsOpenIPC, ContiniousIPC

# Done & remember

/etc/spirit.d - rules for start spirits
rule file contains:
1. roles. for every role - some description, available cmds.
2. start,stop,restart rules
3. pidfile

generate role file in /var/spirit/role1

# TO DO

DaemonOrigin:
daemonlib: doublefork, check running, redirect std
???? What is the name in this funcs?
Main - maybe template function?:
	parse arg - maybe map<string, function>, d_cmd_map, add_cmd():
		Start:
			Create Spirit, setup, run, create sigint handler
		Stop:
			Send sigint 
		Check:
			Check if running
		Restart:

int main(int argc, char ** argv)
{
	Daemon<MySpirit> d(args); // d = make_unique<MySpirit>(args);
	return Daemon.run(argc, argv);
}
????????????? How to get note without spirit construction? Static function maybe?


SpiritBase:

Log:
cout logger. Part of Spirit_Base. If you want use it - create in setup and use print_sp()? but how?
Check if printf is threadsafe.
```
printf(const char *fmt, ...)
{
	va_list	listp;

	disable_preemption();
	va_start(listp, fmt);
	_doprnt(fmt, &listp, conslog_putc, 16);
	va_end(listp);
	enable_preemption();
}
```

Shaman:
dynamic shaman - abstract class
spiritd helps other spirits to create shamans (Spirit note).
Shaman_t d_shaman = create_shaman(ROLE_ID);
d_shaman.send(REQ_ID, data);
d_shaman.recv() -> switch (ANS_ID): do smth;
communication way depends of: type(ipc, ioctl...), special name(/dev/setdevm, ipc_name, rpmsg_name) - this name - in special file.
Shaman can wait the spirit for start
Shaman methods: send_spirit_msg(TITLE, data, size), 
recv callback(switch title, func(title, data* , size) of func(spmsg)) - for answers.
handle answers ACCEPTED, DENIED
get state
Shaman start(setup), shaman stop(destroy)

spirit_lib
Install functions
1. create dir /etc/spirit.d, subdirs

Spiritlib contains
1. Spirit master daemon
2. Spirit systemd rule

3. Spirit template
	- communication tool like IPC, ioctl, etc depends on core
	- spirit msg construct/parse rule, functions
	- spirit special msg handling

SpiritMaster can:
1. Look through spirit.d lib and run the spirits
2. Check if spirit is alive and restart it, update spirit state
	If spirit doesn't answer, execute special_name check - if result is negative - restart
3. CMD Request_spirit_note(role)
4. create dir /var/spirit/state content: file like 01role1, content: 01 role1 state comm_type 0xff\n\r

SpiritState:
1. Available messages (option - short and long msg - depends of spirit msg data size)

# Issues:
1. IOCTL - transfer spirit message with void* data, copy_from_user(spirit_msg.data)
2. RPMSG - double read

# Questions

Enable/Disamble spirits - use symlinks like systemd?

# Future readme

To install spirit you need:
1. Create spirit driver/daemon/firmware and install it in system as you want
2. Create app in /usr/bin with name=special_name and command line interface start/stop/restart/check
3. Create SpiritNote in /etc/spirit.d folder

# Hmmm

Runtime functions
2. for every role in spirit.d -> create /var/spirit/roleX file content:
	roleID, special name, comm_type, msg_hex_word
4. create dir /var/spirit/state content: file like 01role1, content: 01 role1 state comm_type 0xff\n\r
