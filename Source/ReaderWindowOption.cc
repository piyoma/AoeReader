#include "ReaderWindow.h"
#include "AppConfig.h"


bool StringReplace(std::string& src, std::string target, std::string replacement)
{
	std::string::size_type startpos = 0;  
	while (startpos!= std::string::npos)  
	{  
		startpos = src.find(target);    
		if( startpos != std::string::npos ) 
		{  
			src.replace(startpos,1,replacement);  
		}  
	}  
	return true;
}

void  ReaderWindow::OnReaderOptionBar(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();

	if(element_id == "btnSelectBkgImage")
	{
		AAGuiArgs open_file_arg;

		open_file_arg.WriteString("filter1", "图像文件 (*.jpg;*.jpeg;*.png;*.bmp)");
		open_file_arg.WriteString("filter2", "*.jpg;*.jpg;*.jpeg;*.png;*.bmp");
		open_file_arg.WriteString("ext", "*.jpg");
		open_file_arg.WriteString("title", "打开文件");


		READ_CONFIG& readConfig_ = AppConfigure::Instance()->GetReadConfig();

		if(!readConfig_.last_image_path.empty())
		{
			open_file_arg.WriteStringW("path", readConfig_.last_image_path);
		}


		AAGUIEXEC(0, L"root", L"OpenFile", open_file_arg);

		std::string sel_path_result_ = open_file_arg.GetString("result");
		std::wstring sel_path_result_w = open_file_arg.GetStringW("resultw");

		if(!sel_path_result_w.empty())
		{

			AAGuiArgs bkgArg;
			bkgArg.WriteStringW("image_path", sel_path_result_w);
			AAGUIEXEC(L"contentView", L"ReaderViewScroll", L"SetImage", bkgArg);
		}
	}else if(element_id == "btnHidePageOption")
	{

		ModuleConfig conf;

		conf.write_string(L"USERINTER", L"SHOWTOOLBAR", L"false");

		AAGuiArgs isVisibleArg;

		AAGUIEXEC(L"contentView", L"BookMark", L"IsVisible", isVisibleArg);

		if(isVisibleArg.GetString("Value") != "true")
		{
			AAGUIEXEC(L"contentView", L"checkSidebar", L"Set", AAGuiArgs("Value", "true"));
		}
		AAGUIEXEC(L"contentView", L"ReaderOptionBar", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"root", L"contentView", L"Update", AAGuiArgs());
		AAGUIEXEC(L"contentView", L"PageShowHideTab", L"Set", AAGuiArgs("Value", "1"));
	}
	else if(element_id == "btnShowPageOption")
	{
		ModuleConfig conf;
		conf.write_string(L"USERINTER", L"SHOWTOOLBAR", L"true");

		AAGUIEXEC(L"contentView", L"checkSidebar", L"Set", AAGuiArgs("Value", "false"));

		AAGUIEXEC(L"contentView", L"ReaderOptionBar", L"SetVisible", AAGuiArgs("Value", "true"));

		AAGUIEXEC(L"contentView", L"PageShowHideTab", L"Set", AAGuiArgs("Value", "2"));
		AAGUIEXEC(L"root", L"contentView", L"Update", AAGuiArgs());
	}
	if(event_name_ == "ComboSelect")
	{
		if(element_id == "ComboFont")
		{
			std::string font_name = e.GetString("Value");

			VIEW_CONFIG&  viewConfig_ = AppConfigure::Instance()->GetViewConfig();

			viewConfig_.FONT = str_conv::str2wstr(font_name);

			LoadReaderViewFont();
			/*AAGuiArgs fontArg;
			fontArg.WriteString("Value", font_name);
			AAGUIEXEC(L"contentView", L"DefaultContet", L"SetFont", fontArg);
*/
			ModuleConfig conf;
			conf.write_string(L"VIEW", L"FONT", str_conv::str2wstr(font_name));
		}
		else if(element_id == "ComboFontSize")
		{
			std::string font_size = e.GetString("Value");

			VIEW_CONFIG&  viewConfig_ = AppConfigure::Instance()->GetViewConfig();

			viewConfig_.FONTSIZE = str_conv::string_2_int(font_size);

			LoadReaderViewFont();

			ModuleConfig conf;
			conf.write_int(L"VIEW", L"FONTSIZE", str_conv::string_2_int(font_size));
			/*AAGuiArgs fontArg;
			fontArg.WriteString("Value", font_name);
			AAGUIEXEC(L"contentView", L"DefaultContet", L"SetFont", fontArg);*/
		}
		else if(element_id == "ComboZoomPage")
		{
			std::string zoom_value = e.GetString("Value");

			
			StringReplace(zoom_value, "%", "");

			AAGuiArgs zoomArg;
			zoomArg.WriteString("Value", zoom_value);
			AAGUIEXEC(L"contentView", L"DefaultContet", L"SetZomm", 
				zoomArg);

			int int_zoom_value = str_conv::string_2_int(zoom_value);
			ModuleConfig conf;
			conf.write_int(L"VIEW", L"ZOOM", int_zoom_value);

			/*AAGuiArgs fontArg;
			fontArg.WriteString("Value", font_name);
			AAGUIEXEC(L"contentView", L"DefaultContet", L"SetFont", fontArg);*/
		}
	}

	if(event_name_ == "CheckBoxChanged")
	{
		if(element_id == "checkBOLD")
		{
			std::string bool_value = e.GetString("Value");

			VIEW_CONFIG&  viewConfig_ = AppConfigure::Instance()->GetViewConfig();

			viewConfig_.BOLD = bool_value == "true" ? true : false;

			LoadReaderViewFont();

			ModuleConfig conf;
			conf.write_int(L"VIEW", L"BOLD", bool_value == "true" ? 1 : 0);
		}
		else if(element_id == "checkUNDERLINE")
		{
			std::string bool_value = e.GetString("Value");

			VIEW_CONFIG&  viewConfig_ = AppConfigure::Instance()->GetViewConfig();

			viewConfig_.UNDERLINE = bool_value == "true" ? true : false;

			LoadReaderViewFont();

			ModuleConfig conf;
			conf.write_int(L"VIEW", L"UNDERLINE", bool_value == "true" ? 1 : 0);
		}
		else if(element_id == "checkIMAGE")
		{
			std::string bool_value = e.GetString("Value");


			ModuleConfig conf;
			conf.write_int(L"VIEW", L"VIEWIMAGE", bool_value == "true" ? 1 : 0);
		}
		else if(element_id == "checkPAGEVIEW")
		{
			std::string bool_value = e.GetString("Value");

			PAGE_CONFIG&  pageConfig_ = AppConfigure::Instance()->GetPageConfig();

			
			ModuleConfig conf;
			conf.write_string(L"PAGE", L"SHOWPAGE", bool_value == "true" ? L"true" : L"false");
		}
		else if(element_id == "checkTEXTSHADOW")
		{
			std::string bool_value = e.GetString("Value");

			ModuleConfig conf;
			conf.write_int(L"VIEW", L"TEXTSHADOW", bool_value == "true" ? 1 : 0);
		}
		else if(element_id == "checkHDRAG")
		{
			std::string bool_value = e.GetString("Value");

			ModuleConfig conf;
			conf.write_int(L"VIEW", L"HDRAG", bool_value == "true" ? 1 : 0);
		}
	}
}
void ReaderWindow::LoadReaderViewFont()
{
	VIEW_CONFIG&  viewConfig_ = AppConfigure::Instance()->GetViewConfig();
	AAGuiArgs FontArg;

	FontArg.WriteStringW("font_name", viewConfig_.FONT);
	FontArg.WriteString("font_size", str_conv::int_2_string(viewConfig_.FONTSIZE));
	FontArg.WriteString("bold", viewConfig_.BOLD ? "true" : "false");
	FontArg.WriteString("under_line", viewConfig_.UNDERLINE ? "true" : "false");

	AAGUIEXEC(L"contentView", L"DefaultContet", L"SetFont", 
		FontArg);


	

}
void ReaderWindow::LoadTextColor()
{
	TEXT_CONFIG&  textConfig_ = AppConfigure::Instance()->GetTextConfig();


	int RValue = GetRValue(textConfig_.CLRSEL);
	int GValue = GetGValue(textConfig_.CLRSEL);
	int BValue = GetBValue(textConfig_.CLRSEL);

	AAGuiArgs ColorArg;

	ColorArg.WriteString("SelRValue", str_conv::int_2_string(RValue));
	ColorArg.WriteString("SelGValue", str_conv::int_2_string(GValue));
	ColorArg.WriteString("SelBValue", str_conv::int_2_string(BValue));

	int RValue2 = GetRValue(textConfig_.CLRSELTEXT);
	int GValue2 = GetGValue(textConfig_.CLRSELTEXT);
	int BValue2 = GetBValue(textConfig_.CLRSELTEXT);


	ColorArg.WriteString("SelTextRValue", str_conv::int_2_string(RValue2));
	ColorArg.WriteString("SelTextGValue", str_conv::int_2_string(GValue2));
	ColorArg.WriteString("SelTextBValue", str_conv::int_2_string(BValue2));

	int RValue3 = GetRValue(textConfig_.CLRCURSOR);
	int GValue3 = GetGValue(textConfig_.CLRCURSOR);
	int BValue3 = GetBValue(textConfig_.CLRCURSOR);


	ColorArg.WriteString("CursorRValue", str_conv::int_2_string(RValue3));
	ColorArg.WriteString("CursorGValue", str_conv::int_2_string(GValue3));
	ColorArg.WriteString("CursorBValue", str_conv::int_2_string(BValue3));

	ColorArg.WriteStringW("TRANS", textConfig_.TXTSELECTTRANS);
	AAGUIEXEC(L"contentView", L"DefaultContet", L"SetTextArg", 
		ColorArg);
}
void ReaderWindow::ApplyTextColor()
{

	TEXT_CONFIG&  textConfig_ = AppConfigure::Instance()->GetTextConfig();

	AAGuiArgs getColorArg1;

	AAGUIEXEC(L"text-option", L"ColorSel", L"GetColor", 
			getColorArg1);

	std::string RSTR1 = getColorArg1.GetString("RValue");
	std::string GSTR1 = getColorArg1.GetString("GValue");
	std::string BSTR1 = getColorArg1.GetString("BValue");

	int RValue1 = str_conv::string_2_int(RSTR1);
	int GValue1 = str_conv::string_2_int(GSTR1);
	int BValue1 = str_conv::string_2_int(BSTR1);

	AAGuiArgs getColorArg2;

	AAGUIEXEC(L"text-option", L"ColorSelText", L"GetColor", 
		getColorArg2);

	std::string RSTR2 = getColorArg2.GetString("RValue");
	std::string GSTR2 = getColorArg2.GetString("GValue");
	std::string BSTR2 = getColorArg2.GetString("BValue");

	int RValue2 = str_conv::string_2_int(RSTR2);
	int GValue2 = str_conv::string_2_int(GSTR2);
	int BValue2 = str_conv::string_2_int(BSTR2);

	AAGuiArgs ColorArg;

	ColorArg.WriteString("SelRValue", RSTR1);
	ColorArg.WriteString("SelGValue", GSTR1);
	ColorArg.WriteString("SelBValue", BSTR1);



	ColorArg.WriteString("SelTextRValue", RSTR2);
	ColorArg.WriteString("SelTextGValue", GSTR2);
	ColorArg.WriteString("SelTextBValue", BSTR2);

	int RValue3 = GetRValue(textConfig_.CLRCURSOR);
	int GValue3 = GetGValue(textConfig_.CLRCURSOR);
	int BValue3 = GetBValue(textConfig_.CLRCURSOR);


	ColorArg.WriteString("CursorRValue", str_conv::int_2_string(RValue3));
	ColorArg.WriteString("CursorGValue", str_conv::int_2_string(GValue3));
	ColorArg.WriteString("CursorBValue", str_conv::int_2_string(BValue3));


	std::string trans_value = GUI_GET(L"text-option", L"SliderTransSelectText");

	AppConfigure::Instance()->WriteTextOptionValue(RValue1, GValue1, BValue1,
		RValue2, GValue2, BValue2, trans_value);

	ColorArg.WriteStringW("TRANS", str_conv::str2wstr(trans_value));
	AAGUIEXEC(L"contentView", L"DefaultContet", L"SetTextArg", 
		ColorArg);
}
void ReaderWindow::LoadTextColorOption()
{
	TEXT_CONFIG&  textConfig_ = AppConfigure::Instance()->GetTextConfig();


	int RValue = GetRValue(textConfig_.CLRSEL);
	int GValue = GetGValue(textConfig_.CLRSEL);
	int BValue = GetBValue(textConfig_.CLRSEL);

	AAGuiArgs ColorArg;

	ColorArg.WriteString("RValue", str_conv::int_2_string(RValue));
	ColorArg.WriteString("GValue", str_conv::int_2_string(GValue));
	ColorArg.WriteString("BValue", str_conv::int_2_string(BValue));

	AAGUIEXEC(L"text-option", L"ColorSel", L"SetBkColor", 
		ColorArg);


	int RValue2 = GetRValue(textConfig_.CLRSELTEXT);
	int GValue2 = GetGValue(textConfig_.CLRSELTEXT);
	int BValue2 = GetBValue(textConfig_.CLRSELTEXT);

	AAGuiArgs ColorArg2;
	ColorArg2.WriteString("RValue", str_conv::int_2_string(RValue2));
	ColorArg2.WriteString("GValue", str_conv::int_2_string(GValue2));
	ColorArg2.WriteString("BValue", str_conv::int_2_string(BValue2));

	AAGUIEXEC(L"text-option", L"ColorSelText", L"SetBkColor", 
		ColorArg2);


	int RValue3 = GetRValue(textConfig_.CLRCURSOR);
	int GValue3 = GetGValue(textConfig_.CLRCURSOR);
	int BValue3 = GetBValue(textConfig_.CLRCURSOR);

	/*AAGuiArgs ColorArg3;
	ColorArg3.WriteString("RValue", str_conv::int_2_string(RValue3));
	ColorArg3.WriteString("GValue", str_conv::int_2_string(GValue3));
	ColorArg3.WriteString("BValue", str_conv::int_2_string(BValue3));

	AAGUIEXEC(L"text-option", L"ColorSelCursor", L"SetBkColor", 
		ColorArg3);*/
	std::string pos_trans_value = str_conv::wstr2str(textConfig_.TXTSELECTTRANS);

	GUI_SET(L"text-option", L"SliderTransSelectText", pos_trans_value);
}
void ReaderWindow::LoadReaderPageParam()
{
	PAGE_CONFIG&  pageConfig_ = AppConfigure::Instance()->GetPageConfig();
	VIEW_CONFIG&  viewConfig_ = AppConfigure::Instance()->GetViewConfig();

	if(pageConfig_.show_color)
	{
		
		AAGuiArgs pageArgs;

		pageArgs.WriteStringW("image_path", pageConfig_.image);
		pageArgs.WriteStringW("save_string", pageConfig_.save_string);
		pageArgs.WriteString("show_page", pageConfig_.show_page ? "true" : "false");

		pageArgs.WriteString("R", str_conv::int_2_string(pageConfig_.ColorR));
		pageArgs.WriteString("G", str_conv::int_2_string(pageConfig_.ColorG));
		pageArgs.WriteString("B", str_conv::int_2_string(pageConfig_.ColorB));

		pageArgs.WriteString("TRANS", str_conv::int_2_string(pageConfig_.transparent_value));
		pageArgs.WriteString("Lvalue",str_conv::int_2_string(pageConfig_.Lvalue));
		pageArgs.WriteString("Rvalue", str_conv::int_2_string(pageConfig_.Rvalue));
		pageArgs.WriteString("Uvalue", str_conv::int_2_string(pageConfig_.Uvalue));
		pageArgs.WriteString("Bvalue", str_conv::int_2_string(pageConfig_.Bvalue));

		pageArgs.WriteString("show_textshadow", viewConfig_.TEXT_SHADOW? "true" : "false");
		pageArgs.WriteString("horizon_drag", viewConfig_.HDRAG? "true" : "false");
		pageArgs.WriteStringW("textshadow_color", viewConfig_.TXTSHADOWCLR);
		pageArgs.WriteStringW("textshadow_trans", viewConfig_.TXTSHADOWTRANS);
		pageArgs.WriteStringW("textshadow_radius", viewConfig_.TXTSHADOWRADIUS);

		AAGUIEXEC(L"contentView", L"DefaultContet", L"SetPageColor", 
			pageArgs);

		
	}
	else
	{
		AAGuiArgs pageArgs;
		pageArgs.WriteStringW("image_path", pageConfig_.image);
		pageArgs.WriteStringW("save_string", pageConfig_.save_string);
		pageArgs.WriteString("show_page", pageConfig_.show_page ? "true" : "false");
		pageArgs.WriteString("show_textshadow", viewConfig_.TEXT_SHADOW? "true" : "false");
		pageArgs.WriteString("horizon_drag", viewConfig_.HDRAG? "true" : "false");
		pageArgs.WriteStringW("textshadow_color", viewConfig_.TXTSHADOWCLR);
		pageArgs.WriteStringW("textshadow_trans", viewConfig_.TXTSHADOWTRANS);
		pageArgs.WriteStringW("textshadow_radius", viewConfig_.TXTSHADOWRADIUS);
		pageArgs.WriteString("TRANS", str_conv::int_2_string(pageConfig_.transparent_value));
		pageArgs.WriteString("L",str_conv::int_2_string(pageConfig_.Lvalue));
		pageArgs.WriteString("R", str_conv::int_2_string(pageConfig_.Rvalue));
		pageArgs.WriteString("U", str_conv::int_2_string(pageConfig_.Uvalue));
		pageArgs.WriteString("B", str_conv::int_2_string(pageConfig_.Bvalue));
		AAGUIEXEC(L"contentView", L"DefaultContet", L"SetPageImage", 
			pageArgs);
	}
	/*if(pageConfig_.show_page)
	{
		AAGUIEXEC(L"DefaultContet", L"checkPAGEVIEW", L"Set", 
			AAGuiArgs("Value", "true"));
	}
	else
	{
		AAGUIEXEC(L"DefaultContet", L"checkPAGEVIEW", L"Set", 
			AAGuiArgs("Value", "false"));
	}*/
}
void  ReaderWindow::OnOptionPopupEvent(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();

	if(element_id == "SaveTextColor")
	{
		/*VIEW_CONFIG&  viewConfig_ = AppConfigure::Instance()->GetViewConfig();

		viewConfig_.UNDERLINE = bool_value == "true" ? true : false;

		LoadReaderViewFont();

		ModuleConfig conf;
		conf.write_int(L"VIEW", L"UNDERLINE", bool_value == "true" ? 1 : 0);*/

		AAGuiArgs ColorArg;

		AAGUIEXEC(L"text-color-option", L"ColorDisplay", L"GetColor", 
			ColorArg);

		VIEW_CONFIG&  viewConfig_ = AppConfigure::Instance()->GetViewConfig();

		int RValue = str_conv::string_2_int(ColorArg.GetString("RValue"));
		int GValue = str_conv::string_2_int(ColorArg.GetString("GValue"));
		int BValue = str_conv::string_2_int(ColorArg.GetString("BValue"));
#ifndef MAKE_ARGB
#define MAKE_ARGB(a, r, g, b)  ((COLORREF)(RGB(r,g,b)|(((DWORD)(BYTE)(a))<<24)))
#endif

		DWORD newColor =  MAKE_ARGB(255, RValue, GValue, BValue);

		if(viewConfig_.TEXTCLR != newColor)
		{
			viewConfig_.TEXTCLR = newColor;


			wchar_t color_buf[MAX_PATH];

			wsprintf(color_buf, L"255,%d,%d,%d\0", RValue, GValue, BValue);

			ModuleConfig conf;
			conf.write_string(L"VIEW", L"TEXTCLR", color_buf);

			AAGUIEXEC(L"contentView", L"DefaultContet", L"SetTextColor", 
				ColorArg);


		}
		AAGUIEXEC(0, L"root", L"popup_layer",  AAGuiArgs("command", "anime_destory"));
	}
	else if(element_id == "SaveShadowColor")
	{

		AAGuiArgs ColorArg;

		/*AAGUIEXEC(L"text-shadow-option", L"ColorDisplay", L"GetColor", 
		ColorArg);*/

		AAGUIEXEC(L"contentView", L"DefaultContet", L"GetCurrentShadow", 
			ColorArg);

		VIEW_CONFIG&  viewConfig_ = AppConfigure::Instance()->GetViewConfig();

		int RValue = str_conv::string_2_int(ColorArg.GetString("RValue"));
		int GValue = str_conv::string_2_int(ColorArg.GetString("GValue"));
		int BValue = str_conv::string_2_int(ColorArg.GetString("BValue"));
		int AValue = str_conv::string_2_int(ColorArg.GetString("AValue"));
		int ShadowRadius = str_conv::string_2_int(ColorArg.GetString("ShadowRadius"));

#ifndef MAKE_ARGB
#define MAKE_ARGB(a, r, g, b)  ((COLORREF)(RGB(r,g,b)|(((DWORD)(BYTE)(a))<<24)))
#endif

		DWORD newColor =  MAKE_ARGB(255, RValue, GValue, BValue);



			wchar_t color_buf[MAX_PATH];

			wsprintf(color_buf, L"255,%d,%d,%d\0", RValue, GValue, BValue);


			std::wstring transAValue = str_conv::int_2_wstr(AValue);
			std::wstring strShadowRadius = str_conv::int_2_wstr(ShadowRadius);
			ModuleConfig conf;
			conf.write_string(L"VIEW", L"TEXTSHADOWCLR", color_buf);
			conf.write_string(L"VIEW", L"SHADOWTRANS", transAValue);
			conf.write_string(L"VIEW", L"SHADOWRADIUS", strShadowRadius);

		AAGUIEXEC(0, L"root", L"popup_layer",  AAGuiArgs("command", "anime_destory"));
	}
}
void  ReaderWindow::OnPageOptionEvent(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();

	if(element_id == "btnSelImage1")
	{
		AAGuiArgs open_file_arg;

		open_file_arg.WriteString("filter1", "图像文件 (*.jpg;*.jpeg;*.png;*.bmp)");
		open_file_arg.WriteString("filter2", "*.jpg;*.jpg;*.jpeg;*.png;*.bmp");
		open_file_arg.WriteString("ext", "*.jpg");
		open_file_arg.WriteString("title", "打开文件");

		READ_CONFIG& readConfig_ = AppConfigure::Instance()->GetReadConfig();

		if(!readConfig_.last_image_path.empty())
		{
			open_file_arg.WriteStringW("path", readConfig_.last_image_path);
		}

		AAGUIEXEC(0, L"root", L"OpenFile", open_file_arg);

		std::string sel_path_result_ = open_file_arg.GetString("result");
		std::wstring sel_path_result_w = open_file_arg.GetStringW("resultw");
		if(!sel_path_result_w.empty())
		{
			AAGuiArgs previewArg;

			last_image_path = sel_path_result_w;
			previewArg.WriteStringW("Value", sel_path_result_w);
			AAGUIEXEC(L"page-option", L"CustomImageDisplay", L"Set", 
				previewArg);

		//	GUI_SET(L"page-option", L"page-option-tab", "2");

			AppConfigure::Instance()->WriteLastOpenDir(sel_path_result_w, tOpenDirImage);
		}
	}
	else if(element_id == "btnApplyImage1")
	{

		std::string sel_string_value_ = GUI_GET(L"page-option", L"InnerImageTab");
		std::string transparent_value_ = GUI_GET(L"page-option", L"SliderTrans");
		std::string L_value_ = GUI_GET(L"page-option", L"SliderL");
		std::string R_value_ = GUI_GET(L"page-option", L"SliderR");
		std::string T_value_ = GUI_GET(L"page-option", L"SliderU");
		std::string N_value_ = GUI_GET(L"page-option", L"SliderB");

		std::wstring sel_wstring_value_ = str_conv::str2wstr(sel_string_value_);

		std::wstring value_string_ = L"pageres\\page\\";

		value_string_+= sel_wstring_value_;

		value_string_ += L".png";

		if(!sel_string_value_.empty())
		{
			AppConfigure::Instance()->WritePageValue(value_string_,L"", 
				str_conv::string_2_int(L_value_), str_conv::string_2_int(R_value_), 
				str_conv::string_2_int(T_value_), str_conv::string_2_int(N_value_), 
				str_conv::string_2_int(transparent_value_));

			AAGuiArgs pageArgs;
			pageArgs.WriteStringW("image_path", value_string_);
			pageArgs.WriteString("TRANS", transparent_value_);
			pageArgs.WriteString("L", L_value_);
			pageArgs.WriteString("R", R_value_);
			pageArgs.WriteString("U", T_value_);
			pageArgs.WriteString("B", N_value_);
			AAGUIEXEC(L"contentView", L"DefaultContet", L"SetPageImage", 
				pageArgs);


			AAGUIEXEC(0, L"root", L"popup_layer",  AAGuiArgs("command", "anime_destory"));
		}
	}
	else if(element_id == "btnApplyImage2")
	{

		AAGuiArgs oneArg;

		oneArg.WriteStringW("filew", last_image_path);

		dataExecNULL(0, "obj_document", "generate_image_path",
			oneArg);

		std::wstring md5string_ = oneArg.GetStringW("md5");
		std::wstring srcString_ = oneArg.GetStringW("path");
		AAGuiArgs SaveArg;

		SaveArg.WriteStringW("Value", md5string_);
		SaveArg.WriteStringW("path", srcString_); //srcimage

		/*AAGUIEXEC(L"page-option", L"CustomImageDisplay", L"Save", 
			SaveArg);*/
		/*AAGuiArgs saveString;
		AAGUIEXEC(L"page-option", L"CustomImageDisplay", L"GetSaveString", 
			saveString);*/
		AAGUIEXEC(L"contentView", L"DefaultContet", L"SaveiMage", 
				SaveArg);

		if(!SaveArg.GetString("Error").empty())
		{

			AAGUIEXEC(L"page-option", L"page_option_tip", L"Set", 
				AAGuiArgs("Value", "2"));
			return;
		}

		std::wstring SaveString = SaveArg.GetStringW("SaveString");

		std::string transparent_value_ = GUI_GET(L"page-option", L"SliderTrans2");
		std::string L_value_ = GUI_GET(L"page-option", L"SliderL2");
	//	std::string R_value_ = GUI_GET(L"page-option", L"SliderR2");
		std::string T_value_ = GUI_GET(L"page-option", L"SliderU2");
		std::string N_value_ = GUI_GET(L"page-option", L"SliderB2");

		if(!md5string_.empty())
		{
			AppConfigure::Instance()->WritePageValue(srcString_/*md5string_*/, SaveString,
				str_conv::string_2_int(L_value_), str_conv::string_2_int(L_value_), 
				str_conv::string_2_int(T_value_), str_conv::string_2_int(N_value_), 
				str_conv::string_2_int(transparent_value_));

			AAGuiArgs pageArgs;
			pageArgs.WriteStringW("image_path", srcString_/*md5string_*/);
			pageArgs.WriteString("TRANS", transparent_value_);
			pageArgs.WriteString("L", L_value_);
			pageArgs.WriteString("R", L_value_);
			pageArgs.WriteString("U", T_value_);
			pageArgs.WriteString("B", N_value_);
			pageArgs.WriteString("show_page", "true");
			pageArgs.WriteString("restore_before", "true");
			pageArgs.WriteStringW("save_string", SaveString);
			AAGUIEXEC(L"contentView", L"DefaultContet", L"SetPageImage", 
				pageArgs);


			AAGUIEXEC(0, L"root", L"popup_layer",  AAGuiArgs("command", "anime_destory"));
		}

	}
	else if(element_id == "ReviseColor")
	{
		AAGuiArgs ColorArg;

		AAGUIEXEC(L"image-color-revise", L"ColorDisplay", L"GetColor", 
			ColorArg);


		int RValue = str_conv::string_2_int(ColorArg.GetString("RValue"));
		int GValue = str_conv::string_2_int(ColorArg.GetString("GValue"));
		int BValue = str_conv::string_2_int(ColorArg.GetString("BValue"));

		
	
		AAGUIEXEC(L"frameCostomImage", L"CustomImageDisplay", L"SetColor", 
			ColorArg);

		AAGUIEXEC(L"root", L"page-option", L"popup_layer",  AAGuiArgs("command", "anime_destory"));
		
	}
	else if(element_id == "btnApplyColor")
	{
		AAGuiArgs ColorArg;

		AAGUIEXEC(L"frameCostomColor", L"ColorDisplay", L"GetColor", 
			ColorArg);

		std::string RSTR = ColorArg.GetString("RValue");
		std::string GSTR = ColorArg.GetString("GValue");
		std::string BSTR = ColorArg.GetString("BValue");

		int RValue = str_conv::string_2_int(RSTR);
		int GValue = str_conv::string_2_int(GSTR);
		int BValue = str_conv::string_2_int(BSTR);

		std::string transparent_value_ = GUI_GET(L"page-option", L"SliderTrans3");
		std::string L_value_ = GUI_GET(L"page-option", L"SliderL3");
		std::string R_value_ = L_value_;/*GUI_GET(L"page-option", L"SliderR3");*/
		std::string T_value_ = GUI_GET(L"page-option", L"SliderU3");
		std::string N_value_ = GUI_GET(L"page-option", L"SliderB3");

		AGuiString colorStr;
		colorStr.Format(L"255,%d,%d,%d", RValue, GValue, BValue);


		AppConfigure::Instance()->WritePageColor(colorStr.GetData(), 
			str_conv::string_2_int(L_value_), str_conv::string_2_int(R_value_), 
			str_conv::string_2_int(T_value_), str_conv::string_2_int(N_value_), 
			str_conv::string_2_int(transparent_value_));


		AAGuiArgs pageArgs;
		pageArgs.WriteString("R", RSTR);
		pageArgs.WriteString("G", GSTR);
		pageArgs.WriteString("B", BSTR);

		pageArgs.WriteString("TRANS", transparent_value_);
		pageArgs.WriteString("Lvalue", L_value_);
		pageArgs.WriteString("Rvalue", R_value_);
		pageArgs.WriteString("Uvalue", T_value_);
		pageArgs.WriteString("Bvalue", N_value_);
		pageArgs.WriteString("restore_before", "true");
		AAGUIEXEC(L"contentView", L"DefaultContet", L"SetPageColor", 
			pageArgs);


		AAGUIEXEC(0, L"root", L"popup_layer",  AAGuiArgs("command", "anime_destory"));
	}
}
void  ReaderWindow::OnTextOptionEvent(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();

	if(element_id == "ReviseColorSelect")
	{
		AAGuiArgs ColorArg;

		AAGUIEXEC(L"color-revise-select", L"ColorDisplay", L"GetColor", 
			ColorArg);

	/*	std::string RSTR = ColorArg.GetString("RValue");
		std::string GSTR = ColorArg.GetString("GValue");
		std::string BSTR = ColorArg.GetString("BValue");

		int RValue = str_conv::string_2_int(RSTR);
		int GValue = str_conv::string_2_int(GSTR);
		int BValue = str_conv::string_2_int(BSTR);*/

		AAGUIEXEC(L"text-option", L"ColorSel", L"SetBkColor", 
			ColorArg);

		AAGUIEXEC(0, L"text-option", L"popup_layer",  AAGuiArgs("command", "anime_destory"));
	}
	else if(element_id == "ReviseColorText")
	{
		AAGuiArgs ColorArg;

		AAGUIEXEC(L"color-revise-text", L"ColorDisplay", L"GetColor", 
			ColorArg);

		/*std::string RSTR = ColorArg.GetString("RValue");
		std::string GSTR = ColorArg.GetString("GValue");
		std::string BSTR = ColorArg.GetString("BValue");

		int RValue = str_conv::string_2_int(RSTR);
		int GValue = str_conv::string_2_int(GSTR);
		int BValue = str_conv::string_2_int(BSTR);*/

		AAGUIEXEC(L"text-option", L"ColorSelText", L"SetBkColor", 
			ColorArg);

		AAGUIEXEC(0, L"text-option", L"popup_layer",  AAGuiArgs("command", "anime_destory"));
	}
	else if(element_id == "btnDefaultColor")
	{
		AAGuiArgs ColorArg;

		ColorArg.WriteString("RValue", "12");
		ColorArg.WriteString("GValue", "134");
		ColorArg.WriteString("BValue", "241");

		AAGUIEXEC(L"text-option", L"ColorSel", L"SetBkColor", 
			ColorArg);

		AAGuiArgs ColorArg2;
		ColorArg2.WriteString("RValue", "255");
		ColorArg2.WriteString("GValue", "255");
		ColorArg2.WriteString("BValue", "255");

		AAGUIEXEC(L"text-option", L"ColorSelText", L"SetBkColor", 
			ColorArg2);

	}
	else if(element_id == "btnApplyColorTextOption")
	{
		ApplyTextColor();

		AAGUIEXEC(0, L"root", L"popup_layer",  AAGuiArgs("command", "anime_destory"));
	}
}