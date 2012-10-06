C-Shell
=======
This is a simple shell written in C, created for Operating Systems lab at Colgate University.

![screenshot.png](https://raw.github.com/swiecki/C-Shell/master/screenshot.png "Screenshot")

Features
--------

Built in commands:

To quit the shell, the user can type `exit`.

The shell runs in two different modes: sequential and parallel. In sequential mode, when multiple jobs are listed on a single command line, they are run one at a time to completion, in left-to right order. So, for the example above (/bin/ls ; /bin/ps), first /bin/ls should run to completion, then /bin/ps. In contrast, in parallel mode, all jobs are started in rapid succession. To switch between modes, type `mode p` or `mode s`. To display current mode, type `mode`.

There is PATH-like capability available: any paths for execution can be listed in shell-config. Note that these are only loaded upon running, and not dynamically updated upon every command.

The following 3 commands are specific to parallel mode:

`jobs` - prints out a list of all processes that are currently running in the background, including PID, command, and state
`pause PID` - sends a signal to the background child process with process ID [PID] in order to pause the process.
`resume PID` - sends a signal to a background child process that has been paused in order to restart it.
