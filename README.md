# brobash
Team brOS's implementation of a shell.
***

### Features
* Execute commands provided on your system.
  * `firefox` opens firefox in a new process. 
* Supports piping multiple commands.
  * `ls | grep .c | grep bb` retrieve all `.c` files which contain `bb` as part of their filename in the current directory.
* Supports IO redirection.
  * `sort < input.txt > output.txt` reads input.txt, sorts it, and saves it to output.txt.
* Multiple piping with multiple redirection is also supported
  * `cat < file1 | sort > file2 | ls > file3` retrieves contents of file1 then sorts and outputs to file2 and finally ls outputs to file3  
* History functionality.
  * `!!` to execute the most recent command.
  * `! n` to execute the nth command in the history.
  * `@@ n` to execute the nth recent command.
  * `??` to view the most recent command.
  * `history` to view the command history list.
* Supports many built-in commands.
  * `type exit cd` to find the types of specified commands. 
  * `cd directory` to change current working directory to specified directory.
* Capable of taking inputs from a file.
  * `./brobash input.txt` will run all the commands in the input file.
* Provides basic init file which runs before the terminal starts.
* Provides basic configuration file to edit shell properties.

### Install
* brobash is a standalone program.
* Go to brobash directory, type `make brobash`.

### How To Run
* Run brobash by typing `./brobash`.
* You can also add brobash to your Enivronment path to make the command `brobash` available globally.

### License
The code is released under The MIT License (MIT).
To view the terms of the license refer [this](https://opensource.org/licenses/MIT).

### Error and difficults
* Not all memory leaks were handled
* Parsing the command was tedious, strtok used a static pointer which was not known to us.

### Contributions
* Abhinav Singh - " I handled parsing the command and taking file inputs for the bash "
* Suhas. N - " I implemented multiple piping with multiple io redirection "
* Aditya Kanthale - " I created history :) "
* Ashish Singh - " I made builtin commands "
