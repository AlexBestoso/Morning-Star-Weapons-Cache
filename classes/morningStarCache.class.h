class MorningStarCache : BtApplication{
	private:
		bool _running = true;
		const char *BASE_NAME = "storage";

		string _userInput = "";

		void _exitCommand(void){
			_running = false;
		}
		void _welcomeBanner(void){
			string banner = "";
			banner += "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
			banner += "-   The Morning Star Weapons Cache                  -\n";
			banner += "=                              Version "+this->version+"       =\n";
			banner += "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
			printf("%s\n\n", banner.c_str());
		}
		
		void _helpCommand(void){
			printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
			printf("List of Commands (%ld total)\n", (long)this->commandCount);
			for(int i=0; i<this->commandCount; i++){
				printf("%s\n", this->commandList[i].c_str());
			}
			printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
		}
		
	public:
		bool failed = false;
		
		bool initSystem(){
			FileSnake fileSnake;
			string name = BASE_NAME;
			this->basePath = "./"+name;

			printf("Checking for base dir '%s' ...\n", this->basePath.c_str());
			if(!fileSnake.dirExists(this->basePath)){
				printf("Creating base dir\n");
				if(!fileSnake.makeDir(this->basePath+"/")){
				this->pError("\n[E] Failed to create base dir '%s"+this->basePath+"'\n");
					return false;
				}
				this->pSuccess("Dir Successfuly created\n");
			}
			this->pSuccess("System Initialized.\n");
			return true;
		}

		MorningStarCache() : BtApplication(){
			this->version = "0.0.2a";
			this->commandCount = 4;
			this->commandList = (string *)malloc(sizeof(string) * this->commandCount);
			this->commandList[0] = "help";
        	        this->commandList[1] = "exit";
        	        this->commandList[2] = "http-brute";
			this->commandList[3] = "show banner";

			if(!initSystem()){
				this->pError("Failed to init system.\n");
				failed = true;
				return;
			}
		
			_welcomeBanner();

			HttpBrute httpBrute(basePath);
			while(_running){
				_userInput = this->getUserInput("(.)> ");
				if(_userInput == "help"){
					_helpCommand();		
				}else if(_userInput == "exit"){
					_exitCommand();
				}else if(_userInput == "http-brute"){
					httpBrute.run();
				}else if(_userInput == "show banner"){
					_welcomeBanner();
				}else{
					printf("Invalid Command. Try the 'help' command\n");
				}
			}
		}	
};
