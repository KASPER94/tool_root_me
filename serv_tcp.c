#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

const char* host = "challenge01.root-me.org";
int port = 52002;

int *find_nb(char *line) {
	int *nbs = malloc(sizeof(int) * 3);
	char *math_line = strstr(line, "Calculate the square root of");
    if (!math_line) {
        free(nbs);
        return NULL;
    }
    char *word = strstr(math_line, "of");
    if (!word) {
        free(nbs);
        return NULL;
    }
    word += 3;
    int count = 0;
    while (*word && count < 2) {
        if (isdigit(*word)) {
            nbs[count] = strtol(word, &word, 10);
            count++;
        } else {
            word++;
        }
    }

    if (count < 2) {
        free(nbs);
        return NULL;
    }

    return nbs;
}

int main(void) {
	struct sockaddr_in servaddr;
	struct hostent *entry_host = gethostbyname(host);
	char buf_recv[BUFFER_SIZE];
	char buf_send[BUFFER_SIZE];

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr = *((struct in_addr*)entry_host->h_addr);
	servaddr.sin_port = htons(port);

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) return (1);

	if (connect(fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("connection failed");
        return 1;
    }

	// AUTOMATED SCRIPT TO SUCCED THE CHALLENGE
	ssize_t ret = recv(fd, buf_recv, sizeof(buf_recv), 0);
	if (ret < 0) {
        perror("recv failed");
        return 1;
    }
    buf_recv[ret] = '\0';

	printf("%s", buf_recv);
	int *result = find_nb(buf_recv);
	double calcul = sqrt(result[0]) * result[1];
	printf("%f\n", calcul);
    snprintf(buf_send, sizeof(buf_send), "%.2f\n", calcul);

    if (send(fd, buf_send, strlen(buf_send), 0) < 0)
        return (1);

    ret = recv(fd, buf_recv, sizeof(buf_recv), 0);
    if (ret < 0) {
        perror("recv failed");
        return 1;
    }
    buf_recv[ret] = '\0';
    printf("%s\n", buf_recv);
	free(result);

	// INTERACTIVE SCRIPT IMPOSSIBLE TO RESPONSE BUT CAN GET THE QUESTION
	// while (1) {
	// 	ssize_t ret = recv(fd, buf_recv, sizeof(buf_recv), 0);
	// 	if (ret >= 0) {
	// 		if (ret == 0)
	// 			break;
	// 		buf_recv[ret] = '\0';
	// 		printf("%s\n", buf_recv);
	// 	}
		// printf("Enter your response:");
		// if (fgets(buf_send, sizeof(buf_send), stdin) == NULL)
		// 	break;
		
		// if (send(fd, buf_send, strlen(buf_send), 0) < 0)
		// 	break;
	// }

	close(fd);
	return (0);	
}