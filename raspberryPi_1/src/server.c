#include "server.h"

extern int share_val;
extern int alret_val;
extern pthread_mutex_t mutex;

void *runServer(void* arg) {
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[100]; // Adjust the size based on your needs

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        fputs("socket() error\n", stderr);
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8080);

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        fputs("bind() error\n", stderr);
        exit(1);
    }

    if (listen(serv_sock, 5) == -1) {
        fputs("listen() error\n", stderr);
        exit(1);
    }

    while (1) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            fputs("accept() error\n", stderr);
            exit(1);
        }

        // Receive data from the client
        while(1){
            int str_len = read(clnt_sock, message, sizeof(message) - 1);
            if (str_len == -1) {
                fputs("read() error\n", stderr);
                break; // Exit the loop on read error
            } else if (str_len == 0) {
                // Connection closed by the client
                break; // Exit the loop on connection closed
            } else {
                message[str_len] = '\0'; // Null-terminate the received data
                printf("Message from client: %s\n", message);
                pthread_mutex_lock(&mutex);
                share_val += message[0] - '0';
                alret_val += message[2] - '0';
                pthread_mutex_unlock(&mutex);

            }
        }

        close(clnt_sock);
    }

    close(serv_sock);
    return NULL;
}
