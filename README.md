Group Members: Yacob Mesfun

This project utilizes C++17 to compile the webserver. If you are using a standard install of the VMWare virtual machine, here is how you can get G++.

Steps:

sudo apt update

sudo apt install build-essential

To confirm you can build the software, check your version by entering G++ --version. If it's 9 or higher, you have C++17

MAKING SERVER:

Type in "make" to your terminal, the default build will build the server.

To run it, type ./webserver.

To run test cases, type in "make test".

you can run tests by doing ./test

TO CONNECT:

if you wish to locally connect, open a new terminal and type in curl -v http://localhost:8080.

If you wish to connect remotely, enter in something like ssh -L 9000:localhost:8080 vm_user@vm_address

TO VIEW SERVER METRICS:

"To view live server metrics, navigate to http://localhost:8080/stats in your browser."

LIMITATIONS:

This project has incredibly basic file handling, it can only handle GET requests, and very specific filesets, being html, png, jpeg, and css file extensions.

This project terminates the connection immediately, because the number of possible connections is bound to the number of threads, which forces us to close the TCP Connection, which is not standard in modern HTTP servers.

Currently, the server will just actively run. Because the main thread is the listening socket, ctrl+c is required to kill the program.

Currently, uses some basic overwriting of SIGINT to handle server termination. Still is messy and buggy, but good enough for now.