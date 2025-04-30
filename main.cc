/*
 * Core includes
 * */
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

using namespace std;
/*
 * data layer
 * */
#include "./database.h"
/*
 * Tool Layer
 * */
#include "./snakes/netsnake.class.h"
#include "./snakes/fileSnake.class.h"
#include "./snakes/sqlSnake.class.h"
#include "./base64.class.h"
#include  "./permuter.class.h"

/*
 * Module Layer
 * */
#include "./modules/module.class.h"
#include "./modules/http-brute/http-brute.module.h"
#include "./modules/port-probe/port-probe.module.h"

/*
 * Application Layer
 * */
#include "./morningStarCache.class.h"
int main(int argc, char *argv[]){
	string start = "";
	if(argc > 1)
		start = argv[1];
	MorningStarCache cache(start);
	exit(EXIT_SUCCESS);
}
