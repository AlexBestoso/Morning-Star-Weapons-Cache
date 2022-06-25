#define COLOR_RESET  	"\x1B[0m"
#define COLOR_RED  	"\x1B[31m"
#define COLOR_GREEN  	"\x1B[32m"
#define COLOR_YELLOW  	"\x1B[33m"
#define COLOR_BLUE  	"\x1B[34m"
#define COLOR_MAGENTA  	"\x1B[35m"
#define COLOR_CYAN  	"\x1B[36m"
#define COLOR_WHITE  	"\x1B[37m"

class BtApplication{
	private:
	public:
		string version = "0.0.0";
		string basePath = "";
		string getUserInput(string prompt){
			printf("%s", prompt.c_str());
                	string ret = "";
                	getline(cin, ret);
                	return ret;
        	}
		
		int getUserInputInt(string prompt){
			printf("%s", prompt.c_str());
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
		void pError(string msg){
			fprintf(stderr, "\x1b[31m[E] %s\x1b[0m", msg.c_str());
		}
		void pSuccess(string msg){
			printf("\x1b[32m[+] %s\x1b[0m", msg.c_str());
		}

		string *commandList;
		size_t commandCount = 0;
		
		BtApplication(){
		
		}
		
};
