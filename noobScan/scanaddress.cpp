/**/
/*
   scanaddress.cpp of noobScan, a novice port scanner
   scanaddress.cpp
*/
/**/

#include "scanaddress.h"


//    for (size_t port = 0; port < 1024; port++) {
//
//        // Set up info for the getaddrinfo request
//        char sport[10];
//        // Connect to this port number
//        sprintf(sport, "%zu", port);
//        struct addrinfo hints = {0};
//
//        // Use either IPv4 or IPv6, we're OK with either.
//        hints.ai_family = AF_UNSPEC;
//
//        // Must be TCP. connect() doesn't tell us anything for a UDP 'connection'.
//        hints.ai_socktype = SOCK_STREAM;
//
//        // Retrieve address info's. If this returns failure, you probably mistyped the address or domain name.
//        struct addrinfo* addr;
//        if (getaddrinfo(argv[1], sport, &hints, &addr) != 0) {
//            printf("Target host could not be looked up");
//            return -1;
//        }
//
//        // Assuming we get here, we'll connect to the first result coming out.
//        // Normally you'd retry to the other results from getaddrinfo, but
//        // 1. I'm lazy and
//        // 2. we actually want the error message more.
//        int sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
//        int res = connect(sock, addr->ai_addr, addr->ai_addrlen);
//
//        // If it failed, check what kind of failure to determine the state of the port
//        if (res == -1) {
//            switch(errno) {
//                case ETIMEDOUT:
//                    printf("Port %zu is shadowed\n", port);
//                    break;
//                case ECONNREFUSED:
//                    printf("Port %zu is closed\n", port);
//                    break;
//                case ENETUNREACH:
//                    printf("Cannot reach the target host\n");
//                    return -1;
//            }
//        }
//
//        // Otherwise, I guess it worked.
//        else {
//            printf("Port %zu is open\n", port);
//        }
//
//        // Clean up and go back for another port connect attempt.
//        close(sock);
//        freeaddrinfo(addr);
//        }
