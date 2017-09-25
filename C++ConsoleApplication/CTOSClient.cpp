#include "stdafx.h"
#include "CTOSClient.h"

#include <iostream>
#include <string>

#include <cpprest\http_client.h>
#include <cpprest\filestream.h>
#include <cpprest\filestream.h>

using namespace std;
using namespace web::http;
using namespace web::http::client;
using namespace Concurrency::streams;


CTOSClient::CTOSClient(std::wstring url, std::string userName, std::string password)
{
	this->URL = url;
	this->UserName = userName;
	this->Password = password;
}

CTOSClient::~CTOSClient()
{
}

std::wstring s2ws(const std::string& s)
{
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, "C");
	return result;
}

std::string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, _Source, _Dsize);
	std::string result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::wstring CTOSClient::FinishTesting(std::string sn)
{
	// result message
	wstring message = L"";

	// get api token first
	wstring token = GetCredentialToken(this->UserName, this->Password);

	// setup request
	wstring baseUrl = this->URL + (L"/api/passmark/FinishTesting/" + s2ws(sn));
	http_client httpClient(baseUrl);
	http_request req(methods::GET);
	req.headers().add(L"cache-control", L"no-cache");
	req.headers().add(L"authorization", L"Bearer " + token);
	req.headers().add(L"content-type", L"application/json");

	// get response
	http_response httpResponse = httpClient.request(req).get();
	message = httpResponse.extract_utf16string().get();

	return message;
}

std::wstring CTOSClient::GetCredentialToken(std::string user, std::string password)
{
	wstring token = L"";

	// setup request
	wstring baseUrl = this->URL + L"/token";
	http_client httpClient(baseUrl);
	http_request req(methods::POST);
	req.headers().add(L"cache-control", L"no-cache");
	req.headers().add(L"content-type", L"application/x-www-form-urlencoded");
	req.set_body("grant_type=password&username=" + user + "&password=" + password);

	// send request 
	http_response httpResponse = httpClient.request(req).get();

	// get response
	if (httpResponse.status_code() == status_codes::OK)
	{
		web::json::value jsonToken = httpResponse.extract_json().get();
		web::json::value responseToken = jsonToken.at(U("access_token"));
		token = responseToken.as_string();
	}
	else
	{
		// TODO error handling
	}

	return token;
}