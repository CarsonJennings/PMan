CSC 360 Assignment 1

Notes: PMan is a mini process manager that uses linux system calls to allow the user to create and manage their own proecesses. This program is meant run on linux systems only.

Compilation:
	use the make command using a linux terminal to compile the program

Running:
	Use the ./PMan command to run the program.

Supported Commands:
	bg: The bg command is meant to execute a program in the current directory and run it in the background.
		Usage: enter the command bg followed by the program you would like to execute and any arguments that program takes. 
		(e.g. "bg foo arg1 arg2")
		Errors: If fork() fails the program will print "Failed to create a new process". If the program passed in after bg is not in the working directory or does not exist the program will prompt for user input again.

	bglist: the bglist command will print out a list of all proccess that are currently running or have been stopped.
		Usage: enter the command bglist.
			Example output:
				123:  ./foo
				456:  ./bar
				Total Background Jobs: 2
				Errors: None

	bgkill: The bgkill command will send a signal to terminate a given process
		Usage: enter the command bgkill followed by the pid you would like to kill (e.g. bgkill 456). The process will then be terminated and will no longer show in bglist
		Errors: If the pid does not exist the program will print out that the PID does not exist and prompt for user input.

	bgstop: The bgstop command will send a signal to stop a pid that is currently running.
		Usage: enter the command bgstop followed by the pid you would like to stop (e.g. "bgstop 123")
		Errors: If the pid does not exist the program will print out that the PID does not exist and prompt for user input.

	bstart: The bgstart command will send a signal to start a pid that had previously been stopped.
		Usage: enter the command bgstart followed by the pid you would like to start (e.g. "bgstart 123")
		Errors: If the pid does not exist the program will print out that the PID does not exist and prompt for user input.

	pstat: The pstat command will print out additional information about the process
		Usage: enter the command pstat followed by the pid you would like to get more information about (e.g. pstat 123)
			Example output:
				comm  -  (test)
				state -  S
				utime -  33
				stime -  17
				rss   -  144
				voluntary_ctxt_switches:        137080
				nonvoluntary_ctxt_switches:     19
		Errors: If the pid entered is not a valid pid then the program will print out "Error: Process [pid] does not exist" and prompt for user input.
