//argv[1]= listining port port numbe are 16 bit and range from 0 to 65535
//pick a port in the range of 2000 to 49000
#include <stdio.h> /*for printf and fprintf*/
#include <sys/socket.h> /*for socket(), bind(), connect(), recv(), send() */
#include <arpa/inet.h> /*for sockaddr_in and inet_ntoa*/
#include <stdlib.h> /*for atoi() and exit() */
#include <string.h> /*for memset() */
#include <unistd.h> /* for close() */
#include <sys/types.h> 
#include <netdb.h> /* for struct addrinfo and getnameinfo  */
#include <time.h>

#define MAXPENDING 5 /*Maximum outstanding connection requests most systems max is 20 (use 5 to 10)*/
#define RCVBUFSIZE 200 /*size of receive Buffer*/

struct addrinfo hints, *infoptr;

int CreateTCPServerSocket(int,char *argv[]);
int AcceptTCPConnection(int);
void HandelTCPClient(int );
int ClientMessageProcess(char rcvstring[],char timestamp[]);
int CheckSensor(char SensorID[]);
int LogData(char SensorID[], char rcvstring[], char timestamp[]);

/*----------Start: Main----------*/
int main(int argc, char *argv[]){

	int portnum;
	int  serverSock;
	int serverPortnum;
	int clientSock;

	if(argc != 2){
		fprintf(stderr,"Error: no port provided.\n");
		exit(1);	
	}

	portnum=atoi(argv[1]);


	serverSock = CreateTCPServerSocket(serverPortnum,argv);

	//Runforever
	while(1){
		clientSock = AcceptTCPConnection(serverSock);
		HandelTCPClient(clientSock);
	}

	return 0;

}
/*----------End: Main----------*/


/*----------Start: AcceptTCPConnection----------*/
int AcceptTCPConnection(int serverSock){
	int clientid;

	struct sockaddr_in clientAddr;/*Client address*/

	unsigned int clientLength;/*Length of the client address data structure*/
	clientLength=sizeof(clientAddr);

	if((clientid = accept(serverSock, (struct sockaddr *) &clientAddr,&clientLength)) < 0){
		perror("accept failed\n");
	}

	printf("Handling client: %s\n", inet_ntoa(clientAddr.sin_addr));
	return clientid;
}
/*----------End: AcceptTCPConnection----------*/



