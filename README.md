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
