## Group Members: Yacob Mesfun

This project utilizes C++17 to compile the webserver. If you are using a standard install of the VMWare virtual machine, here is how you can get G++.

### Steps:
```bash
sudo apt update

sudo apt install build-essential
```
To confirm you can build the software, check your version by entering G++ --version. If it's 9 or higher, you have C++17 and compiling the project is possible.

### MAKING SERVER:

1. Type in "make" to your terminal, the default build will build the server.

1. To run it, type ./webserver.

1. To run test cases, type in "make test".

1. you can run tests by doing ./test

### TO CONNECT:

1. if you wish to locally connect, open a new terminal and type in curl -v http://localhost:8080.

1. If you wish to connect remotely, enter in something like ssh -L 9000:localhost:8080 vm_user@vm_address

1. TO VIEW SERVER METRICS:

1. "To view live server metrics, navigate to http://localhost:8080/stats in your browser."

### LIMITATIONS:

1. This project has incredibly basic file handling, it can only handle GET requests, and very specific filesets, being html, png, jpeg, and css file extensions.

1. This project terminates the connection immediately, because the number of possible connections is bound to the number of threads, which forces us to close the TCP Connection, which is not standard in modern HTTP servers.

1. Currently, the server will just actively run. Because the main thread is the listening socket, ctrl+c is required to kill the program.

1. Currently, uses some basic overwriting of SIGINT to handle server termination. Still is messy and buggy, but good enough for now.
