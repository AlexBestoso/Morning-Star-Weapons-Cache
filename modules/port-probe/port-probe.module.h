#define PORT_PROBE_CMD_BUF_SIZE 4098
#define PORT_PROBE_AVAILABLE_COMMAND_COUNT 5

#define PORT_PROBE_CMDMAP_HELP 0
#define PORT_PROBE_CMDMAP_EXIT 1
#define PORT_PROBE_CMDMAP_SETHOST 2
#define PORT_PROBE_CMDMAP_INFO 3
#define PORT_PROBE_CMDMAP_SETPORT 4

class PortProbe : public Module{
	private:
		string host = "";
		int port = 0;
		int portEnd = 0;
		char cmdBuf[PORT_PROBE_CMD_BUF_SIZE];
		size_t cmdSize = 0;

		int portlist[65535] = {0};

		int availableCommandsCount = PORT_PROBE_AVAILABLE_COMMAND_COUNT;
		string availableCommands[PORT_PROBE_AVAILABLE_COMMAND_COUNT] = {
			"help",
			"exit",
			"setHost",
			"info",
			"setPort"
		};
		
		string getArg(int target){
			string ret = "";

			int idx = 0;
			for(int i=0; i<PORT_PROBE_CMD_BUF_SIZE && idx <= target; i++){
				if(cmdBuf[i] == 0x0){idx++;continue;}
				if(idx == target)ret += cmdBuf[i];
			}
			return ret;
		}

		int processCmd(string cmd){
			memset(cmdBuf, 0x00, PORT_PROBE_CMD_BUF_SIZE);
			int argc = 1;
			
			for(int i=0; i<PORT_PROBE_CMD_BUF_SIZE && i<cmd.length(); i++){
				if(cmd[i] == ' ')argc++;
				cmdBuf[i] = cmd[i] == ' ' ? 0x0 : cmd[i];
			}
			cmdSize = argc;
			string target = getArg(0);
			for(int i=0; i<PORT_PROBE_AVAILABLE_COMMAND_COUNT; i++){
				if(target == availableCommands[i])return i;
			}
			return 0;
		}

		void headsUpDisplay(void){
			printf("Host: '%s'\n", host.c_str());
			printf("Target Ports : ");
			for(int i=0; i<65535 && portlist[i] != 0; i++){
				printf("%d ", portlist[i]);
			}printf("\n");
		}
	public:

	PortProbe(string basePath, SqlSnake *sqlsnake) : Module("port-probe", "0.0.0", basePath, sqlsnake){
		this->setRunning(true);
		while(this->isRunning()){
			printf("(port-prober)> ");
			string cmd = this->getUserInput();
			switch(processCmd(cmd)){
				case PORT_PROBE_CMDMAP_EXIT:{
					printf("Exiting.\n");
					this->setRunning(false);
                                }break;
				case PORT_PROBE_CMDMAP_SETHOST:{
					if(cmdSize != 2){
						printf("[E] usage : setHost <targetHost>\n");
						break;
					}

					host = getArg(1);
					printf("[*] Host: %s\n", host.c_str());
				}break;
				case PORT_PROBE_CMDMAP_INFO:{
					headsUpDisplay();
				}break;
				case PORT_PROBE_CMDMAP_SETPORT:{
					if(cmdSize < 2){
						printf("[E] usage : setPort <port> [<port-range>] [<port-range>] [<port>]\n");
						break;
					}
					for(int i=0; i<65535; i++) portlist[i] = 0;
					int b=0;
					for(int i=1; i<cmdSize; i++){
						string val = getArg(i);
						bool ranged = false;
						for(int j=0; j<val.length(); j++){
							if(val[j] == '-')
								ranged = true;
						}
						if(ranged){
							int start =0;
							int end = 0;
							string g="";
							int leftOff = 0;
							for(int j=0; j<val.length(); j++){
								if(val[j] == '-'){
									start = stoi(g.c_str());
									leftOff = j+1;
									break;
								}
								g+=val[j];
							}
							string gg = "";
							for(int j=leftOff; j<val.length(); j++){
								gg += val[j];
							}
							end = stoi(gg.c_str());

							for(int j=start; j<=end; j++){
								bool skip = false;
								for(int a=0; a<b; a++) if(portlist[a] == j) skip = true;
								if(skip) continue;

								portlist[b] = j;
								b++;
							}
						}else{
							bool skip = false;
							string g = "";
							int vali = 0;
							for(int j=0; j<val.length(); j++){
								g += val[j];
							}
							vali = stoi(g.c_str());
							for(int a=0; a<b; a++) if(portlist[a] == vali) skip = true;
                                                                if(skip) continue;
							portlist[b] = vali;
							b++;
						}
					}
					headsUpDisplay();
				}break;
				case PORT_PROBE_CMDMAP_HELP:
				default:{
					printf("Available Commands : %d\n", availableCommandsCount);
					for(int i=0; i<PORT_PROBE_AVAILABLE_COMMAND_COUNT; i++){
						printf("%i) %s -\n", i, availableCommands[i].c_str());
					}
				}break;
			}
		}
	}
	
};
