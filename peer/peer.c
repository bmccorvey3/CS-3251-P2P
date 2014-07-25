#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>


// Globals
const char *g_szMessage;
const char *g_szServer;
unsigned short g_usPort;


// Function Prototypes
void parse_args(int argc, char **argv);
void setupSocket();


// Function Implementations

int main(int argc, char **argv)
{
  parse_args(argc, argv);
  setupSocket();
  //sendto(sock, g_szMessage, strlen(g_szMessage), 0, (struct sockaddr_in*)&addr, sizeof(addr)); 
  close(sock);

  return 0;
}

void setupSocket(){
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(g_usPort);
  inet_aton(g_szServer, &addr.sin_addr.s_addr);
  connect(sock, (struct sockaddr_in*)&addr,sizeof(addr));
}

void goOffline(){
  g_szMessage = "lvps";
  sendto(sock, )
}


void parse_args(int argc, char **argv)
{
  if (argc < 4)
  {
    fprintf(stderr,
            "Usage: tcp_client SERVER PORT \"MESSAGE\"\n");
    exit(1);
  }

  errno = 0;
  char *endptr = NULL;
  unsigned long ulPort = strtoul(argv[2], &endptr, 10);

  if (0 == errno)
  {
    // If no other error, check for invalid input and range
    if ('\0' != endptr[0])
      errno = EINVAL;
    else if (ulPort > USHRT_MAX)
      errno = ERANGE;
  }
  if (0 != errno)
  {
    // Report any errors and abort
    fprintf(stderr, "Failed to parse port number \"%s\": %s\n",
            argv[2], strerror(errno));
    abort();
  }
  g_usPort = ulPort;

  g_szServer = argv[1];
}
