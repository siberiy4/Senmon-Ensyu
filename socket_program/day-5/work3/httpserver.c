#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DEFAULT_SERV_TCP_PORT 8800

/*
NAME
    httpserver -- simple http server

SYNOPSIS
    httpserver [port]

DESCRIPTION
    port: port number for listening
*/

int main(int argc, char **argv)
{
    int sockfd, len, s, n, port, pid;
    struct sockaddr_in ca;
    char buf[1024];
    char scode[100] = "HTTP/1.0 200 OK\n";
    char eoh[] = "\n";
    FILE *fp;
    char meth[100], ind[100], ver[100];
    char open_file[500];

    //サーバの待受ポートの決定
    //実行オプションが過多であればプログラムを終了
    switch (argc)
    {
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

    //構造体caを用意し、bindする
    ca.sin_family = AF_INET;
    ca.sin_addr.s_addr = htonl(INADDR_ANY);
    ca.sin_port = htons(port);

    //bindに失敗すればエラー表示して終了
    if (bind(sockfd, (struct sockaddr *)&ca, sizeof(ca)) == -1)
    {
        perror("bind");
        return EXIT_FAILURE;
    }

    //ソケットをlistenし始める
    listen(sockfd, 5);
    len = sizeof(ca);

    //サーバーのプログラムをループさせる
    while (1)
    {
        //accept()で接続を受け入れる.相手から接続されればacceptを抜ける
        s = accept(sockfd, (struct sockaddr *)&ca, (socklen_t *)&len);
        

        //プロセス分ける
        pid = fork();

        //子プロセスの内容
        if (pid == 0)
        {

            //accept()で異常がないか確認する
            if (s < 0)
            {
                perror("accept");
                return EXIT_FAILURE;
            }

            //接続元IP、PORTを表示する
            printf("%s:%d\n", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));

            //ブラウザから送られてきたGETメソッドを受け取る
            n = recv(s, buf, sizeof(buf), 0);

            //HTTPリクエストを最大1024バイトだけ読む、それ以降は無視する
            buf[n] = 0;

            //相手のリクエストを表示
            printf("文字数：%d\n", n);
            printf("%s\n", buf);

            //GET リクエストの解析
            sscanf(buf, "%s%s%s", meth, ind, ver);

            //相手が指定するファイルを開く
            if (strlen(ind) == 1)
            {
                strncpy(open_file, "index.html", 500);
            }
            else
            {
                strncpy(open_file, &ind[1], 500);
            }
            fp = fopen(open_file, "r");

            //ファイルが開かなければ"not found"のページを開く
            if (fp == '\0')
            {
                strncpy(scode, "HTTP/1.0 404 Not Found\n", 100);
                strncpy(open_file, "404.html", 500);
                fp = fopen(open_file, "r");
            }

            //HTTP1.0の手順に従い、ステータスコードと空行を送出
            send(s, scode, strlen(scode), 0);
            send(s, eoh, strlen(eoh), 0);

            //開いたファイルを読み出し、すべてsendする
            while ((n = fread(buf, 1, 1024, fp)) > 0)
            {
                //一度に最大1024バイト読み、配列buf[]に格納
                send(s, buf, n, 0);
            }
            //ファイルのを閉じ、接続を終了する
            fclose(fp);
            close(s);
        }
        else
        {
            
            //親プロセス側の接続を終了する
            close(s);
            continue;
        }
    }
    close(sockfd);
    return EXIT_SUCCESS;
}