/*----------Start: CreateTCPServerSocket----------*/
int CreateTCPServerSocket(int portnum,char *argv[]){

	int result;
	int sockid;
	int optval;
	int otplen;
	optval=1;

	//Method 1: socket setup and bind, modern way using getaddrinfo()---------------------- 
	memset(&hints, 0 ,sizeof(hints));	
	hints.ai_family = AF_UNSPEC; //use IPv4 or IPv6, which ever
	//hints.ai_family = AF_INET; //use IPv4 or IPv6, which ever
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; //fill in my IP for me

	if((result = getaddrinfo(NULL, argv[1], &hints, &infoptr)) != 0){
		//getaddrinfo error
		fprintf(stderr, "ERROR getaddrinfo: %s\n", gai_strerror(result));
		exit(1);
	}else{
		printf("getaddrinfo success\n");

	}

	struct  addrinfo *p;
	char host[1024];
	char service[20];
	for(p = infoptr; p != NULL ; p = p->ai_next){
		if(getnameinfo(p->ai_addr, p->ai_addrlen, host, sizeof(host), service, sizeof(service), 0)!=0){
			perror("get name info error\n");
		}else{	
			printf(" host: %s\n",host);//e.g. IP "ip
			printf(" service: %s\n",service);//e.g."http"

		}
		if((sockid= socket(infoptr->ai_family, infoptr->ai_socktype, infoptr->ai_protocol))==-1){
			//error socket
			perror("socket");
			continue;
		}else{
			printf("Socket Created\n");
		}

		//setsocket options: set SO_REUSEADDR on socket
		if(setsockopt(sockid,SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1){
			perror("setsockopt() error\n");
			exit(1);
		}



		if(bind(sockid, infoptr->ai_addr, infoptr->ai_addrlen) ==-1){
			//bind error
			perror("bind");
			close(sockid);
			continue;
		}else{
			printf("Soceket bound to port successfully\n");
		}	
		break;
	}

	if(p==NULL){
		//looped of the end of the list with no successful bind 
		fprintf(stderr, "failed to bind socke\n");
		exit(1);
	}


	if(listen(sockid, MAXPENDING)==-1){
		perror("listen error");
		exit(1);
	}



	freeaddrinfo(infoptr);

	//End Method 1------------------------------------------------------------------------- 




	/*
	//Method 2: socket setup and bind, packing struct by hand------------------------------ 
	struct sockaddr_in addrport;

	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(portnum);//host to network short

	//IP-address setup two ways
	//1)you can let it automatically select one:
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);//host to network long 
	//2)you can specidt the ip address
	//inet_pton(AF_INET, "63.161.169.137",&(addrport.sin_addr));


	// Create Socket
	if((sockid =socket(PF_INET, SOCK_STREAM, 0)) <0){
	//socket error
	}else{
	printf("Socket Created\n");
	}
	if(bind(sockid, (struct sockaddr *) &addrport, sizeof(addrport)) <0){
	//bind error
	}else{
	printf("Soceket bond to port successfully\n");
	}	
	//End Method 2: --------------------------------------------------------------------- 
	 */

	return sockid;
}
/*----------End: CreateTCPServerSocket----------*/










/*----------Start: HandleTCPClient----------*/
void HandelTCPClient(int clientSock){
	char rcvbuffer[RCVBUFSIZE];
	char sendbuffer[RCVBUFSIZE];
	int recvMsgSize;
	int recvMsgLength;
	int ErrorCode= -1;
	memset(rcvbuffer, '\0',RCVBUFSIZE);

	char timestamp[50];
	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	/*Receive message from client*/
	if((recvMsgSize = recv(clientSock,rcvbuffer,RCVBUFSIZE,0)< 0 )){
		perror("recv failed");
	}

	printf("received from client: ");
	puts(rcvbuffer);
	recvMsgLength= strlen(rcvbuffer);

	if(recvMsgLength > 7){//possible good message
		printf("Message length: %d\n", recvMsgLength);	
		printf("Data logged time: %s", asctime_r(timeinfo, timestamp));	
		printf("\n");
		ErrorCode=ClientMessageProcess(rcvbuffer,timestamp);
	}else{
		if(send(clientSock,"Bad Message",11,0)<0){
			perror("send failed");
		}
	}



	//ErrorCode= ClientMessageProcess(rcvbuffer);

	switch(ErrorCode){

		case 0:
			printf("Datalogged\n");
			if(send(clientSock,"Data Logged",11,0)<0){
				perror("send failed");
			}
			break;
		case 2:
			printf("File Error: ListSensors.txt\n");
			if(send(clientSock,"File Error",10,0)<0){
				perror("send failed");
			}		
			break;

		case 3://unknown ID
			printf("Error Unknow ID\n");
			if(send(clientSock,"Error Unknown ID",16,0)<0){
				perror("send failed");
			}
			break;
		case 4:
			printf("File Error: ListSensors.txt\n");
			if(send(clientSock,"File Error",10,0)<0){
				perror("send failed");
			}		
			break;

	}



	////do something-----------------



	close(clientSock);
}
/*----------End: HandleTCPClient----------*/








int ClientMessageProcess(char rcvstring[],char timestamp[]){

	char SensorId[7];//six numbers plus null char
	int c=0;
	int ErrorNum=0;	

	while(c<6){
		SensorId[c]=rcvstring[c];
		c++;
	}

	printf("Sensor ID is:%s\n",SensorId);

	ErrorNum=CheckSensor(SensorId);

	if(ErrorNum == 0){
		//write to file
		printf("Writeing to file\n");
		ErrorNum=LogData(SensorId,rcvstring,timestamp);
		return 0;
	}else{
		return ErrorNum;
	}	




}


//Check to see if Valid sensor ID ( file: ListOfSensors.txt)
int CheckSensor(char SensorID[]){

	char ValidSensorID[12];
	char SensorName[32];
	FILE *fptr;
	printf("HERE\n");

	if(( fptr = fopen("./SensorFiles/ListOfSensors.txt","r"))==NULL){
		printf("Erroe! opening  ListOfSensors File\n");//List of Sensors is all the sensors in the network
		fclose(fptr);
		return 2;//file failed to open
	}
	printf("HERE1\n");

	fscanf(fptr,"%*[^\n]\n");//skip line 1 of file
	fscanf(fptr,"%*[^\n]\n");//skip line 2 of file
	fscanf(fptr,"%*[^\n]\n");//skip line  of file

	printf("HERE2\n");
	while(fscanf(fptr,"%s %s",ValidSensorID,SensorName)!= EOF){

		//			printf("Sensor found in ListOfSensors: ValidSensor:%s, SensorID:%s, SensorName:%s\n",ValidSensorID,SensorID,SensorName);
		if(strcmp(SensorID,ValidSensorID)==0){
			printf("Passed Sensor found in ListOfSensors: ValidSensor:%s, SensorID:%s, SensorName:%s\n",ValidSensorID,SensorID,SensorName);
			fclose(fptr);
			return 0;//Sensor ID is in file
		}

	}	
	//look at fseek

	fclose(fptr);
	return 3;//Sensor ID not infile

}

int LogData(char SensorID[], char rcvstring[], char timestamp[]){

	printf("HEREx\n");
	char filename[60];
	FILE *lfptr;

	sprintf(filename,"./SensorFiles/Sensor%s/SensorData%s.csv",SensorID,SensorID);
	printf("Filename: %s\n",filename);
	if(( lfptr = fopen(filename,"a"))==NULL){
		printf("Error! opening  Log file File\n");//List of Sensors is all the sensors in the network
		fclose(lfptr);
		return 4;//file failed to open
	}

	printf("HEREy\n");
	fprintf(lfptr,"%s,%s",rcvstring,timestamp);

	printf("HEREz\n");
	fclose(lfptr);

	printf("HEREzz\n");
	return 0;
}





