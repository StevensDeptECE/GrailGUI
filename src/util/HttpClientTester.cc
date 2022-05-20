/**
         curl-based client to test HTTP server performance
         this is derived from a sample of curl_easy_send

         see:
   https://raw.githubusercontent.com/curl/curl/master/docs/examples/sendrecv.c
         @author: Dov Kruger
         April 19, 2018
*/

#include <curl/curl.h>

#include <cstdlib>
#include <cstring>
#include <iostream>

bool verbose;
struct timeval tv;

using namespace std;
// Auxiliary function that waits on the socket. */
static int waitOnSocket(curl_socket_t sockfd, int for_recv) {
  fd_set infd, outfd, errfd;

  FD_ZERO(&infd);
  FD_ZERO(&outfd);
  FD_ZERO(&errfd);

  FD_SET(sockfd, &errfd); /* always check for error */

  if (for_recv) {
    FD_SET(sockfd, &infd);
  } else {
    FD_SET(sockfd, &outfd);
  }

  // select() returns the number of signalled sockets or -1
  return select((int)sockfd + 1, &infd, &outfd, &errfd, &tv);
}

void sendOneRequest(CURL* curl, curl_socket_t sockfd, const char* request,
                    size_t requestLen) {
  int res;
  size_t nsent_total = 0;
  if (verbose) cerr << "Sending request.\n";

  do {
    size_t nsent;
    do {
      nsent = 0;
      res = curl_easy_send(curl, request + nsent_total,
                           requestLen - nsent_total, &nsent);
      cout << "sent: " << nsent << "\n";
      nsent_total += nsent;

      if (res == CURLE_AGAIN && !waitOnSocket(sockfd, 0)) {
        cerr << "Error: timeout.\n";
      }
    } while (res == CURLE_AGAIN);

    if (res != CURLE_OK) {
      // cerr << "Error: " << curl_easy_strerror(res) << '\n';
    }
  } while (nsent_total < requestLen);
  if (verbose) cerr << "Reading response.\n";

  for (;;) {
    // Warning: This example program may loop indefinitely
    char buf[1024];
    size_t nread;
    do {
      nread = 0;
      res = curl_easy_recv(curl, buf, sizeof(buf), &nread);

      if (res == CURLE_AGAIN && !waitOnSocket(sockfd, 1)) {
        cerr << "Error: timeout.\n";
      }
    } while (res == CURLE_AGAIN);

    if (res != CURLE_OK) {
      cerr << "Error: " << res << "\n";  // curl_easy_strerror(res) << '\n';
    }

    if (nread == 0) {
      break;
    }
    if (verbose) cerr << "Read " << nread << " bytes.\n";
  }
}

int main(int argc, char* argv[]) {
  CURL* curl;
  CURLcode res;
  verbose = false;
  if (argc < 3) {
    cerr << "Usage: HttpClientTester url repeatcount\n";
    exit(-1);
  }
  const char* request = argv[1];
  uint32_t repeatCount = atoi(argv[2]);
  size_t requestLen = strlen(request);
  curl_socket_t sockfd;

  tv.tv_sec = 10;  // set timeout to 10 seconds
  tv.tv_usec = 0;

  /* A general note of caution here: if you're using curl_easy_recv() or
     curl_easy_send() to implement HTTP or _any_ other protocol libcurl
     supports "natively", you're doing it wrong and you should stop.

     This example uses HTTP only to show how to use this API, it does not
     suggest that writing an application doing this is sensible.
  */

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, request);
    // Do not do the transfer - only connect to host
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      cerr << "Error: " << curl_easy_strerror(res) << "\n";
    }

    // Extract the socket from the curl handle for waiting
    res = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sockfd);

    if (res != CURLE_OK) {
      cerr << "Error: " << res << '\n';
      return 1;
    }
    for (int i = 0; i < repeatCount; i++)
      sendOneRequest(curl, sockfd, request, requestLen);

    curl_easy_cleanup(curl);
    return 0;
  }
  cerr << "Curl init failed\n";
  return -1;
}
