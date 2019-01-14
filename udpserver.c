/**************server.c**************/  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
#include<arpa/inet.h>  
#include<netdb.h>  
#include<errno.h>  
#include<sys/types.h>  
int port=8888;  
int main(){  
   int sockfd;  
   int len = sizeof(struct sockaddr_in);  
   int z = 0;  
   char buf[256] = {0};  
   struct sockaddr_in adr_inet;  
   struct sockaddr_in adr_clnt;  
   printf("waiting for client...\n");  
   adr_inet.sin_family=AF_INET;  
   adr_inet.sin_port=htons(port);  
   adr_inet.sin_addr.s_addr=htonl(INADDR_ANY);  
   bzero(&(adr_inet.sin_zero),8);  
   len=sizeof(adr_clnt);  
   sockfd=socket(AF_INET,SOCK_DGRAM,0);  
   if(sockfd==-1){  
     perror("socket error_1");  
     exit(1);  
   }  
   z=bind(sockfd,(struct sockaddr *)&adr_inet,sizeof(adr_inet));  
   if(z==-1){  
     perror("bind error_1");  
     exit(1);  
   }  

   while(1){  
     z = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&adr_clnt, &len);  
     if(z < 0){  
       perror("recvfrom error_1");  
       exit(1);  
     }  
     //printf("line %d:%s, z=%d\n", __LINE__, buf, z);
     buf[z] = 0;  
     printf("receive from %s:%d：%s\n", (char *)inet_ntoa(((struct sockaddr_in )adr_clnt).sin_addr), adr_clnt.sin_port, buf);  
     printf("---------%d\n", len);
     int n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&adr_clnt, len);
     printf("---------%d\n", len);
     printf("line %d:send:%s,len=%d\n", __LINE__, buf, n);
     if(strncmp(buf, "stop", 4) == 0){  
       printf("结束....\n");  
       break;  
     }  
   }  
   close(sockfd);  
   exit(0);  
} 
