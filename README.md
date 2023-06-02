# Linux Shell

This C++ program implements a simple linux shell that allows the user to execute commands. The shell performs tokenization to separate the command and its arguments, creates a child process, and uses the execvp system call to execute the command. It also supports input/output redirection for commands involving |, <, and > symbols.

### Some of Commands Supported
ls > file.txt

man fork | grep child > file.txt

command1 | command2 | command3 > file.txt

### License
This project is licensed under the MIT License. Feel free to modify and distribute it as needed.
