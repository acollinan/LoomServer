//argv[1]= listining port

#include <stdio.h> /*for printf and fprintf*/
#include <sys/socket.h> /*for socket(), bind(), connect(), recv(), send() */
#include <arpa/inet.h> /*for sockaddr_in and inet_ntoa*/
#include <stdlib.h> /*for atoi() and exit() */
#include <string.h> /*for memset() */
#include <unistd.h> /* for close() */


int CreateTCPServerSocket(unsigned short ServerPort,char *argv[]);

int main(int argc, char *argv[]){

int  sockid;





	return 0;

}


int CreateTCPServerSocket(unsigned short ServerPort,char *argv[]){


	//Method 1: socket setup and bind, modern way using getaddrinfo()---------------------- 
	
	//End Method 1------------------------------------------------------------------------- 


	//Method 2: socket setup and bind, packing struct by hand------------------------------ 
	int portnum;
	int sockid;
	struct sockaddr_in addrport;

	portnum=atoi(argv[1]);
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(portnum);//host to network short
	
	//IP-address setup two ways
	//1)you can let it automatically select one:
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);//host to network long 
	//2)you can specidt the ip address
	//inet_pton(AF_INET, "63.161.169.137",&(addrport.sin_addr));
	
	
	/* Create Socket*/
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

return 1;
}
