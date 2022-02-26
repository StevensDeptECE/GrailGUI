//#include <openssl/applink.c>
#include <unistd.h> /*FOR USING FORK for at a time send and receive messages*/ 
#include <errno.h>   /*USING THE ERROR LIBRARY FOR FINDING ERRORS*/
#include <malloc.h>  /*FOR MEMORY ALLOCATION */
#include <cstring>  /*using fgets funtions for geting input from user*/
//#include <arpa/inet.h>  /*for using ascii to network bit*/ 
#include <sys/socket.h>
//#include <sys/types.h>
#include <netinet/in.h>        /* network to asii bit */
//#include <resolv.h>  /*server to find out the runner's IP address*/ 
#include <cstdio>   /*standard i/o*/
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <inttypes.h>
#define FAIL    -1  /*for error output == -1 */
#define BUFFER 1024  /*buffer for reading messages*/

// SERVER PROGRAM

void InitializeSSL() {
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
}

void DestroySSL() {
    ERR_free_strings();
    EVP_cleanup();
}

void ShutdownSSL(SSL* cSSL) {
    SSL_shutdown(cSSL);
    SSL_free(cSSL);
}

bool str_to_uint16(char *str, uint16_t *res) {
    char *end;
    errno = 0;
    intmax_t val = strtoimax(str, &end, 10);
	if (errno == ERANGE || val < 0 || val > UINT16_MAX || end == str || *end != '\0') {
		return false;
	}
	*res = (uint16_t) val;
	return true;
}

// TODO: dead code
SSL* OpenListener(uint16_t aPortNumber)   {   
	
	InitializeSSL();
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd< 0) {
		//Log and Error
		exit(1);
	}
	struct sockaddr_in saiServerAddress;
	bzero((char *) &saiServerAddress, sizeof(saiServerAddress));
	saiServerAddress.sin_family = AF_INET;
	saiServerAddress.sin_addr.s_addr = INADDR_ANY;
	saiServerAddress.sin_port = htons(aPortNumber);

	bind(sockfd, (struct sockaddr *) &saiServerAddress, sizeof(saiServerAddress));

	struct sockaddr_in cli_addr;
	socklen_t len = sizeof(cli_addr);
	listen(sockfd,5);
	int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &len);

	SSL_CTX *sslctx = SSL_CTX_new( TLS_server_method());
	
	// open server side cert, c
	int use_cert = SSL_CTX_use_certificate_file(sslctx, "server.pem", SSL_FILETYPE_PEM);
	if (use_cert == 0) {
		ERR_print_errors_fp(stderr);
		exit(-1);
	}
	
	// TODO: find out what the private key is for
	int use_prv = SSL_CTX_use_PrivateKey_file(sslctx, "private-key.pem", SSL_FILETYPE_PEM);
	if (use_prv == 0) {
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	SSL *cSSL = SSL_new(sslctx);
	SSL_set_fd(cSSL, newsockfd );
	//TODO: figure out if these are needed 
	//SSL_CTX_set_ciphersuites(sslctx, cipher_list.c_str());
	//SSL_set_ciphersuites(cSSL, cipher_list.c_str());
	return cSSL;
}

void Servlet(SSL* ssl) /* Serve the connection -- threadable */ {   
	char buf[1024];
	int sd, bytes;
	char input[BUFFER];  
	pid_t cpid; 
	//Here is the SSL Accept portion.  Now all reads and writes must use SSL
	int ret = SSL_accept(ssl);
	printf("%d\n", ret);
	if (ret == 0) {
        /* Hard error */
        exit(-1);
    } else if ( ret == -1 )     /* do SSL-protocol accept */ {
		printf("Handshake Error %d\n", SSL_get_error(ssl, ret));
		ERR_print_errors_fp(stderr);
		exit(-1);
	}
	
	/*Fork system call is used to create a new process*/
	cpid=fork();
	if(cpid==0) { 
		while(1) {
			bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request and read message from server*/
			if ( bytes > 0 ) { 
				buf[bytes] = 0;
				printf("\nMESSAGE FROM CLIENT: %s\n", buf);
				printf("\nMESSAGE TO CLIENT:");
				fflush(stdout);
			} else {
				ERR_print_errors_fp(stderr);
			}
		} 
	} else {
		while(1) {
			fflush(stdout);
			printf("\nMESSAGE TO CLIENT:");
			fgets(input, BUFFER, stdin);  /* get request and reply to client*/
			SSL_write(ssl, input, strlen(input)); 
		}  
	}

	sd = SSL_get_fd(ssl);       /* get socket connection */
	SSL_free(ssl);         /* release SSL state */
	close(sd);          /* close connection */
}

int main(int count, char *strings[])   /* getting port as a argument*/ {   
	
	if ( count != 2 ) {
		printf("Usage: %s <portnum> \n", strings[0]);   /*send the usage guide if syntax of setting port is different*/
		exit(0);
	}
	
	char *portnum = strings[1];
	uint16_t aPortNumber;
	if (str_to_uint16(portnum, &aPortNumber) == false) {
		printf("Port number must be between 0 and 65535.");
		exit(1);
	}
<<<<<<< HEAD
	SSL *cSSL = OpenListener(aPortNumber);
	
=======
	InitializeSSL();
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd< 0) {
		//Log and Error
		exit(1);
	}
	struct sockaddr_in saiServerAddress;
	bzero((char *) &saiServerAddress, sizeof(saiServerAddress));
	saiServerAddress.sin_family = AF_INET;
	saiServerAddress.sin_addr.s_addr = INADDR_ANY;
	saiServerAddress.sin_port = htons(*aPortNumber);

	bind(sockfd, (struct sockaddr *) &saiServerAddress, sizeof(saiServerAddress));

	struct sockaddr_in cli_addr;
	socklen_t len = sizeof(cli_addr);
	listen(sockfd,5);
	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &len);

	sslctx = SSL_CTX_new( TLS_server_method());
	
	// check return values vvvv
	int use_cert = SSL_CTX_use_certificate_file(sslctx, "server.pem" , SSL_FILETYPE_PEM);
	if (use_cert == 0) {
		ERR_print_errors_fp(stderr);
		exit(-1);
	}
#if 0
	//TODO: do we need a private user key for anything?
	int use_prv = SSL_CTX_use_PrivateKey_file(sslctx, "private-key.pem", SSL_FILETYPE_PEM);
	if (use_prv == 0) {
		ERR_print_errors_fp(stderr);
		exit(-1);
	}
#endif

	cSSL = SSL_new(sslctx);
	SSL_set_fd(cSSL, newsockfd );
	//SSL_CTX_set_ciphersuites(sslctx, cipher_list.c_str());
	//SSL_set_ciphersuites(cSSL, cipher_list.c_str());
>>>>>>> 019487dfc288f3be441627de5986e3c3425eb12c
	Servlet(cSSL);
	ShutdownSSL(cSSL);
	return 0;
}
