#ifndef __string_conv_h__
#define __string_conv_h__

#include <time.h>
#include "stdafx.h"
#include <algorithm>
#include <deque>
#pragma once

using namespace std;
class str_conv{

public:
	struct char_encode
	{
		enum encode_t{utf8=CP_UTF8 , win_sys=CP_ACP};
	};


	static std::string time_t2str( time_t _time )
	{
		char _st[100] = { 0 };
		//memset(_st,0,100);
		int year=0,mon=0,day=0,hour=0,min=0,sec=0;
		tm m_tm; 
		errno_t err;
		//m_tm = localtime(&(time_t)_time);
		err = localtime_s(&m_tm,&(time_t)_time);
		if(err)
		{
			return "";
		}
		year = m_tm.tm_year+1900;
		mon = m_tm.tm_mon+1;
		day = m_tm.tm_mday;
		hour = m_tm.tm_hour;
		min = m_tm.tm_min;
		sec = m_tm.tm_sec; 
		sprintf_s(_st,100,"%04d-%02d-%02d %02d:%02d:%02d",year,mon,day,hour,min,sec);
		return std::string(_st);
	}
	static std::wstring str2wstr( const std::string from, char_encode::encode_t char_code = char_encode::win_sys )
	{
		if(from.length() == 0)
			return L"";
		int buflen=MultiByteToWideChar( char_code, 0, from.c_str(), -1, NULL, 0 ) +1 ;
		if(buflen == 0)
			return L"";
		wchar_t * buf =  new wchar_t[buflen];
		std::wstring retn = L"";
		if (buf)
		{
			memset(buf,0,  buflen*2 );
			MultiByteToWideChar( char_code, 0, from.c_str(), -1, buf, buflen );
			retn = buf;
			delete []buf;
		}	
		return retn;
	}
	static std::string wstr2str(const std::wstring from, char_encode::encode_t char_code  = char_encode::win_sys  )
	{
		if(from.length() == 0)
			return "";

		//得到转化后需要Buf的长度
		std::string retn = "";
		try
		{
			int buflen = WideCharToMultiByte( char_code, 0, from.c_str(), -1, NULL, 0, NULL, NULL ) + 1;
			if(buflen == 0)
				return "";
			char * buf = new char[buflen];
			if(buf != NULL)
			{
				memset(buf,0,  buflen );
				WideCharToMultiByte( char_code, 0, from.c_str(), -1, buf, buflen, NULL, NULL );
				retn = buf;
				delete []buf;
			}
		}
		catch (...)
		{

		}
		return retn;

	}
	static std::string int_2_string(int aa)
	{
		char a[20];
		return itoa(aa, a, 10);
	}
	static std::wstring  int_2_wstr(int idata)
	{
		char ss[MAX_PATH] = { 0 };
		sprintf_s(ss,MAX_PATH,"%d",idata);
		return str2wstr(std::string(ss));
	}
	static std::string int_2_timestr(__int64 _time )
	{
		char _st[100] = { 0 };
		//memset(_st,0,100);
		int year=0,mon=0,day=0,hour=0,min=0,sec=0; 
	
		//m_tm = localtime(&(time_t)_time);
		struct tm m_tm = *_localtime64((__time64_t *)&_time); 
		
		year = m_tm.tm_year+1900;
		mon = m_tm.tm_mon+1;
		day = m_tm.tm_mday;
		hour = m_tm.tm_hour;
		min = m_tm.tm_min;
		sec = m_tm.tm_sec; 
		sprintf_s(_st,100,"%04d-%02d-%02d",year,mon,day);
		return std::string(_st);
	}
	static time_t  str2time_t(const std::string str_time)
	{
		time_t    tResult        = 0;
		struct tm tmResult    = {0};
		int iYear    = 0;
		int iMonth    = 0;
		int iDay    = 0;
		int iHour    = 0;
		int iMinut    = 0;
		int iSecon    = 0;
		sscanf_s(str_time.c_str(),"%d-%d-%d %d:%d:%d",&iYear,&iMonth,&iDay,&iHour,&iMinut,&iSecon);
		tmResult.tm_hour    = iHour;
		tmResult.tm_isdst    = 0;
		tmResult.tm_min        = iMinut;
		tmResult.tm_mon        = iMonth - 1;
		tmResult.tm_sec        = iSecon;
		tmResult.tm_mday    = iDay;
		tmResult.tm_year    = iYear - 1900;
		tResult = mktime(&tmResult);
		return tResult;
	}
	static int wstring_2_int(wstring wa)
	{
		string aa = wstr2str(wa);
		int i_value = atoi(aa.c_str());
		return i_value;
	}
	static int string_2_int(string aa)
	{
		int i_value = atoi(aa.c_str());
		return i_value;
	}
	static std::string utf8_2_gbk(std::string szUtf8)
	{
		DWORD   UniCodeLen=MultiByteToWideChar(CP_UTF8, 0, szUtf8.c_str(), -1,   NULL, 0); 
		std::vector <wchar_t>   vWCH(UniCodeLen); 
		MultiByteToWideChar(CP_UTF8, 0, szUtf8.c_str(), -1, &vWCH[0], UniCodeLen); 

		DWORD   dwASCIILen=WideCharToMultiByte(CP_ACP, 0, &vWCH[0], UniCodeLen, NULL, NULL, NULL, NULL); 

		if(!dwASCIILen)
			return "";

		char*   pASCIIBuf = new char[dwASCIILen + 1];
		if(NULL == pASCIIBuf)
		{
			return "";
		}
		memset(pASCIIBuf, 0, dwASCIILen + 1);

		WideCharToMultiByte(CP_ACP, 0, &vWCH[0], UniCodeLen, pASCIIBuf, dwASCIILen, NULL, NULL); 
		std::string szRetAsiic = pASCIIBuf;
		delete[] pASCIIBuf;
		return szRetAsiic;
	}
	static std::string gbk_2_utf8(std::string strGBK)
	{
		if(strGBK.length() == 0)
			return "";

		string strOutUTF8 = ""; 
		wchar_t * str1;	
		try
		{
			int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
			if(n == 0)
				return "";
			str1 = new WCHAR[n]; 
			if(NULL == str1)
			{
				return "";
			}
			MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n); 
			n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL); 

