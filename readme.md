# F22_CIS3210_Assignment3

- Student Name: Rehan Nagoor Mohideen
- Student ID: 1100592

## Description

Program to demonstrate the sending and receiving data over unconnected UDP.

## Requirements

Requires gcc

## Usage

run `$make` to compile the Sender and Receiver programs.

The code compiles asnd runs 2/10

### Sender

run `$./sender <receiver-IP-address> <filename>` to run the sender program.

sends teh data from teh file in 10 byte packets 5/10

### Receiver

run `$./receiver` to run the receiver program.
The receiver listens for data being sent to its port and prints it out with the
address and port that it is sent from.

prints the text as specified to stdout in the right order along with the address
and port number 8/10 and 9/10

Can handle multiple requests and does print in the right order from my testing. 10/10
