#include "AppConfig.h"

#define COLOR_ARGB(a, r, g, b)  ((COLORREF)(RGB(r,g,b)|(((DWORD)(BYTE)(a))<<24)))
void AppConfigure::LoadConfig()
{
	LoadViewConfig();
	LoadPageConfig();
	LoadReadConfig();
	LoadTextConfig();

	ModuleConfig conf;
	std::wstring window_value = conf.query_string(L"WINDOW", L"POS");

	if(!window_value.empty())
	{
		int l, r, t, b;
		_stscanf_s(window_value.c_str(),L"%d,%d,%d,%d",&l,&r,&t,&b);

		window_pos.left = l;
		window_pos.right = r;
		window_pos.top = t;
		window_pos.bottom = b;
	}
	std::wstring isFristRunApp = conf.query_string(L"APP", L"HAJIME");

	if(isFristRunApp == L"true")
	{
		conf.write_string(L"APP", L"HAJIME", L"false");
		isFirstRun = true;
	}
	else
	{
		isFirstRun = false;
	}
}
void AppConfigure::LoadViewConfig()
{
	ModuleConfig conf;

	m_VIEW_CONFIG.FONT = conf.query_string(L"VIEW", L"FONT");
	m_VIEW_CONFIG.FONTSIZE = conf.query_int(L"VIEW", L"FONTSIZE");

	int zoom_ = conf.query_int(L"VIEW", L"ZOOM");
	if(zoom_ >= 20 && zoom_ <= 400)
	{
		m_VIEW_CONFIG.VIEWZOOM = zoom_;
	}
	else
	{
		ModuleConfig conf;
		conf.write_int(L"VIEW", L"ZOOM", 100);
		m_VIEW_CONFIG.VIEWZOOM = 100;
	}
	int int_bold = conf.query_int(L"VIEW", L"BOLD");
	int int_underline = conf.query_int(L"VIEW", L"UNDERLINE");

	int int_VIEWIMAGE = conf.query_int(L"VIEW", L"VIEWIMAGE");
	int int_TEXTSHADOW = conf.query_int(L"VIEW", L"TEXTSHADOW");
	int int_HDRAG = conf.query_int(L"VIEW", L"HDRAG");
	m_VIEW_CONFIG.BOLD = int_bold > 0 ? true : false;
	m_VIEW_CONFIG.UNDERLINE = int_underline > 0 ? true : false;

	m_VIEW_CONFIG.VIEW_IMAGE = int_VIEWIMAGE > 0 ? true : false;
	m_VIEW_CONFIG.HDRAG = int_HDRAG > 0 ? true : false;

	m_VIEW_CONFIG.IMAGE = conf.query_string(L"VIEW", L"IMAGE");
	
	/*if(m_VIEW_CONFIG.IMAGE.empty())
	{
		std::wstring defImage = conf.getDefImage();

		m_VIEW_CONFIG.IMAGE = defImage;

		conf.write_string(L"VIEW", L"IMAGE", defImage);
	}*/
	m_VIEW_CONFIG.TEXT_SHADOW = int_TEXTSHADOW > 0 ? true : false;

	std::wstring text_color = conf.query_string(L"VIEW", L"TEXTCLR");
	std::wstring text_shadow_color = conf.query_string(L"VIEW", L"TEXTSHADOWCLR");
	std::wstring text_shadow_trans = conf.query_string(L"VIEW", L"SHADOWTRANS");
	std::wstring text_shadow_radius = conf.query_string(L"VIEW", L"SHADOWRADIUS");
	if(!text_color.empty())
	{
		int a, r, g, b;
		_stscanf_s(text_color.c_str(),L"%d,%d,%d,%d",&a,&r,&g,&b);

		m_VIEW_CONFIG.TEXTCLR = COLOR_ARGB(a, r, g, b);
	}
	if(!text_shadow_color.empty())
	{
		/*int a, r, g, b;
		_stscanf_s(text_shadow_color.c_str(),L"%d,%d,%d,%d",&a,&r,&g,&b);

		m_VIEW_CONFIG.TXTSHADOWCLR = COLOR_ARGB(a, r, g, b);*/
		m_VIEW_CONFIG.TXTSHADOWCLR = text_shadow_color;
		m_VIEW_CONFIG.TXTSHADOWRADIUS = text_shadow_radius;
		m_VIEW_CONFIG.TXTSHADOWTRANS = text_shadow_trans;
	}
}
void AppConfigure::LoadPageConfig()
{
	//bool show_page;
	//int image_type;
	//int transparent_value;
	//int Lvalue;
	//int Rvalue;
	//int Uvalue;
	//int Bvalue;
	//std::wstring image
	ModuleConfig conf;

	std::wstring str_show = conf.query_string(L"PAGE", L"SHOWPAGE");
	std::wstring str_color = conf.query_string(L"PAGE", L"COLOR");

	std::wstring PAGECOLOR = conf.query_string(L"PAGE", L"PAGECOLOR");

	if(!PAGECOLOR.empty())
	{
		int a, r, g, b;
		_stscanf_s(PAGECOLOR.c_str(),L"%d,%d,%d,%d",&a,&r,&g,&b);

		m_PAGE_CONFIG.ColorR = r;
		m_PAGE_CONFIG.ColorG = g;
		m_PAGE_CONFIG.ColorB = b;
	}
	int int_type = conf.query_int(L"PAGE", L"TYPE");
	int transparent = conf.query_int(L"PAGE", L"TRANS");

	m_PAGE_CONFIG.show_page = str_show == L"true"  ? true : false;
	m_PAGE_CONFIG.show_color = str_color == L"true"  ? true : false;
	m_PAGE_CONFIG.image_type = int_type;
	m_PAGE_CONFIG.transparent_value = transparent;
	m_PAGE_CONFIG.image = conf.query_string(L"PAGE", L"IMAGEPATH");

	if(m_PAGE_CONFIG.image.empty())
	{
		std::wstring defImage = conf.getDefImage();

		m_PAGE_CONFIG.image  = defImage;

		conf.write_string(L"PAGE", L"IMAGEPATH", defImage);
	}

	m_PAGE_CONFIG.save_string = conf.query_string(L"PAGE", L"NIPPLE");
	m_PAGE_CONFIG.Lvalue = conf.query_int(L"PAGE", L"L");
	m_PAGE_CONFIG.Rvalue = conf.query_int(L"PAGE", L"R");
	m_PAGE_CONFIG.Uvalue = conf.query_int(L"PAGE", L"U");
	m_PAGE_CONFIG.Bvalue = conf.query_int(L"PAGE", L"B");

}
void AppConfigure::LoadReadConfig()
{
	ModuleConfig conf;

	m_READ_CONFIG.last_image_path = conf.query_string(L"READ", L"IMAGE");
	m_READ_CONFIG.last_txt_path = conf.query_string(L"READ", L"TEXT");
}
void AppConfigure::LoadTextConfig()
{
	ModuleConfig conf;
	std::wstring STRCLR = conf.query_string(L"TEXT", L"CLRSEL");

	if(!STRCLR.empty())
	{
		int a, r, g, b;
		_stscanf_s(STRCLR.c_str(),L"%d,%d,%d,%d",&a,&r,&g,&b);
		m_TEXT_CONFIG.CLRSEL = COLOR_ARGB(a, r, g, b);
	}
	else
	{
		m_TEXT_CONFIG.CLRSEL = COLOR_ARGB(255, 255, 255, 255);
	}
	STRCLR = conf.query_string(L"TEXT", L"CLRSELTEXT");

	if(!STRCLR.empty())
	{
		int a, r, g, b;
		_stscanf_s(STRCLR.c_str(),L"%d,%d,%d,%d",&a,&r,&g,&b);
		m_TEXT_CONFIG.CLRSELTEXT = COLOR_ARGB(a, r, g, b);
	}
	else
	{
		m_TEXT_CONFIG.CLRSELTEXT = COLOR_ARGB(255, 12, 134, 241);
	}
	STRCLR = conf.query_string(L"TEXT", L"CLRCURSOR");

	if(!STRCLR.empty())
	{
		int a, r, g, b;
		_stscanf_s(STRCLR.c_str(),L"%d,%d,%d,%d",&a,&r,&g,&b);
		m_TEXT_CONFIG.CLRCURSOR = COLOR_ARGB(a, r, g, b);
	}
	
	std::wstring SELTEXTTRANS = conf.query_string(L"TEXT", L"SELTEXTTRANS");

	if(!SELTEXTTRANS.empty())
	{
	
		m_TEXT_CONFIG.TXTSELECTTRANS = SELTEXTTRANS;
	}
	else
	{
		m_TEXT_CONFIG.TXTSELECTTRANS = L"50";
	}
}
void AppConfigure::WriteTextOptionValue(int r1, int g1, int b1,
	int r2, int g2, int b2, std::string tansparent)
{
	ModuleConfig conf;
	
	AGuiString strTextSel;
	strTextSel.Format(L"255,%d,%d,%d", r1, g1, b1);

	m_TEXT_CONFIG.CLRSEL = COLOR_ARGB(255, r1, g1, b1);

	conf.write_string(L"TEXT", L"CLRSEL", strTextSel.GetData());

	AGuiString strTextColor;
	strTextColor.Format(L"255,%d,%d,%d", r2, g2, b2);

	m_TEXT_CONFIG.CLRSELTEXT = COLOR_ARGB(255, r2, g2, b2);

	conf.write_string(L"TEXT", L"CLRSELTEXT", strTextColor.GetData());
	conf.write_string(L"TEXT", L"SELTEXTTRANS", str_conv::str2wstr(tansparent));
}

