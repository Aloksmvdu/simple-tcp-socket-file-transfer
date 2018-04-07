#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

char fname[256];

void gotoxy(int x,int y)
 {
 printf("%c[%d;%df",0x1B,y,x);
 }
 
int SendFileToClient(int connfd)
{
      long double sz=1;
       write(connfd, fname,256);
       unsigned long fsize=0;
       FILE *fs = fopen(fname,"r");
      if(fs==NULL)
        {
            printf("File opern error");
	    return -1;
        } 
       fseek(fs, 0L, 2);
       fsize=ftell(fs);
       long int doter=(long)fsize/100;
       printf("Size: %lu bytes\n",fsize);
       fclose(fs);
       char fsz[20];
       sprintf(fsz,"%lu",fsize);
       printf("fsz= %s\n", fsz);
       write(connfd, fsz, 20);
        
        FILE *fp = fopen(fname,"rb");
        if(fp==NULL)
        {
            printf("File opern error");
            return -1;   
        }   

        /* Read data from file and send it */
        while(1)
        {
            /* First read file in chunks of 256 bytes */
            unsigned char buff[1024]={0};
            int nread = fread(buff,1,1024,fp);
            //printf("Bytes read %d \n", nread);        

            /* If read was success, send data. */
            if(nread > 0)
            {
                //printf("Sending \n");
                write(connfd, buff, nread);
            }
            
            sz++;
            gotoxy(0,5);
            printf("Transfered:\033[0;32m %llf Mb\033[0m",(sz/1024));
            gotoxy(0,6); 
            unsigned long int per=(unsigned long)sz*1024;
            float pers=((float)per/(float)fsize)*100;
	    if(pers > 100.0){pers = 100;}
            printf("Completed: [%0.2f%]                       \n",pers);

            fflush(stdout);
            
            if (nread < 1024)
            {
                if (feof(fp))
		{
                    printf("End of file\n");
		    printf("File transfer completed\n");
		}
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }
        }
	return 0;
}

int main(int argc, char *argv[])
{
    system("clear");
    int sockfd = 0;
    int bytesReceived = 0;
    char recvBuff[1024];
    memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in serv_addr;

    /* Create a socket first */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    /* Initialize sockaddr_in data structure */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); // port
    char ip[50];
if (argc < 3) 
{
    printf("Enter IP address to connect: ");
    gets(ip);
    printf("Enter file name to send: ");
    gets(fname);
}
else
    strncpy(ip,argv[1], sizeof(ip));
    strncpy(fname,argv[2], sizeof(fname));
    serv_addr.sin_addr.s_addr = inet_addr(ip);

    /* Attempt a connection */
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

    printf("Connected to ip: %s : %d\n",inet_ntoa(serv_addr.sin_addr),ntohs(serv_addr.sin_port));
    SendFileToClient(sockfd);
    printf("File Sent....Completed\n");
    return 0;
}
