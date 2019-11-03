//Client 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS 
 
#pragma comment(lib,"ws2_32.lib") //Required for WinSock 
#include <WinSock2.h>  
#include <string>  
#include <iostream>  
#include <stdio.h> 
#include <conio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <string.h> 
 
//DECLARATIONS FOR ENCRYPTION 
long int p, q, n, t, flag, e[100], d[100], temp1[100], temp2[100], j, m1[100], m2[100], en1[100], en2[100], i; char msg1[100], msg2[100]; int prime(long int); void ce(); long int cd(long int); void encrypt1(); void encrypt2(); void decrypt1(); void decrypt2(); 
//DECLARATION FOR ENCRYPTION ENDS 
 
//DECLARATION FOR WINSOCK STARTS 
SOCKET Connection;//This client's connection to the server 
 
void ClientThread() 
{ 
 int bufferlength;   while (true) 
	 	{ 
 	recv(Connection, (char*)&bufferlength, sizeof(int), NULL);  	char * buffer = new char[bufferlength + 1];   	buffer[bufferlength] = '\0';   	recv(Connection, buffer, bufferlength, NULL); 
	 	 	//Encrypting the input by user 
 
 	 	strcpy(msg2, buffer);  	 	for (i = 0; msg2[i] != NULL; i++) 
 	 	 	m2[i] = msg2[i];  	 	ce(); 
 	 	encrypt2();  	 	decrypt2();  	 	delete[] buffer;  
	 	} 
} 
 
