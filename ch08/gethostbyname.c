/*
	IP주소는 도메인 이름에 비해 변경될 확률이 높다.
	클라이언트에서 도메인을 통해 IP를 가져온다면, 조금 더 자유로워질 수 있을 것이다.
	도메인으로부터 IP를 추출하는 함수에 대해 알아보자.
	
	
	#include<netdb.h>
	struct hostent* gethostbyname(const char* hostname);
	
	성공시 hostent 구조체 변수의 주소값. 실패시 NULL 포인터를 반환한다.
	hostent 구조체는 다음과 같이 정의되어 있다.
	
	struct hostent{
		char *h_name;		// official name
		char **h_aliases;   // alias list
		int h_addrtype;		// host address type
		int h_length;		// address length
		char **h_addr_list;	// address list
	}

*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netdb.h>

void error_handling(char *message);

int main(int argc, char* argv[]){
	int i;
	struct hostent *host;
	if(argc != 2){
		printf("Usage : %s <addr>\n", argv[0]);
		exit(1);
	}
	
	host = gethostbyname(argv[1]);
	if(!host){
		error_handling("gethost... error");
	}
	
	// Official name
	printf("Official name: %s \n", host->h_name);

	
	// Aliases
	for(i=0;host->h_aliases[i];i++){
		printf("Aliases %d: %s\n", i+1, host->h_aliases[i]);
	}

	
	// Address type
	printf("Address type: %s \n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");

	
	// IP address
	for(i=0;host->h_addr_list[i];i++){
		printf("IP addr %d: %s\n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	}
	
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}