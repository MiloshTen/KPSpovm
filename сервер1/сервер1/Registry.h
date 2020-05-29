#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <windows.h>
#include <conio.h>
#include <iostream>
#pragma hdrstop
#include <string.h>
#include <stdio.h>
#define MAX_VALUE_NAME 16383
using namespace std;

class regedit
{
	HKEY hKey;//�������� ������
	::PHKEY phkResult;//��������� �����
	REGSAM samDesired;//��� �������
	CHAR *cBuf;//�������� �����
public:
	regedit(HKEY hKey);//����������� � ����������, ����������� � ���-�� ��������� ��� �����
	~regedit();
	void QueryKey(HKEY hKey, LPVOID param);//�����, �������������� ������ ��������
	void WriteKey(HKEY hKey, LONG lRet, char *s, LPVOID param);//�����, �������������� ������ ��������
};
regedit::regedit(HKEY hKey)
{
	LONG lRet = 0;
	lRet = RegCreateKeyEx(HKEY_CURRENT_USER, L"MyKey", 0, L"", 0,
		KEY_READ | KEY_WRITE, NULL, &hKey, NULL);
};
regedit::~regedit()
{
	RegCloseKey(hKey);
};
void regedit::WriteKey(HKEY hKey, LONG lRet, char *s, LPVOID param)
{
	SOCKET sock = *(SOCKET*)param;
	CHAR buf[MAX_PATH] = { 0 };
	if (hKey)
	{
		strcpy(buf, s);
		lRet = RegSetValueEx(hKey, L"MyValue", 0, REG_SZ,
			(LPBYTE)&buf, strlen(buf));
		if (lRet != ERROR_SUCCESS)
			printf("Error writing value to registry");
		else
			printf("Data '%s' is written to registry", buf);
	};
};
void regedit::QueryKey(HKEY hKey, LPVOID param)
{
	SOCKET sock = *(SOCKET*)param;
	TCHAR achKey[MAX_PATH] = { 0 }; // ����� ��� ����� �����
	DWORD cValues; // ���������� �������� �����
	DWORD i, retCode;
	DWORD cchValue = MAX_VALUE_NAME;
	TCHAR tmp[MAX_PATH] = { 0 }; //����� ��� ��� ��������
								 //�������� ���������� �������� �����
	retCode = RegQueryInfoKey(
		hKey, NULL, NULL, NULL, NULL, NULL,
		NULL, &cValues, NULL, NULL, NULL, NULL);
	//����������� �������� ������
	if (cValues)
	{

		cout << "\nNumber of values: " << cValues<< endl;
		for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
		{
			cchValue = MAX_VALUE_NAME;
			achKey[0] = '\0';
			DWORD dwType;
			DWORD dwReturnLength = MAX_PATH;
			retCode = RegEnumValue(hKey,
				i,
				achKey,
				&cchValue,
				NULL,
				NULL,
				NULL,
				NULL);
			if (retCode == ERROR_SUCCESS)
			{
				//������ ��� ������ �������� �����
				retCode = RegQueryValueEx(hKey,
					achKey,
					NULL,
					&dwType,
					NULL,
					NULL);
				//���� ������, ����� ������ �� � ������� �� �����
				if ((retCode == ERROR_SUCCESS) && (dwType == REG_SZ))
				{
					cchValue = MAX_VALUE_NAME;
					retCode = RegEnumValue(hKey,
						i, achKey, &cchValue, NULL, &dwType,
						(LPBYTE)&tmp,
						&dwReturnLength);
					if (retCode == ERROR_SUCCESS)
						cout << achKey<<" = " << tmp << endl;
				}
			}
		}
	}
}