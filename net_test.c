#include <arpa/inet.h>
#include <netdb.h>
//#include <netif.h>
//#include <wnet.h>
//#include <ppp.h>
#include <signal.h>
#include <errno.h>
#include <net/if.h>


void tcp_init(void)
{
	static int inited = 0;
	if (!inited){
		signal(SIGPIPE, SIG_IGN);
		inited = 1;
	}
}

void tcp_close(int isocket)
{
	if (isocket >=0)
		close(isocket);
}

int tcp_connect(const char * pServerAddr, unsigned int port)
{
	int clientsocket = -1;
	struct sockaddr_in addr;
	struct hostent *pHE;
	tcp_init();
	clientsocket = socket(AF_INET,SOCK_STREAM,0);
	if (-1 != clientsocket)
	{
		memset(&addr, 0, sizeof(addr));
		addr.sin_addr.s_addr=inet_addr(pServerAddr);
		if (INADDR_NONE == addr.sin_addr.s_addr)
		{
			pHE=gethostbyname(pServerAddr);
			if(pHE)
			{
				addr.sin_addr.s_addr = *(u_long *) pHE->h_addr_list[0];
				printf("\r\n+++++++%s:%08X\r\n", pServerAddr, addr.sin_addr.s_addr);
			}
		}

		addr.sin_family=AF_INET;
		addr.sin_port=htons(port);

		if(connect(clientsocket,(struct sockaddr *)&addr, sizeof(addr))==0)
		{
			printf("connect ok!\r\n");
		}
		else
		{
			tcp_close(clientsocket);
			clientsocket = -1;
		}
	}
	return clientsocket;
}


void main()
{
 tcp_connect("www.baidu.com", 80);
}




