# Task

This project is my implementation of a parallel non-blocking Lightweight directory access protocol server using BSD sockets network library. This program is implemented in the C++ language. The server does not support national characters and does not require authentication. Bytes reffering to authentication are ignored.

# Usage

**make**

**./myldap [-p port] -f file_path**

-p parameter sets the port for listening
-f declares the input .csv file

# Examples

**make**

Server side:

**./myldap -p 7588 -f input.csv**

Client side:

ldapsearch -h localhost -p 7588 -x uid="xb*"  
**ldapsearch -h localhost -p 7588 -x uid="xburza00"**  
**ldapsearch -h localhost -p 7588 -x cn="Bu*Ma*"**  
**ldapsearch -h localhost -p 7588 -x "(&(cn=Bu*Ma*)(uid=xburza*))"**  
**ldapsearch -h localhost -p 7588 -x "(|(cn=Bu*Ma*)(uid=xburza*))"**  
**ldapsearch -h localhost -p 7588 -x "(!(cn=Bu*Ma*))"**  


