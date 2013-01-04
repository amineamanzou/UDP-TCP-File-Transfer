UDP and TCP File Transfer Program
=====================================================================

## Introduction
This project is a way for me to practice what i learned in university about unix administration and network.

The project is composed of two folder representing the two way to send a file using the local network.
User have to give args to the program to use it : IP Adress, Port, File to send.

## Prerequisites
A unix distribution and a C compiler : gcc.

## Installation
### Download and install the package
* Download zip file from gitHub [here](https://github.com/amineamanzou/UDP-TCP-File-Transfer)
* Unzip it where you want

> ### How to compile the applications (Quick mode !)
> 1. Navigate to the right folder UDP or FTP with a console.
> 2. Enter :
```bash
make 
```

## Ready !
You can now use the program client to send your file and server to receive the file.
### Example session
```bash
./client <IP-server> <port> <filetosend>
./server <port>
```
