#include <network.h>
#include <cstring>
#include <cstdio>
#include <cstdarg>

#include "netcat_logger.hpp"

bool Netcat::init = false;
s32 csock;
static char dest[1024];
static s32 sock;
static struct sockaddr_in client;
static struct sockaddr_in server;
static u32 clientlen;

void Netcat::console() {
	s32 ret;

	char localip[16] = {0};
	char gateway[16] = {0};
	char netmask[16] = {0};

	ret = if_config(localip, netmask, gateway, TRUE, 32);
	if (ret >= 0) {
		//char temp[1026];

		clientlen = sizeof(client);

		sock = net_socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

		memset(&server, 0, sizeof(server));
		memset(&client, 0, sizeof(client));

		server.sin_family = AF_INET;
		server.sin_port = htons(1337);
		server.sin_addr.s_addr = INADDR_ANY;
		net_bind(sock, (struct sockaddr*) &server, sizeof(server));

		net_listen(sock, 5);

		Netcat::init = true;
	}
}

void Netcat::accept() {
	if (!Netcat::init) return;
	csock = net_accept(sock, (struct sockaddr*) &client, &clientlen);

	if (csock < 0) {
		printf("Error connecting socket %d!\n", csock);
		while(1);
	}

	printf("Connecting port %d from %s\n", client.sin_port, inet_ntoa(client.sin_addr));
	Netcat::log("Hi there.\n");
}

void Netcat::close() {
	if (!Netcat::init) return;
	net_close(csock);
}

void Netcat::log(const char* data) {
	if (!Netcat::init) return;
	net_send(csock, data, strlen(data), 0);
}

void Netcat::logf(const char* format, ...) {
	if (!Netcat::init) return;
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(dest, 1024, format, argptr);
    va_end(argptr);
    Netcat::log(dest);
}
