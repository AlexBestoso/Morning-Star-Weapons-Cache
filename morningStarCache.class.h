#define CACHE_ITEM_COUNT 4
class MorningStarCache{
	private:
		bool _running = true;
		int _context = -1;
		const char *BASE_NAME = "storage";
		const char *CONFIG_FILE = "config.data";
		string _basePath = "";
		string config = "";
		string _userInput = "";

		SqlSnake sqlsnake;
		
		const int _commandCount = CACHE_ITEM_COUNT;
		string _commandList[CACHE_ITEM_COUNT] = {
			"help",
			"exit",
			"http-brute",
			"port-probe"
		};

		void _exitCommand(void){
			_running = false;
		}
		void getInput(void){
			getline(cin, _userInput);
		}
		void _welcomeBanner(void){
			printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
			printf(" (^)<  Order of the Morning Star\t|\tCyber Weapons Cache               \n");
			printf("~(>)                            God hates bad people  <(.)          \n");  
			printf("          Type 'help' for a list of commands.           (<)~          \n");
			printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n\n");
		}
		
		void _helpCommand(void){
			printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
			printf("List of Commands (%d total)\n", _commandCount);
			for(int i=0; i<_commandCount; i++){
				printf("%s\n", _commandList[i].c_str());
			}
			printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
		}
		
	public:
	bool initSystem(){
		FileSnake fileSnake;
		string name = BASE_NAME;
		_basePath = "./"+name;
		
		config = _basePath + "/"+CONFIG_FILE;

		printf("Checking for base dir '%s' ...\n", _basePath.c_str());
		if(!fileSnake.dirExists(_basePath)){
			printf("Creating base dir\n");
			if(!fileSnake.makeDir(_basePath+"/")){
				fprintf(stderr, "\n[E] Failed to create base dir '%s'\n.", _basePath.c_str());
				return false;
			}
			printf("Dir Successfuly created\n");
		}

		if(!fileSnake.fileExists(config)){
			printf("[*] Creating config file\n");
			string usr = "";
			string pass = "";
			string host = "";
			string port = "";
			string prefix = "";
			while(1){
				if(host == ""){
					printf("Enter MYSQL host : ");
					getInput();
					host = _userInput;
				}else if(port == ""){
					printf("Enter MYSQL port : ");
					getInput();
					port = _userInput;
				}else if(usr == ""){
					printf("Enter MYSQL user : ");
					getInput();
					usr = _userInput;
				}else if(pass == ""){
					printf("Enter MYSQL password : ");
					getInput();
					pass = _userInput;

				}else if(prefix == ""){
					printf("Enter db prefix : ");
					getInput();
					prefix = _userInput;
				}else{
					int porti = stoi(port.c_str());
					
					if(!sqlsnake.init(host, porti, usr, pass, "")){
						printf("[-] Bad SQL Credentials.\n");
						return false;
					}
					sqlsnake.close();

					size_t bufLen = usr.length() + pass.length() + host.length() + port.length() + prefix.length()+5+strlen(db_struct.db_name);
					char buf[bufLen] = {0};
					int b = 0;
					usr += "\n";
					pass += "\n";
					host += "\n";
					port += "\n";
					string dbName = db_struct.db_name;
					dbName = prefix + dbName + "\n";
	
					for(int i=0; i<usr.length(); i++){
						buf[b] = usr[i];
						b++;
					}
					for(int i=0; i<pass.length(); i++){
						buf[b] = pass[i];
						b++;
					}
					for(int i=0; i<host.length(); i++){
						buf[b] = host[i];
						b++;
					}
					for(int i=0; i<port.length(); i++){
						buf[b] = port[i];
						b++;
					}
					for(int i=0; i<dbName.length(); i++){
						buf[b] = dbName[i];
						b++;
					}
					
					if(!fileSnake.writeFileTrunc(config, buf, bufLen)){
						printf("[E] Failed to create config file.\n");
						return false;
					}

					printf("[+] Config file created.\n");

					sqlsnake.connect();
					if(!sqlsnake.createDatabase(dbName)){
						printf("Database creation failed.\n");
						return false;
					}
					sqlsnake.close();		
					break;
				}
			}
		}else{
			printf("[*] Loading configuration settings.\n");
			size_t bufLen = fileSnake.getFileSize(config);
			char buf[bufLen] = {0};
			if(!fileSnake.readFile(config, buf, bufLen)){
				printf("Failed to read config file.\n");
				return false;
			}

			string usr = "";
                        string pass = "";
                        string host = "";
                        string port = "";
			int porti = 0;
			string dbName = "";
			int b =0 ;
			for(int i=0; i<bufLen; i++){
				switch(b){
					case 0:
						if(buf[i] == '\n')
							b++;
						else
							usr += buf[i];
					break;
					case 1:
						if(buf[i] == '\n')
							b++;
						else
							pass += buf[i];
					break;
					case 2:
						if(buf[i] == '\n')
							b++;
						else
							host += buf[i];
					break;
					case 3:
						if(buf[i] == '\n')
							b++;
						else
							port += buf[i];
					break;
					case 4:
						if(buf[i] == '\n')
							b++;
						else
							dbName += buf[i];
					break;

				}
			}
			porti = stoi(port.c_str());
			if(!sqlsnake.init(host, porti, usr, pass, dbName)){
				printf("[E] Failed to connect to database.\n");
				return false;
			}
			sqlsnake.close();
		}

		return true;
	}

	MorningStarCache(){
		while(_running){
			switch(_context){
				case 0:
					printf("(.)> ");
					getline(cin, _userInput);
					if(_userInput == "help"){
						_helpCommand();		
					}else if(_userInput == "exit"){
						_exitCommand();
					}else if(_userInput == "http-brute"){
						HttpBrute httpBrute(_basePath);
					}else if(_userInput == "port-probe"){
						PortProbe portProbe(_basePath, &sqlsnake);
					}else{
						printf("Invalid Command. Try the 'help' command\n");
					}
					break;
				case -1:
					printf("Initializing system...\n");
					if(!initSystem()){
						fprintf(stderr, "[E] Failed to init system.\n");
						_running = false;
					}else{
						_context = 0;
						printf("\n\n");
						_welcomeBanner();
					}
					break;
				default:
					fprintf(stderr, "[E] Something went wrong. Exiting weapons chache.\n");
					_running = false;
					break;
			}
		}
	}	
};
