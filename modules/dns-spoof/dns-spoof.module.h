#define COMMAND_COUNT 3
struct DnsSpoofCommand{
	size_t commandCount;
        string commandList[COMMAND_COUNT];
	string interface;
	Module *mod;

}dsc;

#include "./commandFunctions.h"

class DnsSpoof : Module{
	private:
	
	public:
	struct DnsSpoofCommand *dnsCommand;
	void (*commandFunctions[COMMAND_COUNT])(struct DnsSpoofCommand *);

	void run(void){
		this->setRunning(true);
                while(this->isRunning()){
			int option = this->getCommandIndex(this->promptUserInput("", "[dns-spoof](.)> "), dsc.commandCount, dsc.commandList);
                        if(option < 0 || option >= dsc.commandCount){
                                printf("Invalid command %d\n", option);
                        }else{
                                printf("executing function number %d\n", option);
                                this->commandFunctions[option](dnsCommand);
                        }
		}
	}
	void init(){
		dsc.mod = this;
		dsc.commandCount = COMMAND_COUNT;

		dsc.commandList[0] = "help";
		this->commandFunctions[0] = dnsSpoof_help;

		dsc.commandList[1] = "set interface";
		this->commandFunctions[1] = dnsSpoof_interface;

		dsc.commandList[2] = "exit";
		this->commandFunctions[2] = dnsSpoof_exit;
		dnsCommand = &dsc;
	
	}
	DnsSpoof(string basePath) : Module("dns-spoof", "0.0.1", basePath){
		init();
		run();
	}
};
