Before compiling for the first time g++ must be installed and you must run the following command line in
your terminal to get the required header files to link C++ and mysql:

'sudo apt-get install libmysqlclient-dev'

You must also chage the third and fourth fields of thr mysql_real_connect function to match your
mysql user name and password.

After these first time setup steps are complete open a termial in the folder where the source code is stored and type make
to run the included MAKEFILE.

You are then able to run the program with command:

$ ./main