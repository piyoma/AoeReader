#ifndef __app_config_h__
#define __app_config_h__

#include "ModuleConfig.h"
#include "stdafx.h"

struct VIEW_CONFIG{
	std::wstring	FONT;
	int		 FONTSIZE;
	int VIEWZOOM;
	bool	BOLD;
	bool	UNDERLINE;
	bool VIEW_IMAGE;
	bool TEXT_SHADOW;
	bool HDRAG;
	std::wstring	IMAGE;
	int	FILLTYPE;
	DWORD	TEXTCLR;
	DWORD	BKCLR;
	DWORD	PAGECLR;
	std::wstring	TXTSHADOWCLR;
	std::wstring	TXTSHADOWTRANS;
	std::wstring	TXTSHADOWRADIUS;
};
struct TEXT_CONFIG{
	DWORD	CLRSEL;
	DWORD	CLRSELTEXT;
	DWORD	CLRCURSOR;
	std::wstring	TXTSELECTTRANS;
};

struct PAGE_CONFIG{
	bool show_page;
	bool show_color;
	int image_type;
	int transparent_value;
	int Lvalue;
	int Rvalue;
	int Uvalue;
	int Bvalue;
	int ColorR;
	int ColorG;
	int ColorB;
	std::wstring image;
	std::wstring save_string;
};
struct READ_CONFIG{
	std::wstring last_image_path;
	std::wstring last_txt_path;
};
enum tOpenDir{
	tOpenDirTxt = 0,
	tOpenDirImage
};
class AppConfigure
{
public:

	AppConfigure():isFirstRun(false){};
	~AppConfigure(){};

	static AppConfigure* Instance()
	{
		static AppConfigure* instance_ = NULL;
		if(instance_ == NULL)
		{
			instance_ = new AppConfigure();
		}
		return instance_;
	}
	
	void LoadConfig();
	void LoadViewConfig();
	void LoadPageConfig();
	void LoadReadConfig();
	void LoadTextConfig();

	VIEW_CONFIG& GetViewConfig();
	PAGE_CONFIG& GetPageConfig();
	READ_CONFIG& GetReadConfig();
	TEXT_CONFIG& GetTextConfig();

	void WritePageValue(std::wstring image_file, std::wstring save_string, int Lvalue, int Rvalue,
		int Uvalue, int Bvalue, int transvalue);
	void WritePageColor(std::wstring color_str, int Lvalue, int Rvalue,
		int Uvalue, int Bvalue, int transvalue);
	void WritePosValue(int l, int r, int t, int b);
	void WriteLastOpenDir(std::wstring image_file, tOpenDir type);
	RECT GetPosValue();
	void WriteTextOptionValue(int r1, int g1, int b1,
		int r2, int g2, int b2, std::string tansparent);

	bool IsFirstRunApp();
private:
	VIEW_CONFIG m_VIEW_CONFIG;
	PAGE_CONFIG m_PAGE_CONFIG;
	READ_CONFIG m_READ_CONFIG;
	TEXT_CONFIG m_TEXT_CONFIG;
	RECT window_pos;
	bool isFirstRun;

};

#endif 
