#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
    int sockfd, n;
    struct sockaddr_in sa;  //saは送信用の構造体
    char sendline[BUFSIZE]; //送信用の文字列格納配列
    int yes = 1;

    //ソケットを一つ確保する
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //通信相手先の送信用情報を構造体SAに格納
    bzero((char *)&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("150.89.212.255");
    sa.sin_port = htons(7503);
    //ブロードキャストを送信するオプションを使う
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof(yes));

    // キーボードから読み込んだ文字列を送信する。
    for (;;)
    {
        fgets(sendline, BUFSIZE, stdin);
        n = strlen(sendline);
        sendto(sockfd, sendline, n, 0, (struct sockaddr *)&sa, sizeof(sa));
    }

    return 0;
}
