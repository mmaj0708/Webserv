# Welcome to our webserv repository 👋

The objective of Webserv is to write an HTTP server in C++ 98. <br />
Here you will learn more about :
- [HTTP protocol](#what-is-HTTP)
- [Select, poll, epoll, kqueue](#technical-choice)
- [Socket programming (server + client)](#socket-programming)

# What is HTTP

```
HTTP is a [client-server] protocol for fetching resources such as HTML documents. 
It is the foundation of any data exchange on the Web and it is a client-server protocol, 
which means requests are initiated by the recipient, usually the Web browser.
```

🖇 [Reference](https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview)

Requests will be sent by a client (the web browser) to a server which will gives an answer called response.
The HTTP protocol should be reliable and connexion-oriented. Therefore our webserv will be based on the TCP standard (=/= UCP standard).

## Differences between HTTP protocols

Version|Behavior
-----|------
HTTP/1.0|opens a separate TCP connection for each HTTP request/response pair ⚠️ less efficient than sharing a single TCP connection when multiple requests are sent
HTTP/1.1|introduces pipelining and persistent connections. HTTP messages are human-readable.
HTTP/2|multiplexed messages over a single connection. Connection is kept warm and efficient. HTTP messages are not human-readable because of its binary structure.

## How does it work

#### 1) The client asks the server to open a connexion to send one or several requests and receive answers. It can be:
* an new connexion
* an existing one that has been previously opened
#### 2) The client sends a request to the server
#### 3) The server sends an answer to the client
#### 4) The connexion is closed or will be reused for future requests

## Request & answer's structure

#### 1) HTTP request

The request contains:

* The HTTP method. For our project, only the GET, POST and DELETE methods will be coded.

Method|Behavior
-----|------
GET| Requests data from a specified resource (for instance if you want to get srcs/file.html ressource)
POST| Sends data to a server to create/update a resource
DELETE| Deletes the specified resource

* The path of the ressource

* The version of the HTTP protocol (it will be HTTP/1.1 for our project)

* Optional headers : languages, host, cache-control...

* A body that will contain the ressource sent

![Alt text](/docs/request.png?raw=true "Request example")

#### 2) HTTP response

The response contains:

* The version of the HTTP protocol

* The status code. It will indicate whether the request was successful or not

* The status message. It is a short description of the status code's meaning

* The HTTP headers

* A body (optional) containing the fetched resource

![Alt text](/docs/answer.png?raw=true "Answer example")

I found these examples [here](https://slideplayer.com/slide/5320369/).

🖇 [Reference](https://www.w3schools.com/tags/ref_httpmethods.asp)

# Technical choice

Function|Description & OS
-----|------
select| there are as many waiting lists as there are sockets. Works on Linux & MacOS
poll| the socket itself has its own waiting list. The socket has to be added and deleted only once (=/= select). Works on MacOS
epoll| the socket itself has its own waiting list. The socket has to be added and deleted only once (=/= select). Works on Linux
kqueue| we didn't find a lot of documentation about kqueue. Works on FreeBSD

For our project, we have chosen to work on MacOS and to use poll because it seemed:
* more optimised and quicker
* easier to use
* provided with documentation

You really should read [this](https://stackoverflow.com/questions/17355593/why-is-epoll-faster-than-select/17355702) if you want to better understand our choice.

# Socket programming

```
Socket programming is a way of connecting two nodes on a network to communicate with each other. 
One socket(node) listens on a particular port at an IP, while other socket reaches out 
to the other to form a connection.
```

🖇 [Reference](https://www.geeksforgeeks.org/socket-programming-cc/)

Here, you will learn how to create:
* your server socket
* your client socket
* how to make them communicate

## Server socket

First of all, you have to include
:
`````
#include <sys/types.h>
#include <sys/socket.h>
`````

Then, you have to complete the following steps:

#### 1) Socket creation

````
int sockfd = socket(int domain, int type, int protocol);
`````

It will create a socket descriptor (sockfd) that will be a communication point.

Variable|Type|Description
-----|------|------
domain| int | Communication domain
type| int | Communication type
protocol| int | Protocol value (for IP -> 0)

Return value:
* a descriptor referencing the socket
* -1 if an error occurs

#### 2) Setsockopt (optional)

````
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
````

Helps in reuse of address and port. Prevents error such as: “address already in use”.

Variable|Type|Description
-----|------|------
sockfd| int | The socket descriptor created
level| int | Protocol level
optname| int | Option to set
optval| const void * | Identifies a buffer in which the value for the requested option is to be returned
optlen| socklen_t | The size of the optval buffer

Return value:
* 0 if it's a success
* -1 if an error occurs

#### 3) Bind

````
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
````

It will give a name to the socket previously created.

Variable|Type|Description
-----|------|------
sockfd| int | The socket descriptor created
addr| const struct sockaddr * | Cast the structure pointer passed in addr in order to avoid compiler warnings
addrlen| socklen_t | The size in bytes of the address structure addr

Return value:
* 0 if it's a success
* -1 if an error occurs

#### 4) Listen

````
int listen(int sockfd, int backlog);
````

Waiting for a connexion on the socket.

Variable|Type|Description
-----|------|------
sockfd| int | The socket descriptor created
backlog| int | The maximum number of outstanding connections in the socket's listen queue

Return value:
* 0 if it's a success
* -1 if an error occurs

#### 5) Accept

````
int new_socket = accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
````

Accept a connexion on a socket.

Variable|Type|Description
-----|------|------
sockfd| int | The socket descriptor created
addr| struct sockaddr * | Cast the structure pointer passed in addr in order to avoid compiler warnings
addrlen| socklen_t | The size in bytes of the address structure addr

Return value:
* a file descriptor for the accepted socket (= non-negative integer)
* -1 if an error occurs

## Client socket

#### 1) Create the client socket

#### 2) Connect the client socket to the server

#### 3) Send the response

## 🖇 Reference
- [An overview of HTTP](https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview)
- [Socket Programming in C/C++](https://www.geeksforgeeks.org/socket-programming-cc/)
- [HTTP Server: Everything you need to know to Build a simple HTTP server from scratch](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
- [How nginx processes a request](http://nginx.org/en/docs/http/request_processing.html)
- [Documentation Webserv](https://www.notion.so/Documentation-Webserv-320727979ffd4176a7dd5ba41aaadf46)
- [Building a Web Server in C++ | Part 1](https://www.youtube.com/watch?v=Kc1kwm1WyVM&ab_channel=SloanKelly)
- [Building a Web Server in C++ | Part 2](https://www.youtube.com/watch?v=YqEqjODUkWY&ab_channel=SloanKelly)
- [NGINX vs. Apache : comparaison des architectures et des possibilités de configuration et d’extension](https://www.ionos.fr/digitalguide/serveur/know-how/nginx-vs-apache/)
- [Simple C example of doing an HTTP POST and consuming the response](https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response/22135885#22135885)
- [How to build a web client? (sockets)](https://www.youtube.com/watch?v=bdIiTxtMaKA&list=PL9IEJIKnBJjH_zM5LnovnoaKlXML5qh17&index=2&ab_channel=JacobSorber)
- [Program your own web server in C. (sockets) ](https://www.youtube.com/watch?v=esXw4bdaZkc)
- [Why nginx is faster than Apache, and why you needn't necessarily care — Django deployment](https://djangodeployment.com/2016/11/15/why-nginx-is-faster-than-apache-and-why-you-neednt-necessarily-care/)
