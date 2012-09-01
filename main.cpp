#include <iostream>
#include "IrcBot.h"

using namespace std;

int main()
{
	IrcBot bot = IrcBot("NICK Erikbot\r\n","USER guest server1 server2 :Robby Roboter\r\n");
	bot.start();

	return 0;
}
