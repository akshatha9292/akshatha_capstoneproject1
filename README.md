Linux Multi-threaded Client-Server Using Shared Memory

This project implements a client-server system where the server generates random numbers and shares them with connected clients using shared memory. 
The server supports multiple clients simultaneously and uses semaphores to synchronize access to shared memory, ensuring data integrity.

The scope includes developing:
•	A server application that:
o	Listens for incoming client connections.
o	Handles multiple client connections concurrently using threads.
o	Uses shared memory to store messages from clients.
o	Uses semaphores to synchronize access to shared memory.
•	A client application that:
o	Connects to the server.
o	Sends messages to the server and receives responses.
o	Allows the user to quit the connection gracefully

Features
* Multi-threaded Server: The server can handle multiple clients concurrently.
* Shared Memory: Communication between the server and clients is facilitated through shared memory.
* Semaphores: Synchronization is achieved using semaphores to ensure data integrity in the shared memory.
* Logging: All interactions are logged to a file named test.sh.

Prerequisites
* A Linux-based system
* GCC compiler
* POSIX thread library
* POSIX semaphore library
  
Compilation
To compile the server and client programs, use the following commands:

gcc -pthread -o server server.c 
gcc -o client client.c 
chmod 777 test.sh
./server

ps-ef |grep server
pstree -p 304505
./test.sh


Compiling and Running
1.	Server Setup:
•	Compile and run the server code.
•	Ensure that the server is running and listening on the specified port.
2.	Client Setup:
•	Compile and run the client code.
•	Multiple clients can be launched using the provided test script.
•	The client will connect to the server and display the random numbers received from the server.
3.	Termination:
•	Terminate the server and clients using appropriate termination commands



Project Structure

* server.c: The server code, which handles incoming client connections, updates shared memory, and logs interactions.
* client.c: The client code, which connects to the server, sends messages, and reads responses.
* test.sh: The sh file where all interactions are recorded.

How It Works
Server Code
1.Socket Creation and Binding:
•	Create a socket and bind it to a specified port to listen for client connections.


2.Shared Memory and Semaphore:
•	Create and configure shared memory to store the random number.
•	Initialize a semaphore for synchronizing access to shared memory.
3.Thread Management:
•	One thread generates random numbers and stores them in shared memory.
•	Another thread handles incoming client connections, managing them in a loop and creating new threads for additional clients if needed.
Client Code
1.Socket Connection:
•	Create a socket and connect to the server.
2.Shared Memory and Semaphore:
•	Access the shared memory segment created by the server.
•	Use the semaphore for synchronized read access.
3.Display Data:
•	Continuously read and display the random number from shared memory.


Note-Ensure that the server is running before starting any clients.
To stop the server, use Ctrl+C.
To exit a client, type exit and press Enter.


Author
AKSHATHA D
