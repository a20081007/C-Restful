// CConsoleApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CTOSClient.h"
int main()
{
	std::wstring url = L"http://172.20.2.3/";
	std::string sn = "testSN";
	std::string userName = "apiuser";
	std::string password = "advantech";

	CTOSClient client = CTOSClient(url, userName, password);
	client.FinishTesting(sn);
	return 0;
}

