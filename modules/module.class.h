class Module{
	private:
	string _name = "Module";
	string _version = "0.0.0";
	bool _running = false;
	int _context = -1;
	string _baseStorage = "./storage";
	string _moduleStorage = "";

	public:
	string getName(void){
		return this->_name;
	}
	void setName(string name){
		this->_name = name;
	}
	string getVersion(void){
		return this->_version;
	}
	void setVersion(string version){
		this->_version = version;
	}
	bool isRunning(void){
		return this->_running;
	}
	void setRunning(bool running){
		this->_running = running;
	}
	int getContext(void){
		return this->_context;
	}
	void setContext(int context){
		this->_context = context;
	}
	string getUserInput(void){
		string ret = "";
		getline(cin, ret);
		return ret;
	}
	
	int getUserInputInt(void){
		string valid = "0123456789";
		string grab = "";
		getline(cin, grab);
		int ret = -1;
		int scoreMultiplyer = 1;
		for(int i=grab.length()-1;i>-1; i--){
			for(int j=0; j<valid.length(); j++){
				if(grab[i] == valid[j]){
					if(ret <= -1)
						ret = 0;
					ret += j * scoreMultiplyer;
					scoreMultiplyer *= 10;
					break;
				}
			}
		}
		return ret;
	}
	string getModuleBase(void){
		return this->_moduleStorage;
	}

	void generateModuleBaseDir(void){
		FileSnake filesnake;
		if(!filesnake.dirExists(_moduleStorage)){
			if(!filesnake.makeDir(_moduleStorage))
				fprintf(stderr, "[E] Failed to create module storage folder '%s'\n", _moduleStorage.c_str());
		}
	}

	string promptUserInput(string msg, string pointer){
		printf("%s\n%s", msg.c_str(), pointer.c_str());
		return getUserInput();
	}

	int promptUserInputInt(string msg, string pointer){
		printf("%s\n%s", msg.c_str(), pointer.c_str());
		return getUserInputInt();
	}
	Module(string name, string version, string baseStorage){
		setName(name);
		setVersion(version);
		_baseStorage = baseStorage;
		_moduleStorage = "";
		_moduleStorage += baseStorage + "/" + name + "_" + version;
		generateModuleBaseDir();
	}
};
