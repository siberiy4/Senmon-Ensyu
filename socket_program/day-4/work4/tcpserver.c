#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>

/*
NAME 
    tcpserver -- simple tcp server

SYNOPSIS
    tcpserver port

DESCRIPTION
    port:port number of target sever
*/

int main(int argc, char const *argv[])
{
    int sockfd, len, s, n, i, pid;
    struct sockaddr_in sa, ca;
    char buf[512];
    char *c = buf;
    pid_t result;

    if (argc != 2)
    {
        fprintf(stderr, "invalid arguments\n");
        return EXIT_FAILURE;
    }

    //ソケットを一つ確保する
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //SOCK_STREAMでTCPを指定

    //構造体saを用意
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(atoi(argv[1]));

    //bindの戻り地を確認する。負の場合はエラー
    if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) < 0)
    {
        perror("bind");
        return EXIT_FAILURE;
    }

    //listen() を実行するとTCPそけっとがlistenとなる
    listen(sockfd, 5);

    //accept()で接続の受け入れ。接続されるとacceptをぬける
    len = sizeof(ca);

    while (1)
    {

        s = accept(sockfd, (struct sockaddr *)&ca, (socklen_t *)&len);
        printf("Connected\n");
        //sockfdはTCP接続を受け入れるソケット、sは接続されたソケットを表す

        //子プロセスを作成
        pid = fork();

        if (pid == 0)
        {
            //受け取った文字列を大文字から小文字に変換して返す
            for (;;)
            {
                n = recv(s, buf, sizeof(buf), 0);
                if (n == 0)
                {
                    break;
                }
                buf[n] = '\0';
                fputs(buf, stdout);
                fflush(stdout);
                c = buf;
                while (*c)
                {
                    *c = (char)tolower((int)*c);
                    c++;
                }
                send(s, buf, n, 0);
            }

            close(s);
            printf("Closed\n");
        }
        close(s);
    }
    close(sockfd);

    return EXIT_SUCCESS;
}
