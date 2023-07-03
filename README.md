# Spirit
Spirit - like systemd but more useful features available.

1. Spiritrole.h, spirit role folder
2. On start - read note files.
3. For everyone - check state, check,restart if state exists
4. If no state - start, then restart

6. use map for enum2word for states.

9. check ipc multiple connections
10. shaman can handle accepted, denied, other reqs
11. Role specs shamans
12. ASk for QUestion for ioctl - special bit in ans - shaman handle it as a req for spirit. maybe 2 ioctl types? Ask What do you need for 2 type.

14. ipc: spirit creates "role" mq for listening and opens "role_other" for answers.
16. ipc: shaman opens "role" mq for sendind and creates "role_other" for reading answers.
17. Consider use OneTimeIPC, EveryAnsOpenIPC, ContiniousIPC

# Done & remember

/etc/spirit.d - rules for start spirits
rule file contains:
1. roles. for every role - some description, available cmds.
2. start,stop,restart rules
3. pidfile?

generate role file in /var/spirit/role1

# TO DO
Event for state bit common title - one event. Need reload.
ans handler - what is it return?
!!!!!!!!daemon_tools.h ln82
send msg by cmd line
set state bit in atomic
universal tools for spirit msg consruction from title, object
handle event req: state.wait() in event thread. notify_all in set_state()
mqdata -> new -> smsg -> handler -> stack -> delete.
answer_data T -> 	-> new -> mqdata -> send -> delete
What about data ptr? Use T& better
use malloc vs new, use c
state as int (SpiritState)int, SpiritState | int?

MqReceiver - stop/start. Shaman can recv ans by itself or turn on receiver.
If WAIT - event handler can be set, default handler can be set, trash can be redir to handler by flag.

SpiritSystemUnit - to all ??
??Two types of spirit construction in daemonorigin

SpiritBase:
If smth blocked - set restart bit - master will restart the spirit
Setup: if note is not matched the spirit => return fail.

Log:
cout logger. Part of Spirit_Base. If you want use it - create in setup and use print_sp()? but how?
Check if printf is threadsafe.
Save old log to name.log.old

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
d_shaman.recv() -> switch (ANS_ID): do smth;
communication way depends on: type(ipc, ioctl...), special name(/dev/setdevm, ipc_name, rpmsg_name) - this name - in special file.
Shaman can wait the spirit for start
Shaman methods: 
recv callback(switch title, func(title, data* , size) of func(spmsg)) - for answers.
handle answers ACCEPTED, DENIED
Shaman start(setup), shaman stop(destroy)
Shaman contains MqReceivers and other stuff.
Only req and read the answers.
Req -> Wait for Accepted 
Blocking request - depends on spirit type (for ioctl for example)
Example: create_ins_shaman(INS_IPC_Shaman: IPCShaman, INSShaman)

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
1. Look through spirit.d dir and run the spirits
2. Check if spirit is alive and restart it, update spirit state
	If spirit doesn't answer, execute special_name check - if result is negative - restart
3. CMD Request_spirit_note(role)
4. create dir /var/spirit/state content: file like 01role1, content: 01 role1 state comm_type 0xff\n\r

SpiritState:
1. Available messages (option - short and long msg - depends of spirit msg data size)

Spirits:
FPGA as daemon - for manage, save adc, etc...
Logging in FPGA - start - choose adc,pprb,frame, choose event - start/stop, 1000adc/pulses, etc

# Issues:
2. RPMSG - double read
3. Test cv wait/pause

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
