#ifndef __debug_ini_file_h__
#define __debug_ini_file_h__

#include "stdafx.h"
#include "string_conv.h"
class  ModuleConfig
{
public:
    ModuleConfig(std::wstring file_name = L"module.ini")
    {
		wchar_t wszAppPath[MAX_PATH] = {0};
		::GetModuleFileName(NULL, wszAppPath, MAX_PATH);
		std::wstring wstrAppPath = wszAppPath;
		std::wstring strTemp = wstrAppPath.substr(0, wstrAppPath.find_last_of(L'\\') + 1);

		strTemp += L"\\object_config\\";
		strTemp += file_name;
		m_inifile = strTemp;
    }
	~ModuleConfig()
	{

	}
public:
    void create(std::wstring inifile)
    {
		wchar_t wszAppPath[MAX_PATH] = {0};
		::GetModuleFileName(NULL, wszAppPath, MAX_PATH);
		std::wstring wstrAppPath = wszAppPath;
		std::wstring strTemp = wstrAppPath.substr(0, wstrAppPath.find_last_of(L'\\') + 1);

		strTemp += L"\\object_config\\";
		strTemp += inifile;
		m_inifile = strTemp;
    }
	std::wstring getDefImage()
	{
		wchar_t wszAppPath[MAX_PATH] = {0};
		::GetModuleFileName(NULL, wszAppPath, MAX_PATH);
		std::wstring wstrAppPath = wszAppPath;
		std::wstring strTemp = wstrAppPath.substr(0, wstrAppPath.find_last_of(L'\\') + 1);
		strTemp += L"\\res\\srcimage.png";
		return strTemp;
	}
    std::wstring query_string(const std::wstring &app, const std::wstring &key, const std::wstring &def = L"")
    {
        wchar_t buf[256] = {0};
        GetPrivateProfileString(app.c_str(), key.c_str(), def.c_str(), buf, 256, m_inifile.c_str());
        return buf;
    }
    void write_string(const std::wstring &app, const std::wstring &key, const std::wstring &value)
    {
        WritePrivateProfileString(app.c_str(), key.c_str(), value.c_str(), m_inifile.c_str());
    }
    int query_int(const std::wstring &app, const std::wstring &key, int def = 0)
    {
        return (int)GetPrivateProfileInt(app.c_str(), key.c_str(), def, m_inifile.c_str());
    }
    void write_int(const std::wstring &app, const std::wstring &key, int value)
    {
        std::wstring v = str_conv::int_2_wstr(value);
        WritePrivateProfileString(app.c_str(), key.c_str(), v.c_str(), m_inifile.c_str());
    }


private:
    std::wstring m_inifile;
};

#endif 
