#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

int main()
{

    WSADATA wsaData;
    ADDRINFO  hints;
    ADDRINFO* addrResult;
    SOCKET ConnectSocket = INVALID_SOCKET;
    char recvBuffer[512];

    const char* sendBuffer = "Hello from client";

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cout << "WSAStartup failed result" << endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints)); //обнуление
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    getaddrinfo("localhost", "666", &hints, &addrResult);
    if (result != 0) {
        cout << "getaddrinfo failed with error" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype,
        addrResult->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Socket creation with" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    result = connect(ConnectSocket, addrResult->ai_addr,
        (int)addrResult->ai_addrlen);
    if (result == SOCKET_ERROR) {
        cout << "Unable connect to server" << endl;
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);

    if (result == SOCKET_ERROR) {
        cout << "Send failed, error" << result << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    cout << "Send:" << result << "bytes:" << endl;

    result = shutdown(ConnectSocket, SD_SEND);

    if (result == SOCKET_ERROR) {
        cout << "shutdown failed, error" << result << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    do {
        ZeroMemory(recvBuffer, 512);
        result = recv(ConnectSocket, recvBuffer, 512, 0);

        if (result > 0) {
            cout << "Received" << result << "bytes" << endl;
            cout << "Received data" << recvBuffer << endl;

        }
        else if (result == 0)
            cout << "Connection closwd" << endl;

    } while (result > 0);
    freeaddrinfo(addrResult);
    WSACleanup();
    return 0;


}