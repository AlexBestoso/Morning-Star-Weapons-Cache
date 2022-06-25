#define WORDLIST_MAX 5
#define HEADERS_MAX 10
#define CONTENT_MAX 500000
#define MAX_NET_BUFFER 100000

struct CommandPacket{
	size_t commandCount;
        string *commandList;	
	Module *mod;
	Permuter permuter;
	string padding;
	int maxBruteLength;
	int startingBruteLength;
	string resultsFile;
	string hostName;
	string ipAddress;
	int portNumber;
	string uri;
	bool running;
	int wordlistCount;
	string wordlists[WORDLIST_MAX];
	string method = "GET";
	string httpVersion = "HTTP/1.1";
	string extraHeaders[HEADERS_MAX];
	int extraHeadersCount = 0;
	int fetchedHeadersCount = 0;
	string fetchedHeaders[HEADERS_MAX][3]; // used to map headers to uri locations, and wether or not to gather every time.
	bool fetchCookiesOnce = true;
	bool ignoreCookies = true;
	string cookieFetchUri = "";
	string cookies = "";
	int brutePos[2] = {0, 0};
	size_t payloadSize = -1;
	size_t contentLength = -1;
	char content[CONTENT_MAX] = {0};
        char netBuffer[MAX_NET_BUFFER] = {0};
        size_t netBufferCount = 0;
	int successStringsMax = 15;
	string successStrings[15];
	bool successStringsLogic[15] = {false};
	string sessionFile = "";
	
};
#include "./commandFunctions.h"
class HttpBrute : Module{
	private:	
		struct CommandPacket *commandPacket;
		char *_payload = NULL;

		// under construction	
		bool _httpAuth = false;
		const bool _useSsl = false; // unsupported at this time.

		void _printPermuterValues(void){
			printf("\tStarting Length : %d\n", commandPacket->startingBruteLength);
			printf("\tMax Length : %d\n", commandPacket->maxBruteLength);
			printf("\tIncrememnt Mapper : ");
			for(int i=0; i<commandPacket->maxBruteLength; i++)
				printf("%d ", commandPacket->permuter._mapper[i]);
			printf("\n\tFrozen mapper : ");
			for(int i=0; i<commandPacket->maxBruteLength; i++)
                                printf("%d ", commandPacket->permuter._mapperFrozen[i]);
			printf("\n\tIncrement Order : ");
			for(int i=0; i<commandPacket->maxBruteLength; i++)
				printf("%d ", commandPacket->permuter._mapperIncOrder[i]);
			printf("\n\tValid Char Mappings :\n");
			for(int i=0; i<commandPacket->maxBruteLength; i++)
				printf("\t\t%d) %s\n", i, commandPacket->permuter._validCharsArray[i].c_str());
			printf("\n");
		}
		void (*commandFunctions[18])(struct CommandPacket*);


	public:
	void welcomeBanner(void){
		string banner = "";
		banner += "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
		banner += "-     (^)<  HTTP Brute - "+this->getVersion()+"                      -\n";
		banner += "=   ~(>)                                            =\n"; 
		banner += "-	Type 'help' for a list of commands.            -\n";
		banner += "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
		printf("%s\n\n", banner.c_str());
	}

	int getCommandIndex(string command){
                for(int i=0; i<commandPacket->commandCount; i++){
                        if(commandPacket->commandList[i] == command){
                                return i;
                        }
                }
                return -1;
        }

	struct CommandPacket cp;

	void initStruct(){
		cp.mod = this;
		cp.running = true;

		cp.commandCount = 18;
		cp.commandList = (string *)malloc(sizeof(string)*cp.commandCount);
		cp.commandList[0] = "help";
		this->commandFunctions[0] = help;

		cp.commandList[1] = "exit";
		this->commandFunctions[1] = exitHttpBrute;
			
		cp.commandList[2] = "show configs";
		this->commandFunctions[2] = showConfigs;

		cp.commandList[3] = "set host name";
		this->commandFunctions[3] = setHostName;

		cp.commandList[4] = "set ip";
		this->commandFunctions[4] = setIp;

		cp.commandList[5] = "set port number";
		this->commandFunctions[5] = setPortNumber;

		cp.commandList[6] = "set wordlists";
		this->commandFunctions[6] = setWordlists;

		cp.commandList[7] = "set uri";
		this->commandFunctions[7] = setUri;

		cp.commandList[8] = "set padding";
		this->commandFunctions[8] = setPaddingCommand;

		cp.commandList[9] = "set starting pattern";
		this->commandFunctions[9] = setStartingPatternCommand;

		cp.commandList[10] = "set results file";
		this->commandFunctions[10] = setResultsFileCommand;

		cp.commandList[11] = "configure payload";
		this->commandFunctions[11] = configurePayloadCommand;

		cp.commandList[12] = "configure permuter";
		this->commandFunctions[12] = configurePermuter;

		cp.commandList[13] = "attack brute force";
		this->commandFunctions[13] = attackBruteForceCommand;

		cp.commandList[14] = "save session";
		this->commandFunctions[14] = saveSession;

		cp.commandList[15] = "set session file";
		this->commandFunctions[15] = setSessionFile;

		cp.commandList[16] = "load session";
		this->commandFunctions[16] = loadSessionFromFile;

		cp.commandList[17] = "delete auto session";
		this->commandFunctions[17] = deleteSessionFile;

		commandPacket = &cp;

		commandPacket->resultsFile = this->getModuleBase();
		commandPacket->resultsFile += "/results";
		commandPacket->sessionFile = this->getModuleBase();
		commandPacket->sessionFile += "/session";
		memset(commandPacket->content, 0, CONTENT_MAX);
		commandPacket->permuter._validChars = "eariotnslcudpmhgbfywkvxzjq";
		commandPacket->permuter._startingPattern = "eeeeeeeeeeeeeeee";
		commandPacket->permuter._mapperSize = commandPacket->maxBruteLength;	
		commandPacket->permuter.defaultMapper();
		commandPacket->successStrings[0] = "HTTP/1.1 200";
		commandPacket->successStrings[1] = "HTTP/1.1 301";
		commandPacket->successStringsLogic[0] = false;
		commandPacket->successStringsLogic[1] = false;
	}
	void run(){
		welcomeBanner();
		string io = "";
		this->setRunning(true);
		while(this->isRunning()){
			int option = this->getCommandIndex(this->promptUserInput("", "[http-brute](.)> "));
			if(option < 0 || option >= commandPacket->commandCount){
				printf("Invalid command %d\n", option);
			}else{
				printf("executing function number %d\n", option);
				this->commandFunctions[option](commandPacket);
			}
		}	

		printf("(^)< {Thanks for playing!}\n\n");
	}
	HttpBrute(string basePath) : Module("http-brute", "0.0.2", basePath){
		initStruct();
	}
};
