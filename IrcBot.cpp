#include "IrcBot.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

using namespace std;

#define MAXDATASIZE 100

IrcBot::IrcBot(char * _nick, char * _usr)
{
	nick = _nick;
	usr = _usr;
}

IrcBot::~IrcBot()
{
	close (s);
}

void IrcBot::start()
{
	string connectserver;

	cout<<"Bitte gewünschten Server eingeben:\n";
	cin>>connectserver;
	cout<<"Verbinde zu Server: "<<connectserver<<"\n";

	struct addrinfo hints, *servinfo;

	setup = true;

	port = "6667";

	memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int res;
	if ((res = getaddrinfo(connectserver.c_str(),port,&hints,&servinfo)) != 0)
	{
		setup = false;
		fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(res));
	}


	if ((s = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol)) == -1)
	{
		perror("client: socket");
	}

	if (connect(s,servinfo->ai_addr, servinfo->ai_addrlen) == -1)
	{
		close (s);
		perror("Client Connect");
	}

	freeaddrinfo(servinfo);


	int numbytes;
	char buf[MAXDATASIZE];

	int count = 0;
	while (1)
	{
		count++;

		switch (count) {
			case 3:
					sendData(nick);
					sendData(usr);
				break;
			case 4:
				sendData("JOIN #Erikbot\r\n");
			default:
				break;
	}



		numbytes = recv(s,buf,MAXDATASIZE-1,0);
		buf[numbytes]='\0';
		cout << buf;

		msgHandel(buf);


		if (charSearch(buf,"PING"))
		{
			sendPong(buf);
		}

		if (numbytes==0)
		{
			cout << "----------------------CONNECTION CLOSED---------------------------"<< endl;
			cout << timeNow() << endl;

			break;
		}
	}
}


bool IrcBot::charSearch(char *toSearch, char *searchFor)
{
	int len = strlen(toSearch);
	int forLen = strlen(searchFor);

	for (int i = 0; i < len;i++)
	{
		if (searchFor[0] == toSearch[i])
		{
			bool found = true;
			for (int x = 1; x < forLen; x++)
			{
				if (toSearch[i+x]!=searchFor[x])
				{
					found = false;
				}
			}

			if (found == true)
				return true;
		}
	}

	return 0;
}

bool IrcBot::isConnected(char *buf)
{
	if (charSearch(buf,"/MOTD") == true)
		return true;
	else
		return false;
}

char * IrcBot::timeNow()
{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	return asctime (timeinfo);
}


bool IrcBot::sendData(char *msg)
{
	int len = strlen(msg);
	int bytes_sent = send(s,msg,len,0);

	if (bytes_sent == 0)
		return false;
	else
		return true;
}

void IrcBot::sendPong(char *buf)
{

	char * toSearch = "PING ";

	for (int i = 0; i < strlen(buf);i++)
		{
			if (buf[i] == toSearch[0])
			{
				bool found = true;
				for (int x = 1; x < 4; x++)
				{
					if (buf[i+x]!=toSearch[x])
					{
						found = false;
					}
				}

				if (found == true)
				{
					int count = 0;
					for (int x = (i+strlen(toSearch)); x < strlen(buf);x++)
					{
						count++;
					}

					char returnHost[count + 5];
					returnHost[0]='P';
					returnHost[1]='O';
					returnHost[2]='N';
					returnHost[3]='G';
					returnHost[4]=' ';


					count = 0;
					for (int x = (i+strlen(toSearch)); x < strlen(buf);x++)
					{
						returnHost[count+5]=buf[x];
						count++;
					}

					if (sendData(returnHost))
					{
						cout << timeNow() <<"  Ping Pong" << endl;
					}


					return;
				}
			}
		}

}

void IrcBot::msgHandel(char * buf)
{

	if (charSearch(buf,"hi erikbot"))
	{
		sendData("PRIVMSG #erikbot :hi :)\r\n");
	}
}

