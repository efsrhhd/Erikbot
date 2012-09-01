#ifndef IRCBOT_H_
#define IRCBOT_H_

class IrcBot
{
public:
	IrcBot(char * _nick, char * _usr);
	virtual ~IrcBot();

	bool setup;

	void start();
	bool charSearch(char *toSearch, char *searchFor);

private:
	char *port;
	int s;

	char *nick;
	char *usr;

	bool isConnected(char *buf);

	char * timeNow();

	bool sendData(char *msg);

	void sendPong(char *buf);

	void msgHandel(char *buf);
};

#endif
