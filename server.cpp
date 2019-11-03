//Server 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS 
 
//Server 
 
#pragma comment(lib,"ws2_32.lib") 
#include <WinSock2.h> 
#include <string> 
#include <iostream> 
#include <stdio.h> 
#include <string.h> 
 
SOCKET Connections[100]; 
int TotalConnections = 0; 
 
void ClientHandlerThread(int index)  
{ 
 	int bufferlength;   	
	char Keyword[11];  	
	char terminator[] = "/TERMINATE";  	
	while (true) 
	 	{ 
 	 	recv(Connections[index], (char*)&bufferlength, sizeof(int), NULL);   	 	
		char * buffer = new char[bufferlength];  
 
 	 	recv(Connections[index], buffer, bufferlength, NULL);   	 	
		char * buf = new char(bufferlength + 1); 
 
	 	 	strcpy(buf, buffer); 
 
	 	 	buf[bufferlength] = '\0'; 
 
	 	 	if (bufferlength < 11) 
	 	 	{ 
 	 	 	std::cout << "Strcmp will be used" << std::endl;  	 	 	
			   if (strcmp(terminator, buf) == 0) 
	 	 	 	{ 
	 	 	 	 	delete[] buffer; 
	 	 	 	 	std::cout << "Strcmp successful" << std::endl; 
	 	 	 	 	break; 
	 	 	 	} 
	 	 	} 
	 	 	bufferlength++; 
	 	 	for (int i = 0; i < TotalConnections; i++)  
	 	 	{ 
	 	 	 	if (i == index) //Don't send the chat message to the same user who sent it 
	 	 	 	 	continue;  
 	 	 	send(Connections[i], (char*)&bufferlength, sizeof(int), NULL);  	 	 	
			send(Connections[i], buf, bufferlength, NULL); 
	 	 	} 
 	 	delete[] buffer; //Deallocate buffer to stop from leaking memory  	 	
		 bufferlength = 0; 
	 	} 
 	std::cout << "Closing the socket" << std::endl;  	
	 closesocket(Connections[index]);  	
	 std::cout << "Socket successfully closed exiting thread" << std::endl; 
} 
 
int main() 
{ 
	 	//Winsock Startup 
	 	WSAData wsaData; 
 	WORD DllVersion = MAKEWORD(2, 1);  	
	 if (WSAStartup(DllVersion, &wsaData) != 0) { 
 	 	MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0; 
	 	} 
 
 	//Taking IP Address as input by user  	
	 std::string ipAddress;  	
	 std::cout << "Type the IPv4 address of the server in dotted decimal notation\n" << std::endl; std::getline(std::cin, ipAddress); char ipAddress1[16]; strcpy(ipAddress1, ipAddress.c_str()); 
	 	SOCKADDR_IN addr;  
 	int addrlen = sizeof(addr);   	
	 addr.sin_addr.s_addr = inet_addr(ipAddress1);   	
	 addr.sin_port = htons(1111);   	
	 addr.sin_family = AF_INET; //IPv4 Socket 
 
 	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);   	
	 bind(sListen, (SOCKADDR*)&addr, sizeof(addr));   	
	 listen(sListen, SOMAXCONN);
	 //connection. Note:SOMAXCONN = Socket Oustanding Max Connections 
 
 	SOCKET newConnection; //Socket to hold the client's connection  	
	 int ConnectionCounter = 0;   	
	 for (int i = 0; i < 100; i++) 
	 	{ 
 	 	newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection  	 	
		  if (newConnection == 0) //If accepting the client connection failed 
	 	 	{ 
	 	 	 	std::cout << "Failed to accept the client's connection." << std::endl; 
	 	 	} 
	 	 	else //If client connection properly accepted 
	 	 	{ 
	 	 	 	std::cout << "Client Connected!" << std::endl; 
	 	 	 	Connections[i] = newConnection;  
	 	 	 	TotalConnections += 1;  
	 	 	 	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, 
(LPVOID)(i), NULL, NULL); 
 	 	 	std::string buftest = "MOTD: Welcome! This is the message of the day!.";  
			int size = buftest.size(); //Get size of message in bytes and store it in int size
			send(Connections[i], (char*)&size, sizeof(int), NULL);
			 //send Size of message  	 	 	
			 send(Connections[i], buftest.c_str(), buftest.size(), NULL); //send Message 
	 	 	} 
	 	} 
 	system("pause");  	
	 return 0; 
	} 	 
