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

#define DEFAULT_SERV_TCP_PORT 8800

/*
NAME
    httpserver -- simple http server

SYNOPSIS
    httpserver [port]

DESCRIPTION
    poty: port number for listening
*/

int main(int argc, char **argv)
{
    int sockfd, len, s, n, port;
    struct sockaddr_in ca;
    char buf[1024];
    char scode[] = "HTTP/1.0 200 OK\n";
    char eoh[] = "\n";
    FILE *fp;

    switch (argc) {
    case 1:
        port = DEFAULT_SERV_TCP_PORT;
        break;
    case 2:
        port = atoi(argv[1]);
        break;
    default:
        fprintf(stderr, "invalid arguments.\n");
        return EXIT_FAILURE;
    }

    //ソケットを一つ確保する
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //構造体caを用意し、bind、listenする
    ca.sin_family = AF_INET;
    ca.sin_addr.s_addr = htonl(INADDR_ANY);
    ca.sin_port = htons(port);

    //bindに失敗すればエラー表示して終了
    if (bind(sockfd, (struct sockaddr *)&ca, sizeof(ca)) == -1)
    {
        perror("bind");
        return EXIT_FAILURE;
    }

    listen(sockfd, 5);

    //accept()で接続を受け入れる.相手から接続されればacceptを抜ける
    len = sizeof(ca);
    s = accept(sockfd, (struct sockaddr *)&ca, (socklen_t *)&len);
    if (s < 0)
    {
        perror("accept");
        return EXIT_FAILURE;
    }

    //接続元IP、PORTを表示する
    printf("%s:%d\n",inet_ntoa(ca.sin_addr),ntohs(ca.sin_port));
    
    //ブラウザから送られてきたGETメソッドを受け取る
    n = recv(s, buf, sizeof(buf), 0);
    //HTTPリクエストの最大1024バイトだけ読む、それ以降は無視する
    buf[n] = 0;


    printf("文字数：%d\n",n);
    printf("%s\n",buf);
    
    //HTTP1.0の手順に従い、ステータスコードと空行を送出
    send(s,scode,strlen(scode),0);
    send(s, eoh, strlen(eoh), 0);

    //ローカルディスクの中にあるindex.htmlファイルを読み出し、すべてsendする
    fp=fopen("index.html","r");
    while((n=fread(buf,1,1024,fp))>0){
        //一度に最大1024バイト読み、配列buf[]に格納
        send(s, buf, n, 0);
    }
    fclose(fp);
    close(s);
    close(sockfd);
    return EXIT_SUCCESS;


}