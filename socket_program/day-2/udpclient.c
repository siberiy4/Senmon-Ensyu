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
    struct sockaddr_in sa,ca; //saは送信用,CAは受信用の構造体
    char sendline[BUFSIZE],recvline[BUFSIZE+1]; //送信用受信用の文字列格納配列

    if (argc != 3)
    {
        fprintf(stderr, "invalid parameters\n");
        return -1;
    }
    
    //通信相手先の送信用情報を構造体SAに格納
    bzero((char *)&sa,sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(argv[1]);
    sa.sin_port=htons(atoi(argv[2]));

    //ソケットを一つ確保する

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    //受信用の情報を構造体ca に格納
    bzero((char *)&ca,sizeof(ca));
    ca.sin_family=AF_INET;
    ca.sin_addr.s_addr=htons(INADDR_ANY);

    //自分自身のどのIPアドレスにパケットが到着しても自分宛てとして受け取る
    ca.sin_port=htons(0);
    //自分側のポート番号はOSに適切にもらう、０位外は自分でポート指定したことになる

    //受信用のポートをソケットにバインド
    bind(sockfd,(struct sockaddr *)&ca,sizeof(ca));

    // キーボードから読み込んだ文字列を送信する。また、受信したデータを画面に表示する
    for(;;){
        fgets(sendline,BUFSIZE,stdin);
        n=strlen(sendline);
        sendto(sockfd,sendline,n,0,(struct sockaddr *)&sa,sizeof(sa));
        n=recvfrom(sockfd,recvline,BUFSIZE,0,(struct sockaddr *)NULL,(socklen_t *)NULL);
        recvline[n]='\0';
        fputs(recvline,stdout);
        fflush(stdout);
    }

    return 0;
}