			char * str2 = new char[n]; 
			if(str2 != NULL)
			{
				WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL); 
				strOutUTF8 = str2; 
				delete[]str2; 
				str2 = NULL;
			}

			if(str1 != NULL)
			{		
				delete[]str1; 
				str1 = NULL; 
			}
		}
		catch (...)
		{
			return "";	
		}	
		return strOutUTF8;
	}
	static std::wstring utf8_2_wstring(std::string szUtf8)  
	{  
		std::wstring retn = L"";
		if (szUtf8.empty())  
		{  
			//*unicode_number = 0;  
			//return NULL;  
			return L"";
		}  
		int dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, szUtf8.c_str(), -1, NULL, 0);  
		size_t num = dwUnicodeLen * sizeof(wchar_t);  
		wchar_t *pwText = (wchar_t*)malloc(num);  
		memset(pwText, 0, num);  
		MultiByteToWideChar(CP_UTF8, 0, szUtf8.c_str(), -1, pwText, dwUnicodeLen);  
		//*unicode_number = dwUnicodeLen - 1; 
		retn = pwText;
		return  retn;  
	}  
	static std::string string_To_UTF8(const std::string & str) 
	{ 
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0); 

		wchar_t * pwBuf = new wchar_t[nwLen + 1];
		ZeroMemory(pwBuf, nwLen * 2 + 2); 

		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen); 

		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL); 

		char * pBuf = new char[nLen + 1]; 
		ZeroMemory(pBuf, nLen + 1); 

		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL); 

		std::string retStr(pBuf); 

		delete []pwBuf; 
		delete []pBuf; 

		pwBuf = NULL; 
		pBuf  = NULL; 

		return retStr; 
	} 
	static time_t  now()
	{
		time_t now;
		time(&now);
		return now;
	}
};
class str_utility
{
public:
	static int replace_all( std::string& str, const std::string pattern, const std::string newpat )
	{
		int count = 0; 
		const size_t nsize = newpat.size(); 
		const size_t psize = pattern.size(); 

		for(size_t pos = str.find(pattern, 0);  
			pos != std::string::npos; 
			pos = str.find(pattern,pos + nsize)) 
		{ 
			str.replace(pos, psize, newpat); 
			count++; 
		} 

		return count; 
	}

	static int replace_all( std::wstring& str, const std::wstring pattern, const std::wstring newpat )
	{
		int count = 0; 
		const size_t nsize = newpat.size(); 
		const size_t psize = pattern.size(); 

		for(size_t pos = str.find(pattern, 0);  
			pos != std::string::npos; 
			pos = str.find(pattern,pos + nsize)) 
		{ 
			str.replace(pos, psize, newpat); 
			count++; 
		} 

		return count; 
	}
	static  int splite_string(std::string str, const std::string splite_str_sign, std::deque<std::string>& str_deque )
	{
		try{
			if(str.length() == 0)
				return 0;
			str_deque.clear();
			for (size_t pos = str.find(splite_str_sign, 0); 
				pos != std::string::npos;
				pos = str.find(splite_str_sign, 0))
			{
				str_deque.push_back(str.substr(0, pos));
				str = str.substr(pos + splite_str_sign.length());
			}

			str_deque.push_back(str.substr(0));
			return 1;
		}
		catch(...)
		{
			str_deque.clear();
			return 0;
		}
	}
	static int splite_string(std::wstring str, const std::wstring splite_str_sign, std::deque<std::wstring>& str_deque )
	{
		try{
			if(str.length() == 0)
				return 0;
			str_deque.clear();
			for (size_t pos = str.find(splite_str_sign, 0); 
				pos != std::string::npos;
				pos = str.find(splite_str_sign, 0))
			{
				str_deque.push_back(str.substr(0, pos));
				str = str.substr(pos + splite_str_sign.length());
			}

			str_deque.push_back(str.substr(0));
			return 1;
		}
		catch(...)
		{
			str_deque.clear();
			return 0;
		}

		return 1;
	}
	static std::string  to_lower(const std::string s)
	{
		std::string ret = s;
		std::transform(ret.begin(),ret.end(),ret.begin(), tolower);
		return ret;
	}
};
#endif