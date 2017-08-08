#include "MapMakerExpertWindows.h"



AASS::MapMakerExpertWindows::MapMakerExpertWindows(int x, int y, const QString& path) : MapMakerExpert(x, y, path) 
{
	client_socket = INVALID_SOCKET;
	recvbuflen = DEFAULT_BUFLEN;
}

AASS::MapMakerExpertWindows::~MapMakerExpertWindows()
{
}

int AASS::MapMakerExpertWindows::init_client_socket()
{
	
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char *sendbuf = "this is a test";
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(SERVER_NAME, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        client_socket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (client_socket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = ::connect(client_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(client_socket);
            client_socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (client_socket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

	return 0;
}

int AASS::MapMakerExpertWindows::close_client_socket()
{
	// shutdown the connection since no more data will be sent
    int iResult = shutdown(client_socket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(client_socket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
            printf("Bytes received: %d\n", iResult);
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );

    // cleanup
    closesocket(client_socket);
    WSACleanup();
}

bool AASS::MapMakerExpertWindows::senddata(SOCKET sock, void *buf, int buflen)
{
	DEBUG("sending data\n");
    const char *pbuf = (const char *) buf;

    while (buflen > 0)
    {
        int num = send(sock, pbuf, buflen, 0);
        if (num == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAEWOULDBLOCK)
            {
                // optional: use select() to check for timeout to fail the send
                continue;
            }
            return false;
        }

		DEBUG("sent %d bytes\n", num);
        pbuf += num;
        buflen -= num;
    }

	DEBUG("senddata finished\n");
    return true;
}

bool AASS::MapMakerExpertWindows::sendlong(SOCKET sock, long value)
{
    value = htonl(value);
	DEBUG("sending long %d\n", value);
    return senddata(sock, &value, 8);//sizeof(value));
}

bool AASS::MapMakerExpertWindows::sendfile(SOCKET sock, FILE *f)
{
	printf("sending file\n");
    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    rewind(f);
    if (filesize == EOF)
        return false;
	DEBUG("sending file size\n");
    if (!sendlong(sock, filesize))
        return false;
	DEBUG("sending file contents\n");
    if (filesize > 0)
    {
        char buffer[1024];
        do
        {
            size_t num = min(filesize, sizeof(buffer));
            num = fread(buffer, 1, num, f);
            if (num < 1)
			{
				printf("sending file failed(1)\n");
                return false;
			}
            if (!senddata(sock, buffer, num))
			{
				printf("sending file failed(2)\n");
                return false;
			}
            filesize -= num;
        }
        while (filesize > 0);
    }
	printf("done sending file\n");
    return true;
}

bool AASS::MapMakerExpertWindows::readdata(SOCKET sock, void *buf, int buflen)
{
    char *pbuf = (char *) buf;

    while (buflen > 0)
    {
        int num = recv(sock, pbuf, buflen, 0);
        if (num == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAEWOULDBLOCK)
            {
                // optional: use select() to check for timeout to fail the read
                continue;
            }
			printf("readdata aborted(1): %d\n", WSAGetLastError());
            return false;
        }
        else if (num == 0)
		{
			printf("readdata aborted(2)\n");
            return false;
		}

        pbuf += num;
        buflen -= num;
		DEBUG("readdata received %d bytes\n", num);
    }
	DEBUG("readdata finished");
    return true;
}

bool AASS::MapMakerExpertWindows::readlong(SOCKET sock, long *value)
{
    if (!readdata(sock, value, 8)) //sizeof(value)))
        return false;
    *value = ntohl(*value);
	DEBUG("readlong received %d\n", *value);
    return true;
}

bool AASS::MapMakerExpertWindows::readfile(SOCKET sock, FILE *f)
{
	printf("receiving file\n");
    long filesize;
	DEBUG("receiving filesize\n");
    if (!readlong(sock, &filesize))
        return false;
	DEBUG("receiving file contents\n");
    if (filesize > 0)
    {
        char buffer[1025];
        do
        {
            int num = min(filesize, sizeof(buffer)-1);
            if (!readdata(sock, buffer, num))
                return false;
            int offset = 0;
            do
            {
                size_t written = fwrite(&buffer[offset], 1, num-offset, f);
                if (written < 1)
				{
					printf("error writing received file\n");
                    return false;
				}
                offset += written;
            }
            while (offset < num);
            filesize -= num;
			DEBUG("remaining size: %d\n", filesize);
        }
        while (filesize > 0);
    }
	printf("done receiving file\n");
    return true;
}

void AASS::MapMakerExpertWindows::receive_file(SOCKET sock, const char* filename)
{
	FILE *filehandle = fopen(filename, "wb");
	if (filehandle != NULL)
	{
		bool ok = readfile(sock, filehandle);
		fclose(filehandle);

		if (ok)
		{
			// use file as needed...
		}
		else
			remove(filename);
	}
}

void AASS::MapMakerExpertWindows::send_file(SOCKET sock, const char* filename)
{
	FILE *filehandle = fopen(filename, "rb");
	if (filehandle != NULL)
	{
		sendfile(sock, filehandle);
		fclose(filehandle);
	}
}


//Update the result image
void AASS::MapMakerExpertWindows::Sysexport()
{
	MapMakerExpert::Sysexport();
	
	// initialize socket to exchange files with PC
	int ret = 1;
	int tries = 0;
	while (ret != 0 && tries < 5)
	{
		ret = init_client_socket();
		if (ret != 0)
		{
			printf("Error creating socket! Check if the server is reachable. Retrying in 5 seconds...\n");
			Sleep(2000);
			tries++;
		}
	}
	
	if (ret == 0)
	{
		// send the files to the pc for processing
		QString file1 = _path_for_export + "/obstaclemap.png";
		QString file2 = _path_for_export + "/path.png";
		QString file3 = _path_for_export + "/keypoints.txt";
		
		send_file(client_socket, file1.toStdString().c_str());
		send_file(client_socket, file2.toStdString().c_str());
		send_file(client_socket, file3.toStdString().c_str());
		
		// ...and receive result images (blocking - wait until processing is done)
		receive_file(client_socket, _path_to_result.toStdString().c_str());	
		
		// close socket
		close_client_socket();
	}
}



#include "MapMakerExpertWindows.moc"
