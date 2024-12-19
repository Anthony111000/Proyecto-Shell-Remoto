#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include <string.h>
int main(int argc, char const* argv[]) 
{ 


	char strData[326];
	int sockD = socket(AF_INET, SOCK_STREAM, 0); 

	struct sockaddr_in servAddr; 

	servAddr.sin_family = AF_INET; 
	servAddr.sin_port 
		= htons(9001); // use some unused port number 
	servAddr.sin_addr.s_addr = INADDR_ANY; 

	int connectStatus 
		= connect(sockD, (struct sockaddr*)&servAddr, 
				sizeof(servAddr)); 

	if (connectStatus == -1) { 
		printf("Error...\n"); 
	} 

	else {
		do{//sending message to the server and recieving
			send(sockD,strData,sizeof(strData),0);
			fgets(strData,sizeof(strData),stdin);
			recv(sockD, strData, sizeof(strData), 0); 
			strData[strcspn(strData,"\n")]='\0';
			printf("Message: %s\n", strData);
		}while(strcmp(strData,"leave")!=0); 
		; 
	} 


	return 0; 
}
