#pragma once

#include <string>
#include <vector>
#include <Poco\DigestEngine.h>
#include <concurrent_vector.h>

std::string W2A_UTF8(const std::wstring& strIn);
std::wstring A2W_UTF8(const std::string& strIn);

#ifndef NOWINDOWS
std::string W2A_GB2312(const std::wstring& strIn);
std::wstring A2W_GB2312(const std::string& strIn);
std::string W2A_ACP(const std::wstring& strIn);
std::wstring A2W_ACP(const std::string& strIn);
#endif

std::string BuildMd5(const std::string& strIn);

std::string BuildSha1(const std::string& strIn);
std::string BuildSha1Up(const std::string& strIn);
std::string digestToUpCaseHex(const Poco::DigestEngine::Digest& bytes);

void ToTo(const char* in, char* out, unsigned long len);
std::streamsize ToTo(std::istream& istr, std::ostream& ostr, std::streamsize len);

void split(const std::string& s, const std::string& delim, std::vector< std::string >& ret);

std::string url_encode(const char* str);
std::string url_decode(const char* str);

int base64_decode(const unsigned char *in, unsigned int inlen,
    unsigned char *out, unsigned int *outlen);
int base64_encode(const unsigned char *in, unsigned int inlen,
    unsigned char *out, unsigned int *outlen);
