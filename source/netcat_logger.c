#include <network.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "netcat_logger.h"

bool netcat_init = false;
s32 csock;
static char dest[1024];

void netcat_console()
{
	s32 ret;

	char localip[16] = {0};
	char gateway[16] = {0};
	char netmask[16] = {0};
	
	ret = if_config (localip, netmask, gateway, TRUE);
	if (ret >= 0)
	{
		s32 sock;
		u32	clientlen;
		struct sockaddr_in client;
		struct sockaddr_in server;
		//char temp[1026];
	
		clientlen = sizeof(client);

		sock = net_socket (AF_INET, SOCK_STREAM, IPPROTO_IP);
		
		memset (&server, 0, sizeof (server));
		memset (&client, 0, sizeof (client));

		server.sin_family = AF_INET;
		server.sin_port = htons (1337);
		server.sin_addr.s_addr = INADDR_ANY;
		net_bind (sock, (struct sockaddr *) &server, sizeof (server));
		
		net_listen( sock, 5);
		
		csock = net_accept (sock, (struct sockaddr *) &client, &clientlen);
		
		if ( csock < 0 ) {
			printf("Error connecting socket %d!\n", csock);
			while(1);
		}

		netcat_init = true;

		printf("Connecting port %d from %s\n", client.sin_port, inet_ntoa(client.sin_addr));
		netcat_log("Hi there.\n");
	}
}

void netcat_close()
{
	if (!netcat_init) return;
	net_close(csock);
}

void netcat_log(const char* data)
{
	if (!netcat_init) return;
	net_send(csock, data, strlen(data), 0);
}

void netcat_logf(const char* format, ...)
{
	if (!netcat_init) return;
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(dest, 1024, format, argptr);
    va_end(argptr);
    netcat_log(dest);
}
