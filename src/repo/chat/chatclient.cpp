#include <unistd.h> // FOR USING FORK for at a time send and receive messages
#include <string.h>
#include <netdb.h> // definitions for network database operations
#include <openssl/ssl.h>
#include <openssl/err.h>
#define BUFFER  1024  // buffer for reading messages

//code from: https://github.com/ElectronSz/Encrypted-Chat-Server-Using-C-Programming-and-OpenSSL-AES-DES

int OpenConnection(const char *hostname, int port)
{
	struct hostent *host;
	struct sockaddr_in addr;   /*creating the sockets*/

	if (!(host = gethostbyname(hostname))) {
		perror(hostname);
		abort();
	}
	int sd = socket(PF_INET, SOCK_STREAM, 0);   /* setting the connection as tcp it creates endpoint for connection */
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = *(long*)(host->h_addr);

	if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)))  /*initiate a connection on a socket*/ {
		close(sd);
		perror(hostname);
		abort();
	}
	return sd;
}

SSL_CTX* InitCTX()     /*creating and setting up ssl context structure*/
{
	OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
	SSL_load_error_strings();   /* Bring in and register error messages */
	const SSL_METHOD *method = TLS_client_method();  /* Create new client-method instance */
	SSL_CTX *ctx = SSL_CTX_new(method);   /* Create new context */

	if (!ctx) {
		ERR_print_errors_fp(stderr);
		abort();
	}
	return ctx;
}

// show the ceritficates to server and match them but here we are not using any client certificate
void ShowCerts(SSL* ssl)
{
	X509 *cert;
	char *line;
	cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
	if (cert) {
		printf("Server certificates:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("Subject: %s\n", line);
		free(line);       /* free the malloc'ed string */
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("Issuer: %s\n", line);
		free(line);       /* free the malloc'ed string */
		X509_free(cert);     /* free the malloc'ed certificate copy */
	} else {
		printf("Info: No client certificates configured.\n");
	}
}

int main(int argc, char *strings[])
{
	char buf[1024];
	char input[BUFFER];
	int bytes;
	pid_t cpid;     /* fork variable*/

	if (argc != 3) {
		printf("usage: %s  \n", strings[0]);
		return EXIT_FAILURE;
	}
	SSL_library_init();   /*load encryption and hash algo's in ssl*/
	char *hostname = strings[1];
	char *portnum = strings[2];
	SSL_CTX *ctx = InitCTX();
	int server = OpenConnection(hostname, atoi(portnum));   /*converting ascii port to interger */
	SSL *ssl = SSL_new(ctx);      /* create new SSL connection state */
	SSL_set_fd(ssl, server);    /* attach the socket descriptor */

	if (SSL_connect(ssl) == -1)   /* perform the connection */ {
		ERR_print_errors_fp(stderr);
	} else {
		printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
		ShowCerts(ssl);
		/* get any certs */
		/*Fork system call is used to create a new process*/
		if (!(cpid = fork())) {
			while(1) {
				printf("\nMESSAGE TO SERVER:");
				fgets(input, BUFFER, stdin);
				SSL_write(ssl, input, strlen(input));   /* encrypt & send message */
			}
		} else {
			while(1) {
				bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */
				if ( bytes > 0 ) {
					buf[bytes] = 0;
					printf("\nMESSAGE FROM CLIENT: %s\n", buf);
				}
			}
		}
		SSL_free(ssl);        /* release connection state */
	}
	close(server);         /* close socket */
	SSL_CTX_free(ctx);        /* release context */
	return 0;
}
