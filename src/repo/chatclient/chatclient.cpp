#include <errno.h> /*USING THE ERROR LIBRARY FOR FINDING ERRORS*/
#include <stdio.h> /*standard i/o*/
#include <unistd.h> /*FOR USING FORK for at a time send and receive messages*/ 
#include <malloc.h> /*FOR MEMORY ALLOCATION */
#include <string.h> /*using fgets funtions for geting input from user*/
#include <sys/socket.h> /*for creating sockets*/
#include <resolv.h> /*server to find out the runner's IP address*/ 
#include <netdb.h> /*definitions for network database operations */
#include <openssl/ssl.h> /*using openssl function's and certificates and configuring them*/
#include <openssl/err.h> /* helps in finding out openssl errors*/
#include <unistd.h>  /*FOR USING FORK for at a time send and receive messages*/ 
#define FAIL    -1 /*for error output == -1 */
#define BUFFER  1024  /*buffer for reading messages*/

// CLIENT PROGRAM

//code from: https://github.com/ElectronSz/Encrypted-Chat-Server-Using-C-Programming-and-OpenSSL-AES-DES

int OpenConnection(const char *hostname, int port) {   
	int sd;
	struct hostent *host;
	struct sockaddr_in addr;   /*creating the sockets*/

	if ( (host = gethostbyname(hostname)) == NULL ) {
		perror(hostname);
		abort();
	}
	sd = socket(PF_INET, SOCK_STREAM, 0);   /* setting the connection as tcp it creates endpoint for connection */
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = *(long*)(host->h_addr);
	
	if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )  /*initiate a connection on a socket*/ {
		close(sd);
		perror(hostname);
		abort();
	}
	return sd;
}

void LoadKey(SSL_CTX* ctx, char* KeyFile)   /* to load a certificate into an SSL_CTX structure*/ {
	
	/* set the private key from KeyFile (may be the same as CertFile) */
	if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 ) {
		ERR_print_errors_fp(stderr);
		abort();
	}

}

SSL_CTX* InitCTX(void)     /*creating and setting up ssl context structure*/ {   
	const SSL_METHOD *method;
	SSL_CTX *ctx;
	OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
	SSL_load_error_strings();   /* Bring in and register error messages */
	method = TLS_client_method();  /* Create new client-method instance */
	ctx = SSL_CTX_new(method);   /* Create new context */

	if ( ctx == NULL ) {
		ERR_print_errors_fp(stderr);
		abort();
	}
	return ctx;
}

void ShowCerts(SSL* ssl)  /*show the ceritficates to server and match them but here we are not using any client certificate*/ {   
	X509 *cert;
	char *line;
	cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
	if ( cert != NULL ) {
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

int main(int count, char *strings[])   /* getting port and ip as an argument*/ {   
	SSL_CTX *ctx;
	int server;
	SSL *ssl;
	char buf[1024];
	char input[BUFFER];
	int bytes;
	char *hostname, *portnum;
	pid_t cpid;     /* fork variable*/

	if ( count != 3 ) {
		printf("usage: %s  \n", strings[0]);
		exit(0);
	}
	SSL_library_init();   /*load encryption and hash algo's in ssl*/
	hostname=strings[1];
	portnum=strings[2];
	ctx = InitCTX();
	//SSL_CTX_set_ciphersuites(ctx, OSSL_default_ciphersuites());
	char certi[] = {'c', 'l', 'i', 'e', 'n', 't', '.', 'p', 'e', 'm', '\0'};
	LoadKey(ctx, certi); /* load certs */
	server = OpenConnection(hostname, atoi(portnum));   /*converting ascii port to interger */
	ssl = SSL_new(ctx);      /* create new SSL connection state */
	SSL_set_fd(ssl, server);    /* attach the socket descriptor */
	//SSL_set_ciphersuites(ssl, OSSL_default_ciphersuites());
	/* Host common name */
	// Not necessarily needed
	//SSL_set_tlsext_host_name(ssl, "Conor");
	
	if ( SSL_connect(ssl) == FAIL )   /* perform the connection */ {
	ERR_print_errors_fp(stderr);
	} else {
		printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
		ShowCerts(ssl);
		/* get any certs */
		cpid=fork();
		/*Fork system call is used to create a new process*/
		if(cpid==0) {
			while(1) {
				fflush(stdout);
				printf("\nMESSAGE TO SERVER:");
				fgets(input, BUFFER, stdin);
				SSL_write(ssl, input, strlen(input));   /* encrypt & send message */
			}
		} else {
			while(1) {
				bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */
				if ( bytes > 0 ) { 
					buf[bytes] = 0;
					printf("\nMESSAGE FROM SERVER: %s\n", buf);
					printf("\nMESSAGE TO SERVER:");
					fflush(stdout);
				}
			} 
		}
		SSL_free(ssl);        /* release connection state */
	}   
	close(server);         /* close socket */
	SSL_CTX_free(ctx);        /* release context */
	return 0;
}