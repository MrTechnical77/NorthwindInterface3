Before compiling for the first time g++ must be installed and you must run the following command line in
your terminal to get the required header files to link C++ and mysql:

'sudo apt-get install libmysqlclient-dev'

You must also chage the third and fourth fields of thr mysql_real_connect function to match your
mysql user name and password.

If you do not already have a mysql user on your machine, run the following commands in your terminal to use defualt username and passwords:

$ sudo mysql
mysql> SET GLOBAL validate_password.length = 3;
mysql> SET GLOBAL validate_password.policy = "LOW";
mysql> create user 'griffin'@'localhost' identified by 'password';
mysql> GRANT ALL PRIVILEGES ON *.* TO 'griffin'@'localhost' WITH GRANT OPTION;
mysql> quit

After these first time setup steps are complete open a termial in the folder where the source code is stored and type make
to run the included MAKEFILE.

You are then able to run the program with command:

$ ./main