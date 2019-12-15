#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define BUFSIZE 512


int main(int argc, char const *argv[])
{
    int sockfd,n;
    struct sockaddr_in ca; //CAは受信用の構造体
    char recvline[BUFSIZE+1]; //信用受信用の文字列格納配列
    
    //ソケットを一つ確保する
    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    //受信用の情報を構造体ca に格納
    bzero((char *)&ca,sizeof(ca));
    ca.sin_family=AF_INET;
    ca.sin_addr.s_addr=htons(INADDR_ANY);

    //7503に届いたものを処理する
    ca.sin_port=htons(7501);

    //受信用のポートをソケットにバインド
    bind(sockfd,(struct sockaddr *)&ca,sizeof(ca));

    // 受信したデータを画面に表示する
    for(;;){
        int address_length = sizeof(ca);
        n=recvfrom(sockfd,recvline,BUFSIZE,0,(struct sockaddr *)&ca,&address_length);
        recvline[n]='\0';
        for(int i=0;i<n;++i){
            if('a'<=recvline[i]&&recvline[i]<='z'){
                recvline[i]+='A'-'a';
            }
        }


        n = strlen(recvline);
        sendto(sockfd, recvline, n, 0, (struct sockaddr *)&ca, sizeof(ca));
        printf("%s",recvline);
        fflush(stdout);

    }

    return 0;

}
