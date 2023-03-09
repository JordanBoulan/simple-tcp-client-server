/*
Author:Jordan Boulanger 
CopyLeft/Use for learning
*/


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "10545" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	if (argc != 2) {
	    fprintf(stderr,"usage:client username@hostname.com:port\n");
	    exit(1);
	}

	string connectionString = argv[1];
	if (connectionString.find('@') == -1 || connectionString.find(':') == -1){
		fprintf(stderr,"argument error! usage:client username@hostname.com:port\n");
	    exit(1);
	}
	string name = connectionString.substring(0, connectionString.find('@'));
	string host = connectionString.substring(connectionString.find('@')+1, connectionString.find(':'));
	string port = connectionString.substring(connectionString.find(':'), connectionString.length());

	std::cout << name << std::endl;
	std::cout << host << std::endl;
	std::cout << port << std::endl;

	exit(1);

	while (*cur != '\0'){
		while (*cur != '@' || *cur != '\0'){
			nameLength++;
			cur++;
		}
		name = new char[nameLength+1];
		for (int i = 0; i < nameLength+1; i++){
			name[i] = *start;
			start++;
		}
		name[nameLength] = '\0';
		cur++;
		

		while (*cur != ':' && *cur != '\0'){
			hostLength++;
			cur++;
		}
		host = new char[hostLength+1];
		for (int i = 0; i < hostLength+1; i++){
			host[i] = *start;
			start++;
		}
		host[hostLength] = '\0';

		while (*cur != '\0'){
			portLength++;
			cur++;
		}
		port = new char[portLength+1];
		for (int i = 0; i < portLength+1;i++){
			port[i] = *start;
			start++;
		}
		port[portLength] = '\0';
	


	}

	if (name == NULL || host == NULL || port == NULL){
	   perror("argument error! usage:client username@hostname.com:port \n")
	}


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {

			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure

	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	    perror("recv");
	    exit(1);
	}

	buf[numbytes] = '\0';

	printf("client: received '%s'\n",buf);

	close(sockfd);

	return 0;
}

