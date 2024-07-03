#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <unistd.h>
#define IP "10.249.47.4"
#define PORT 8080
#define MAX 100
int final=0;
// 创建socket套接字文件，并连接
// 接受数据  client 客户端
typedef struct sockaddr_in SIN;
typedef struct sockaddr SA;
SIN ser_addr;
int sockfd;
void *message(void *arg)
{
    printf("thread creat success!\n");
    char buf[100];
    int signal;
    while(1)
    {
        if(final==1)
            break;
        memset(buf,0,100);
        signal=recv(sockfd,buf,MAX,0);
        if(signal!=0){
            system("date");
            write(1,buf,strlen(buf));
            memset(buf,0,100);}
    }
    pthread_exit(NULL);
}
int main(int argc,char *argv[])
{
    //1、创建套接字文件，返回套接字文件描述符 socket()
    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    pthread_t tid;
    if(sockfd == -1)
    {
        perror("socket create failure\r\n");
        return 0;     }
 
    //2、创建结构，初始化数据 struct sockaddr      使用 struct sockaddr_in
    SIN ser_addr;
    ser_addr.sin_family = AF_INET;//选择ipv4协议族  
    ser_addr.sin_port=htons(PORT);  //端口号要转换端绪，从小端绪转换从大端绪
    ser_addr.sin_addr.s_addr=inet_addr(IP);//十进制的字符ip转换成网端数据格式 服务端IP
    int len=sizeof(ser_addr);
    if(connect(sockfd,(SA *)&ser_addr,len)==-1)
    {
        perror("connect failure\r\n");
        return 0;
    }else
    {
        printf("WELCOME TO DADONG TALK ROOM!\r\n");   
        printf("Please sign you name:");
        char temp[20]={0};
        memset(temp,0,100);
        scanf("%s",temp);
        send(sockfd,temp,strlen(temp),0);//发送信息
        pthread_create(&tid,NULL,message,NULL);
    }
    char wbuf[100]={0};
    while(1)
    {
        memset(wbuf,0,100);
        read(0,wbuf,100);
        if(strncmp(wbuf,"quit",4)==0)
        {
            send(sockfd,wbuf,strlen(wbuf),0);
            final=1;
            close(sockfd);
            return 0;
        }
        send(sockfd,wbuf,strlen(wbuf),MSG_NOSIGNAL);
        memset(wbuf,0,100);
        usleep(20);
    }
    close(sockfd);
    return 0;
}        