VIEW_CONFIG& AppConfigure::GetViewConfig()
{
	return m_VIEW_CONFIG;
}
PAGE_CONFIG& AppConfigure::GetPageConfig()
{
	return m_PAGE_CONFIG;
}
READ_CONFIG& AppConfigure::GetReadConfig()
{
	return m_READ_CONFIG;
}
TEXT_CONFIG& AppConfigure::GetTextConfig()
{
	return m_TEXT_CONFIG;
}
void AppConfigure::WritePageValue(std::wstring image_file,std::wstring save_string, int Lvalue, int Rvalue, 
	int Uvalue, int Bvalue, int transvalue)
{

	ModuleConfig conf;
	conf.write_string(L"PAGE", L"IMAGEPATH", image_file);
	conf.write_string(L"PAGE", L"COLOR", L"false");
	conf.write_string(L"PAGE", L"SHOWPAGE", L"true");
	conf.write_string(L"PAGE", L"NIPPLE", save_string);
	conf.write_int(L"PAGE", L"L", Lvalue);
	conf.write_int(L"PAGE", L"R", Rvalue);
	conf.write_int(L"PAGE", L"U", Uvalue);
	conf.write_int(L"PAGE", L"B", Bvalue);
	conf.write_int(L"PAGE", L"TRANS", transvalue);
}
void AppConfigure::WritePageColor(std::wstring color, int Lvalue, int Rvalue, 
	int Uvalue, int Bvalue, int transvalue)
{

	ModuleConfig conf;
	conf.write_string(L"PAGE", L"PAGECOLOR", color);
	conf.write_string(L"PAGE", L"COLOR", L"true");
	conf.write_string(L"PAGE", L"SHOWPAGE", L"true");
	conf.write_int(L"PAGE", L"L", Lvalue);
	conf.write_int(L"PAGE", L"R", Rvalue);
	conf.write_int(L"PAGE", L"U", Uvalue);
	conf.write_int(L"PAGE", L"B", Bvalue);
	conf.write_int(L"PAGE", L"TRANS", transvalue);
}
void AppConfigure::WritePosValue(int l, int r, int t, int b)
{
	AGuiString window_pos;

	if(r > l && b > t && l > 0 && r > 0 && t > 0 && b > 0 )
	{	
		ModuleConfig conf;
		window_pos.Format(L"%d,%d,%d,%d", l, r, t, b);
		conf.write_string(L"WINDOW", L"POS", window_pos.GetData());
	}
}
void AppConfigure::WriteLastOpenDir(std::wstring image_file, tOpenDir type)
{
	size_t dir_pos = image_file.rfind(L"\\");

	if(dir_pos != std::wstring::npos)
	{
		std::wstring dir_name = image_file.substr(0, dir_pos + 1);

		if(type == tOpenDirTxt )
		{
			ModuleConfig conf;
			
			conf.write_string(L"READ", L"TEXT", dir_name);

			m_READ_CONFIG.last_txt_path = dir_name;
		}
		else if(type == tOpenDirImage)
		{
			ModuleConfig conf;

			conf.write_string(L"READ", L"IMAGE", dir_name);

			m_READ_CONFIG.last_image_path = dir_name;
		}
	}
}
RECT AppConfigure::GetPosValue()
{
	return window_pos;
}
bool AppConfigure::IsFirstRunApp()
{
	return isFirstRun;
}