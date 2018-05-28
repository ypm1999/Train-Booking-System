# KitJudge
A functional online judge running on 64 bit linux operating systems.

## Sandbox
KitJudge's sandbox is based on ptrace function, which is developped on the sandbox of [Universal Online Judge]( https://github.com/vfleaking/uoj).


### For users
#### Functions
* Execute a program safely, including banning several dangerous system calls;
* Measure the running time and memory the program used.

#### Changes
* Fixed several bugs including completly killing all subprocess when the main executor subprocess ended;
* Add some additional explanations about the verdict results, such like "SIGFPE: Erroneous arithmetic operation";

#### Usage
Just running the command `pbsbox --help` to obtain the help documents.

### For developers
#### File filiters
The file filiter is used to let the sandbox know whether or not it should let the program access/read/write the file. There are rules:
* `/home`: Only the directory /home is accepted. Anything include `/home/1.txt` is rejected;
* `/home/`: Only accept the files or directories under `/home`(e.g. the file `/home/1.txt` is accepted but `/home/me/1.txt` is rejected);
* `/home/*`: Every files in the directory `/home` is accepted, namely `/home/me/1.txt` is accepted.

#### Compile command
Normally, you can only execute the command `make` to obtain the executable file.
But there are two substitutions you can choose to show the debug message:
* `-DDEBUG1`: Show the basic message such as the subprocess pid and the argument parser's results.
* `-DDEBUG2`: Show the detailed message such as the syscalls.
* `-DCOLORLOG`: Show the colorful logs on linux terminal.
* `-DCOLOREXCEPTION`: Show the colorful exceptions on linux terminal.
