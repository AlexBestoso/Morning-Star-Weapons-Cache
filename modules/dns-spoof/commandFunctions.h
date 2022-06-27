void dnsSpoof_help(struct DnsSpoofCommand *cmd){
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("DNS Spoof Commands : \n");
	for(int i=0; i<cmd->commandCount; i++){
		printf("%s\n", cmd->commandList[i].c_str());
	}
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

void dnsSpoof_interface(struct DnsSpoofCommand *cmd){
	struct if_nameindex *if_nidxs, *intf;

	if_nidxs = if_nameindex();
	int i=0;
    	if (if_nidxs != NULL ){
        	for (intf = if_nidxs; intf->if_index != 0 || intf->if_name != NULL; intf++){
            		printf("%d) %s\n", i, intf->if_name);
			i++;
        	}

    	}else{
		return;
	}

	printf("Which interface would you like to use?\n(.)> ");
	int opt = cmd->mod->getUserInputInt();
	
	i = 0;
	if_nidxs = if_nameindex();
	for(intf = if_nidxs; intf->if_index != 0 || intf->if_name != NULL; intf++){
		if(i == opt){
			cmd->interface = intf->if_name;
			break;
		}
		i++;
	}
	if_freenameindex(if_nidxs);

	cmd->mod->pSuccess("Set interface to '"+cmd->interface+"'\n");
}

void dnsSpoof_exit(struct DnsSpoofCommand *cmd){
	cmd->mod->setRunning(false);
}
