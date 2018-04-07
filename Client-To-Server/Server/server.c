#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>


struct sockaddr_in c_addr;

void *recvFileFromClient(int *arg){
        int sockfd=(int)*arg, bytesReceived = 0;
        char fname[256];
	char recvBuff[1024];
        printf("Connection accepted and id: %d\n",sockfd);
        printf("Connected to Clent: %s:%d\n",inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port));
        /* Create file where data will be stored */
    	FILE *fp;
        unsigned long fsize=0;
        char fsz[20];
	read(sockfd, fname, 256);
	printf("File Name: %s\n",fname);
	fflush(stdout);
        read(sockfd, fsz, 20);
	//strcat(fname,"AK");
	printf("File Name: %s\n",fname);
        printf("File Size: %s bytes\n",fsz);
	fflush(stdout);
        char *ps,*ptr;
        ps=fsz;
        fsize=strtol(ps,&ptr,10);
	printf("Receiving file...");
   	fflush(stdout);
	fp = fopen(fname, "ab"); 
    	if(NULL == fp)
    	{
            printf("Error opening file");
            return;
    	}
        
        int i=0;
        /* Receive data in chunks of 256 bytes */
        while((bytesReceived = read(sockfd, recvBuff, 1024)) > 0)
        { 
            // recvBuff[n] = 0;
            fwrite(recvBuff, 1,bytesReceived,fp);
            // printf("%s \n", recvBuff);
        }

        if(bytesReceived < 0)
        {
            printf("\n Read Error \n");
        }
        printf("File OK....Completed\n");
        printf("Closing Connection for id: %d\n",sockfd);
        close(sockfd);
        shutdown(sockfd,SHUT_WR);
        sleep(2);
}

int main(int argc, char *argv[])
{
    int connfd = 0,err;
    pthread_t tid; 
    struct sockaddr_in serv_addr;
    int listenfd = 0,ret;
    char sendBuff[1025];
    int numrv;
    socklen_t clen=0;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd<0)
	{
	  printf("Error in socket creation\n");
	  exit(2);
	}

    printf("Socket retrieve success\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    ret=bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
    if(ret<0)
    {
      printf("Error in bind\n");
      exit(2);
    }

    if(listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }

    while(1)
    {
        clen=sizeof(c_addr);
        printf("Waiting...\n");
        connfd = accept(listenfd, (struct sockaddr*)&c_addr,&clen);
        if(connfd<0)
        {
	  printf("Error in accept\n");
	  continue;	
	}
        err = pthread_create(&tid, NULL, &recvFileFromClient, &connfd);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
   }
    close(connfd);
    return 0;
}
