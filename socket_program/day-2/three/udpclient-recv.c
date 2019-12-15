#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define BUFSIZE 512

/**
NAME    udpclient -- simple udp client

SYNOPSIS    udpclient address port

DESCRIPTION
    address: UDPサーバのIPアドレス
    port:   UDPサーバのポート番号
**/


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
    ca.sin_port=htons(7503);

    //受信用のポートをソケットにバインド
    bind(sockfd,(struct sockaddr *)&ca,sizeof(ca));

    // 受信したデータを画面に表示する
    for(;;){
        n=recvfrom(sockfd,recvline,BUFSIZE,0,(struct sockaddr *)NULL,(socklen_t *)NULL);
        recvline[n]='\0';
        fputs(recvline,stdout);
        fflush(stdout);

    }

    return 0;
}