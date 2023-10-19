#pragma once
#ifdef NETWORK_MODULE_EXPORTS
#define NETWORK_MODULE_API __declspec(dllexport)
#else
#define NETWORK_MODULE_API __declspec(dllimport)
#endif

//#include <Winsock2.h>

extern "C" NETWORK_MODULE_API SOCKET InitializeConnection(const char* serverIP, int port);
extern "C" NETWORK_MODULE_API int ReceiveData(SOCKET clientSocket, char* buffer, int bufferSize);
extern "C" NETWORK_MODULE_API int SendData(SOCKET clientSocket, const char* data);
extern "C" NETWORK_MODULE_API void Disconnect(SOCKET clientSocket);
