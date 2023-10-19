#include <iostream>
#include <string>
#include <Winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

extern "C" __declspec(dllexport) SOCKET InitializeConnection(const char* serverIP, int port);
extern "C" __declspec(dllexport) int ReceiveData(SOCKET clientSocket, char* buffer, int bufferSize);
extern "C" __declspec(dllexport) int SendData(SOCKET clientSocket, const char* data);
extern "C" __declspec(dllexport) void Disconnect(SOCKET clientSocket);

SOCKET clientSocket;

extern "C" __declspec(dllexport) SOCKET InitializeConnection(const char* serverIP, int port) {
    // 初始化Winsock库
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return INVALID_SOCKET;
    }

    // 创建客户端套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        WSACleanup();
        return INVALID_SOCKET;
    }

    // 设置服务器地址信息
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, serverIP, &serverAddr.sin_addr) <= 0) {
        closesocket(clientSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // 连接到服务器
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(clientSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    return clientSocket;
}

extern "C" __declspec(dllexport) int ReceiveData(SOCKET clientSocket, char* buffer, int bufferSize) {
    int bytesRead = recv(clientSocket, buffer, bufferSize, 0);
    if (bytesRead == SOCKET_ERROR) {
        return -1; // 接收数据出错
    }

    return bytesRead;
}

extern "C" __declspec(dllexport) int SendData(SOCKET clientSocket, const char* data) {
    int bytesSent = send(clientSocket, data, strlen(data), 0);
    if (bytesSent == SOCKET_ERROR) {
        return -1; // 发送数据出错
    }

    return bytesSent;
}

extern "C" __declspec(dllexport) void Disconnect(SOCKET clientSocket) {
    closesocket(clientSocket);
    WSACleanup();
}
