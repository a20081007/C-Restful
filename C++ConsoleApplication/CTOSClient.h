#pragma once

#include <string>

class CTOSClient
{
public:
	std::wstring URL;
	std::string UserName;
	std::string Password;

public:
	CTOSClient(std::wstring url, std::string userName, std::string password);
	~CTOSClient();

	std::wstring FinishTesting(std::string sn);

	std::wstring GetCredentialToken(std::string user, std::string password);

};

