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

#include "./classes/btApplication.class.h"

/*
 * Tool Layer
 * */
#include "./classes/netsnake.class.h"
#include "./classes/fileSnake.class.h"
#include "./classes/base64.class.h"
#include  "./classes/permuter.class.h"

/*
 * Module Layer
 * */
#include "./modules/module.class.h"
#include "./modules/http-brute/http-brute.module.h"

/*
 * Application Layer
 * */
#include "./classes/morningStarCache.class.h"
int main(void){
	MorningStarCache cache;
	exit(EXIT_SUCCESS);
}
