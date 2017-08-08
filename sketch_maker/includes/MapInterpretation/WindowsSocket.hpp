#ifndef WINDOWSSOCKET_13042016
#define WINDOWSSOCKET_13042016

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <math.h>
#include <errno.h>

#define DEFAULT_PORT 8861

#define DEBUG_IN_FILE if (debug_output) printf
const int debug_output = 0;

inline long min(long num1, long num2) { if (num1 <= num2) return num1; else return num2; }

inline bool senddata(int sockfd, void *buf, int buflen)
{
    const char *pbuf = (const char *) buf;

    while (buflen > 0)
    {
        int num = send(sockfd, pbuf, buflen, 0);
	DEBUG_IN_FILE("senddata sent %d bytes\n", num);
        if (num <= 0)
        {
            return false;
        }

        pbuf += num;
        buflen -= num;
    }
DEBUG_IN_FILE("senddata finished\n");
    return true;
}

inline bool sendlong(int sockfd, long value)
{
    value = htonl(value);
    DEBUG_IN_FILE("sending long: %ld\n", value);
    return senddata(sockfd, &value, 8);//sizeof(value));
}

inline bool sendfile(int sockfd, FILE *f)
{
printf("sending file\n");
    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    rewind(f);
    if (filesize == EOF)
        return false;
DEBUG_IN_FILE("sending filesize...\n");
    if (!sendlong(sockfd, filesize))
        return false;
DEBUG_IN_FILE("sending file contents...\n");
    if (filesize > 0)
    {
        char buffer[1024];
        do
        {
            size_t num = min(filesize, sizeof(buffer));
            num = fread(buffer, 1, num, f);
            if (num < 1)
		{
			printf("sendfile failed(1)\n");
	                return false;
		}
            if (!senddata(sockfd, buffer, num))
		{
			printf("sendfile failed(2)\n");
	                return false;
		}
            filesize -= num;
        }
        while (filesize > 0);
    }
printf("done sending file\n");
    return true;
}

inline bool readdata(int sockfd, void *buf, int buflen)
{
    char *pbuf = (char *) buf;

    while (buflen > 0)
    {
        int num = recv(sockfd, pbuf, buflen, 0);
        if (num <= 0)
	{
		printf("readdata error: %d, %d\n", num, errno);
            return false;
	}
        pbuf += num;
        buflen -= num;
DEBUG_IN_FILE("readdata read %d bytes\n", num);
    }

DEBUG_IN_FILE("readdata finished\n");
    return true;
}

inline bool readlong(int sockfd, long *value)
{
    if (!readdata(sockfd, value, 8))//sizeof(value)))
	{
		printf("error reading long\n");
        	return false;
	}
    *value = ntohl(*value);
DEBUG_IN_FILE("readlong read %ld\n", *value);
    return true;
}

inline bool readfile(int sockfd, FILE *f)
{
	printf("reading file...\n");
    long filesize;
DEBUG_IN_FILE("reading filesize...\n");
    if (!readlong(sockfd, &filesize))
        return false;
DEBUG_IN_FILE("reading file contents...\n");
    if (filesize > 0)
    {
        char buffer[1024];
        do
        {
            int num = min(filesize, sizeof(buffer));
            if (!readdata(sockfd, buffer, num))
                return false;
            int offset = 0;
            do
            {
                size_t written = fwrite(&buffer[offset], 1, num-offset, f);
                if (written < 1)
		{
			printf("error writing read file: %d, %d\n", (int)written, errno);
                    return false;
		}
                offset += written;
            }
            while (offset < num);
            filesize -= num;
		DEBUG_IN_FILE("remaining size: %ld\n", filesize);
        }
        while (filesize > 0);
    }
printf("done reading file.\n");
    return true;
}

inline bool receive_file(int sockfd, const char* filename)
{
DEBUG_IN_FILE("receiving file\n");
	FILE *filehandle = fopen(filename, "wb");
	bool ok = false;
	if (filehandle != NULL)
	{
		ok = readfile(sockfd, filehandle);
		fclose(filehandle);

		if (ok)
		{
			// use file as needed...
		}
		else
			remove(filename);
	}
	return ok;
}

inline void send_file(int sockfd, const char* filename)
{
DEBUG_IN_FILE("sending file\n");
	FILE *filehandle = fopen(filename, "rb");
	if (filehandle != NULL)
	{
		sendfile(sockfd, filehandle);
		fclose(filehandle);
	}
}


#endif