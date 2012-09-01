#Erikbot-Makefile - Von Erik Fietz

erikbot: main.cpp IrcBot.cpp IrcBot.h
	g++ IrcBot.cpp main.cpp IrcBot.h -o erikbot
