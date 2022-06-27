#define SNAKE_INACTIVE 0
#define SNAKE_ACTIVE_CLIENT 1
#define SNAKE_ACTIVE_SERVER 2
#define SNAKE_MODE_TCP 0
class NetSnake{
	private:
	int socketDescriptor;
	int connection;
	int isActive = SNAKE_INACTIVE;
	bool isConnected = false;
	int port;
	string host;
	char buffer[8192];
	struct sockaddr_in sockAddr;
	int nll = 0;
	struct rtmsg *rtp;
	int rtl;
	struct rtattr *rtap;
	struct sockaddr_nl netlinkAddr, pAddr;
	struct nlmsghdr *nlp;
	struct iovec iov;
	struct msghdr msg;
	struct {
  		struct nlmsghdr nl;
		struct sockaddr_nl groupAddr;
  		struct rtmsg    rt;
  		char	buf[8192];
	}netlinkReq;
	


	bool connectTcpClient(){
		if(this->isActive != SNAKE_ACTIVE_CLIENT){
			fprintf(stderr, "Snake imporperly configured.\n");
			this->isActive = SNAKE_INACTIVE;
			return false;
		}
		if(connect(this->socketDescriptor, (struct sockaddr *)&this->sockAddr, sizeof(this->sockAddr)) < 0){
			fprintf(stderr, "Failed to Establish tcp connection.\n");
			this->isActive = SNAKE_INACTIVE;
			return false;
		}
		this->isConnected = true;
		return true;
	}
	bool createTcpClient(){
		this->isActive = SNAKE_INACTIVE;
		if((this->socketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0){
			fprintf(stderr, "Failed to create socket.\n");
			return false;
		}
		
		this->sockAddr.sin_family = AF_INET;
		this->sockAddr.sin_port = htons(this->port);
		
		if(inet_pton(AF_INET, this->host.c_str(), &this->sockAddr.sin_addr) <= 0){
			fprintf(stderr, "Invalid Host Name.\n");
			this->socketDescriptor = -1;
			return false;
		}

		
		this->isActive = SNAKE_ACTIVE_CLIENT;
		return true;
	}

	public:
	int recvSize;
	int sendSize;

	void closeSocket(){
                this->isConnected = false;
                close(this->socketDescriptor);
        }

	bool CreateIpClient(string host, int port, int mode){
		this->host = host;
		this->port = port;
		switch(mode){
			case 0:
				return this->createTcpClient();
				break;
			default:
				fprintf(stderr, "Invalid Snake Mode");
				return false;
				break;
		}
	}

	void testA(){ // create request.
		bzero(&netlinkReq, sizeof(netlinkReq));

		netlinkReq.nl.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
		netlinkReq.nl.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
		netlinkReq.nl.nlmsg_type = RTM_GETROUTE;

		netlinkReq.rt.rtm_family = AF_INET;
		netlinkReq.rt.rtm_table = RT_TABLE_MAIN;
		/*netlinkReq.groupAddr.nl_family = AF_INET;
		netlinkReq.groupAddr.nl_pid = getpid();
		netlinkReq.groupAddr.nl_groups = RTMGRP_NEIGH;*/
	}
	void testB(){ // write request
		bzero(&pAddr, sizeof(pAddr));
		pAddr.nl_family = AF_NETLINK;
		
		bzero(&msg, sizeof(msg));
		msg.msg_name = (void *)&pAddr;
		msg.msg_namelen = sizeof(pAddr);

		iov.iov_base = (void *) &netlinkReq.nl;
		iov.iov_len = netlinkReq.nl.nlmsg_len;
		msg.msg_iov = &iov;
		msg.msg_iovlen = 1;

		int ret = sendmsg(this->socketDescriptor, &msg, 0);
	}
	void testC(){ // recv response.
		bzero(buffer, sizeof(buffer));
		char *p = buffer;
		int err = 0;

		while(1){
			err = recv(this->socketDescriptor, p, sizeof(buffer) - nll, 0);
			nlp = (struct nlmsghdr *)p;
			if(nlp->nlmsg_type == NLMSG_DONE){
				break;
			}

			p += err;
			nll += err;
			
			if((netlinkAddr.nl_groups & RTMGRP_IPV4_ROUTE) == RTMGRP_IPV4_ROUTE){
				break;
			}
		}
	}
	void testD(){ // parse response
		char dsts[24], gws[24], ifs[16], ms[24];

		nlp = (struct nlmsghdr *)buffer;
		for(;NLMSG_OK(nlp, nll); nlp=NLMSG_NEXT(nlp, nll)){
			rtp = (struct rtmsg *)NLMSG_DATA(nlp);

			if(rtp->rtm_table != RT_TABLE_MAIN)
				continue;

			bzero(dsts, sizeof(dsts));
			bzero(gws, sizeof(gws));
			bzero(ifs, sizeof(ifs));
			bzero(ms, sizeof(ms));
			
			rtap = (struct rtattr *)RTM_RTA(rtp);
			rtl = RTM_PAYLOAD(nlp);
			for(;RTA_OK(rtap, rtl); rtap = RTA_NEXT(rtap, rtl)){
				switch(rtap->rta_type){
					case RTA_DST:
						inet_ntop(AF_INET, RTA_DATA(rtap), dsts, 24);
						break;
					case RTA_GATEWAY:
						inet_ntop(AF_INET, RTA_DATA(rtap), gws, 24);
						break;
					case RTA_OIF:
						sprintf(ifs, "%d", *((int *) RTA_DATA(rtap)));
						break;
					case RTM_GETNEIGH:
						printf("Found something\n");
						break;
					default:
						break;
				}
			}
			sprintf(ms, "%d", rtp->rtm_dst_len);
			printf("dst %s/%s gw %s if %s\n", dsts, ms, gws, ifs);
		}
	}
	bool CreateNetlinkClient(){
		if((this->socketDescriptor = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE)) < 0){
                        fprintf(stderr, "Failed to create netlink socket.\n");
                        return false;
                }
		bzero(&netlinkAddr, sizeof(netlinkAddr));
		netlinkAddr.nl_family = AF_NETLINK;
		netlinkAddr.nl_pid = getpid();
		testA();
		if(bind(this->socketDescriptor, (struct sockaddr*)&netlinkAddr, sizeof(netlinkAddr)) == -1){
			fprintf(stderr, "Failed to bind netlink socket\n");
			return false;
		}
		return true;
	}

	bool recvTcpClient(char *buffer, size_t bufferSize, int posRecv){
                if(!this->isConnected){
                        if(!this->connectTcpClient())
                                return false;
                }
                memset(buffer, 0x00, bufferSize);
                if((this->recvSize = recv(this->socketDescriptor, buffer, bufferSize, posRecv)) < 0){
                        return false;
                }

                return true;
        }
        bool sendTcpClient(const char *buffer, size_t bufferSize){
                if(!this->isConnected){
                        if(!this->connectTcpClient())
                                return false;
                }
                if((this->sendSize = send(this->socketDescriptor, buffer, bufferSize, 0)) < 0){
                        return false;
                }

                return true;
        }

};