int main() 
{ 
 	//Winsock Startup  	std::cout << "CLIENT" << std::endl; 
	 	WSAData wsaData; 
 	WORD DllVersion = MAKEWORD(2, 1);  	if (WSAStartup(DllVersion, &wsaData) != 0) 
	 	{ 
 	 	MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);  	 	return 0; 
	 	} 
 
 	//Taking IP Address as input by user  	std::string ipAddress; 
 	std::cout << "Type the IPv4 address of the server in dotted decimal notation\n" << std::endl;  	std::getline(std::cin, ipAddress);  	char ipAddress1[16];  	strcpy(ipAddress1, ipAddress.c_str());  	SOCKADDR_IN addr;  
 	int sizeofaddr = sizeof(addr);   	addr.sin_addr.s_addr = inet_addr(ipAddress1);   	addr.sin_port = htons(1111);   	addr.sin_family = AF_INET; //IPv4 Socket 
 
 	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //If we are unable to connect... 
{ 
 	MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);  	return 0; //Failed to Connect 
	 	} 
 	//CODING FOR ENCRYPTION STARTS  	p = 11;  	q = 17;  	n = p*q; 
	 	t = (p - 1)*(q - 1); 
	 	//CODING FOR ENCRYPTION ENDS 
	 	std::cout << "Connected!" << std::endl; 
 	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create the client thread that will receive any data that the server sends. 
 	std::string userinput;   	char Keyword[15];  	char terminator[] = "/TERMINATE";  	while (true) 
	 	{ 
 	 	std::getline(std::cin, userinput);   	 	int bufferlength = userinput.size();  
 
 	 	strcpy(msg1, userinput.c_str());  	 	for (i = 0; msg1[i] != NULL; i++) 
 	 	 	m1[i] = msg1[i];  	 	ce(); 
	 	 	encrypt1(); 
	 	 	//Encryption part Over, Now Sending the message 
 
 	 	send(Connection, (char*)&bufferlength, sizeof(int), NULL);   	 	send(Connection, userinput.c_str(), bufferlength, NULL);  
 	 	 	 	 	 	 	 	 	 	 	 
 	 	 	 	 //Code for disconneting Client  	 	if (bufferlength < 11) 
	 	 	{ 
   strcpy(Keyword, userinput.c_str());    if (strcmp(terminator, Keyword) == 0) 
	 	 	 	{ 
	 	 	 	 	break; 
	 	 	 	} 
	 	 	} 
	 	} 
	 	return 0; 
} 
//DEFINING FUNCTIONS FOR RSA ENCRYPTION 
int prime(long int pr) {  	int i;  	j = sqrt(pr);  	for (i = 2; i <= j; i++) {  	 	if (pr%i == 0) 
	 	 	 	return 0; 
	 	} 
	 	return 1; 
} 
void ce() { 
 	int k;  	k = 0; 
	 	for (i = 2; i<t; i++) { 
	 	 	if (t%i == 0) 
	 	 	 	continue; 
	 	 	flag = prime(i); 
	 	 	if (flag == 1 && i != p&&i != q) { 
	 	 	 	e[k] = i; 
 	 	 	flag = cd(e[k]);  	 	 	if (flag>0) {  	 	 	 	d[k] = flag; 
	 	 	 	 	k++; 
	 	 	 	} 
	 	 	 	if (k == 99) 
    break;   	} 
	 	} 
} 
long int cd(long int x) {  	long int k = 1; 
 
	 	while (1) { 
 	 	k = k + t;  	 	if (k%x == 0)  	 	 	return(k / x); 
	 	} 
} 
void encrypt1() {  	long int pt, ct, key = e[0], k, len;  	i = 0;  	len = strlen(msg1); 
	 	while (i != len) { 
 	 	pt = m1[i];  	 	pt = pt - 96;  	 	k = 1; 
	 	 	for (j = 0; j<key; j++) { 
 	 	 	k = k*pt;  	 	 	k = k%n; 
	 	 	} 
 	 	temp1[i] = k;  	 	ct = k + 96;  	 	en1[i] = ct; 
	 	 	i++; 
	 	} 
 	en1[i] = -1;  	printf("\nTHE ENCRYPTED MESSAGE IS\n");  	for (i = 0; en1[i] != -1; i++) 
 	 	printf("%c", en1[i]);  	printf("\n"); 
} 
void encrypt2() {  	long int pt, ct, key = e[0], k, len;  	i = 0;  	len = strlen(msg2);  	while (i != len) { 
 	 	pt = m2[i];  	 	pt = pt - 96;  	 	k = 1; 
 	 	for (j = 0; j<key; j++) {  	 	 	k = k*pt;  	 	 	k = k%n; 
	 	 	} 
 	 	temp2[i] = k;  	 	ct = k + 96;  	 	en2[i] = ct; 
	 	 	i++; 
	 	} 
 	en2[i] = -1;  	printf("\nTHE ENCRYPTED MESSAGE IS\n");  	for (i = 0; en2[i] != -1; i++) 
 	 	printf("%c", en2[i]);  	printf("\n"); 
} 
void decrypt1() {  	long int pt, ct, key = d[0], k;  	i = 0;  	while (en1[i] != -1) { 
 	 	ct = temp1[i];  	 	k = 1; 
	 	 	for (j = 0; j<key; j++) { 
 	 	 	k = k*ct;  	 	 	k = k%n; 
	 	 	} 
 	 	pt = k + 96;  	 	m1[i] = pt; 
	 	 	i++; 
	 	} 
 m1[i] = -1;  printf("\nTHE DECRYPTED MESSAGE IS\n");  for (i = 0; m1[i] != -1; i++) 
	 	 	printf("%c", m1[i]); 
} 
void decrypt2() {  	long int pt, ct, key = d[0], k;  	i = 0;  	while (en2[i] != -1) { 
 	 	ct = temp2[i];  	 	k = 1; 
 	 	for (j = 0; j<key; j++) {  	 	 	k = k*ct;  	 	 	k = k%n; 
	 	 	} 
 	 	pt = k + 96;  	 	m2[i] = pt; 
	 	 	i++; 
	 	} 
 m2[i] = -1;  printf("\nTHE DECRYPTED MESSAGE IS\n");  for (i = 0; m2[i] != -1; i++) 
 	 	printf("%c", m2[i]);  	printf("\n"); 
} 
