class MorningStarCache{
	private:
		bool _running = true;
		int _context = -1;
		const char *BASE_NAME = "storage";
		string _basePath = "";

		string _userInput = "";

		const int _commandCount = 3;
		string _commandList[3] = {
			"help",
			"exit",
			"http-brute"
		};

		void _exitCommand(void){
			_running = false;
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

		printf("Checking for base dir '%s' ...\n", _basePath.c_str());
		if(!fileSnake.dirExists(_basePath)){
			printf("Creating base dir\n");
			if(!fileSnake.makeDir(_basePath+"/")){
				fprintf(stderr, "\n[E] Failed to create base dir '%s'\n.", _basePath.c_str());
				return false;
			}
			printf("Dir Successfuly created\n");
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
