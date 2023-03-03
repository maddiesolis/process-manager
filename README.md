# Overview

This program is a simple command line process manager (PMan). It keeps a record of background processes started by the user (via the command line prompt) and allows the user to pause, restart, and terminate any running processes, as well as print information about process status.

Author: M. Solis de Ovando

# Compile and run instructions

Run the ulimit -u command in your command line:
```bash
ulimit -u 35
```
This is a precaution because fork() is used in the program.
You may want to set the `ulimit -u` to higher/lower number depending on the number of user processes you have allocated to you. To check the number of user processes, simply enter:
```bash
ulimit -u
```

To compile, type the following:
```bash
make
```

To run the program, type the following:
```bash
./pman
```

# Usage

Once you have compiled and run the program, you will be prompted with the following:
```bash
PMan: >
```
The following 6 commands can be used after this prompt.

## bg
Executes a new process. Some sample processes included are `./inf` and `./args`.
```bash
PMan: > bg ./process
```

## bglist
Lists every process running in the background along with their process ID (pid).
```bash
PMan: > bglist
```

## bgkill
Terminates process with given process ID (pid).
```bash
PMan: > bgkill pid
```

## bgstop
Temporarily stops executing process with given process ID (pid).
```bash
PMan: > bgstop pid
```

## bgstart
Restarts stopeed process with given process ID (pid).
```bash
PMan: > bgstart pid
```

## pstat
Prints out status of process with given process ID (pid).
```bash
PMan: > pstat pid
```