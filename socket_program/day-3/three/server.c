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
    struct sockaddr_in ca; //caは送受信用の構造体
    char recvline[BUFSIZE+1]; //送信用受信用の文字列格納配列
    char tmpline[BUFSIZE+1]; /受信用の変換前の文字列格納配列
    
    //ソケットを一つ確保する
    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    //受信用の情報を構造体ca に格納
    bzero((char *)&ca,sizeof(ca));
    ca.sin_family=AF_INET;
    ca.sin_addr.s_addr=htons(INADDR_ANY);

    //7501に届いたものを処理する
    ca.sin_port=htons(7501);

    //受信用のポートをソケットにバインド
    bind(sockfd,(struct sockaddr *)&ca,sizeof(ca));

    for(;;){
        int address_length = sizeof(ca);
    	//パケットを受信
    	n=recvfrom(sockfd,recvline,BUFSIZE,0,(struct sockaddr *)&ca,&address_length);
        recvline[n]='\0';
	//受け取ったパケットをtmplineにコピー
        strcpy(tmpline,recvline);
        tmpline[n-1]='\0';i
	//小文字を探して大文字に変換
        for(int i=0;i<n;++i){
            if('a'<=recvline[i]&&recvline[i]<='z'){
                recvline[i]+='A'-'a';
            }
        }
        n = strlen(recvline);
	//変換後の文字列を送信
	sendto(sockfd, recvline, n, 0, (struct sockaddr *)&ca, sizeof(ca));
        //変換の状況を出力
	printf("%s:%d - %s => %s",inet_ntoa(ca.sin_addr),ntohs(ca.sin_port),tmpline,recvline);
        fflush(stdout);

    }

    return 0;

}
