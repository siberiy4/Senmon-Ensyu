#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char const *argv[])
{
    int sockfd, n;
    struct sockaddr_in ca;
    char sendline[1000];
    struct hostent *host_info;
    struct in_addr addr;

    //引数でドメイン名を受け取っていることを確認する。
    if (argc != 2)
    {
        fprintf(stderr, "invalid arguments.\n");
        return EXIT_FAILURE;
    }

    //ドメイン名からアドレスを調べる
    host_info = gethostbyname(argv[1]);
    
    //ドメイン名が存在していなければエラー文を出力して終了
    if (!host_info)
    {
        fprintf(stderr, "Host not found\n");
        return EXIT_FAILURE;
    }

    //アドレスを出力用フォーマットに変換して出力
    addr.s_addr = **(unsigned int **)(host_info->h_addr_list);
    printf("%s: %s\n", argv[1], inet_ntoa(addr));
    printf("%s: %s\n", argv[1], *host_info->h_addr_list);

    return 0;
}
