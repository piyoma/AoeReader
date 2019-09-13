#include "ReaderWindow.h"
#include "AppConfig.h"
#include "Resource.h"
#include <ShellAPI.h>

ReaderWindow::ReaderWindow()
	:widget_window_(NULL),
	has_open_file(false),
	is_user_login(false),
	is_big_avatar(false),
	is_set_avatar(false),
	oneAoeUser(NULL),
	init_open_file(L""),
	login_session_count(0),
	history_total_count(0)
{

}
ReaderWindow::~ReaderWindow()
{

}
void ReaderWindow::OnNativeWidgetCreated(HWND windowm, AAWidgetWindow* widget_window)
{
	if(widget_window)
	{
		m_hIcon = ::LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_AOEREADER));
		widget_window_ = widget_window;

		::SendMessage(widget_window_->GetNativeWindow(), WM_SETICON, TRUE, (LPARAM)m_hIcon);
		::SendMessage(widget_window_->GetNativeWindow(), WM_SETICON, FALSE, (LPARAM)m_hIcon);
		ModuleConfig conf;

		std::wstring is_auto_login	= conf.query_string(L"USER", L"AUTOLOGIN");

		std::wstring is_view_big	= conf.query_string(L"USER", L"VIEWBIG");
		if(is_auto_login == L"true")
		{

			std::wstring ClientInfo = conf.query_string(L"USER", L"CLIENT");

			ClientInfoMsg = ClientInfo;

			std::wstring REMPASS = conf.query_string(L"USER", L"REMPASS");
			std::wstring AUTOLOGIN = conf.query_string(L"USER", L"AUTOLOGIN");

			GUI_SET(L"user-login", L"chkRemPass", str_conv::wstr2str(REMPASS));
			GUI_SET(L"user-login", L"chkAutoLogin", str_conv::wstr2str(AUTOLOGIN));

			AAGuiArgs loginArg;

			loginArg.WriteStringW("ClientInfo", ClientInfo);
			dataExecNULL(0, "obj_document", "get_client_info",
				loginArg);
			std::string succeed = loginArg.GetString("succeed");

			if(succeed == "true")
			{
				AAGuiArgs* UserValue = loginArg.GetValue("AoeUser");

				if(UserValue)
				{
					std::string user_mail = UserValue->GetString("mail");
					std::string user_password = UserValue->GetString("pass");

					AAGuiArgs loginArg;

					loginArg.WriteStringW("mail", str_conv::str2wstr(user_mail));
					loginArg.WriteStringW("pass", str_conv::str2wstr(user_password));
					login_session_count++;
					loginArg.WriteString("login_session_count", str_conv::int_2_string(login_session_count));
					AAGUIEXEC(L"SystemBar", L"btnAutoLogin", L"SetVisible", AAGuiArgs("Value", "true"));
					AAGUIEXEC(L"SystemBar", L"btnLogin", L"SetVisible", AAGuiArgs("Value", "false"));

					dataExec(0, "obj_document", "user_login",
						loginArg, ReaderWindow::OnWebData);

				}
			}
			
		}

		if(is_view_big == L"true")
		{
			is_big_avatar = true;
		}
		else
		{
			is_big_avatar = false;
		}
		AAGuiArgs guiArg;
		guiArg.WriteStringW("Value", RrightString);
		AAGUIEXEC(L"ReaderPreview", L"RightComment", L"Set",  guiArg);

		GUI_BIND_AREA(L"WorkTab", L"ReaderPreview", ReaderWindow::OnPreviewFrameEvent);
		GUI_BIND_AREA(L"rootlayout", L"MainToolBar", ReaderWindow::OnToolbarEvent);

		GUI_BIND_AREA(L"WorkTab", L"BookMark", ReaderWindow::OnBookMarkEvent);
		GUI_BIND_AREA(L"WorkTab", L"tabshowBookMarkArea", ReaderWindow::OnBookMarkEvent);

		GUI_BIND_AREA(L"WorkTab", L"ReaderOptionBar", ReaderWindow::OnReaderOptionBar);
		GUI_BIND_AREA(L"WorkTab", L"TitleFuncBar", ReaderWindow::OnTitleFuncBarEvent);
		GUI_BIND_AREA(L"WorkTab", L"FindTextBar", ReaderWindow::OnTitleFuncBarEvent);
		GUI_BIND_AREA(L"root", L"SystemBar", ReaderWindow::OnUserEvent);

		GUI_SET(L"WorkTab", L"PageShowHideTab", "2");
		GUI_BIND_AREA(L"WorkTab", L"PageShowHideTab", ReaderWindow::OnReaderOptionBar);

		GUI_BIND_WIDGET_EVENT(0, L"root", L"NativeClose", ReaderWindow::OnNativeClose);
		AAGuiArgs recentArg;
		dataExecNULL(0, "obj_document", "get_recent_read",
			recentArg);
	
		if(recentArg.HasListValue())
		{
			AAGUIEXEC(L"ReaderPreview", L"ListRecentRead", L"AddItem", recentArg);
		}
		else
		{
			AAGUIEXEC(L"ReaderPreview", L"btnAllDeleteRecent", L"SetVisible", AAGuiArgs("Value", "false"));
		}
		LoadReaderViewFont();
		LoadReaderPageParam();
		LoadTextColor();
		CheckSideProcedure();
		VIEW_CONFIG&  viewConfig_ = AppConfigure::Instance()->GetViewConfig();

		int RValue = GetRValue(viewConfig_.TEXTCLR);
		int GValue = GetGValue(viewConfig_.TEXTCLR);
		int BValue = GetBValue(viewConfig_.TEXTCLR);

		AAGuiArgs ColorArg;

		ColorArg.WriteString("RValue", str_conv::int_2_string(RValue));
		ColorArg.WriteString("GValue", str_conv::int_2_string(GValue));
		ColorArg.WriteString("BValue", str_conv::int_2_string(BValue));



		AAGUIEXEC(L"contentView", L"DefaultContet", L"SetTextColor", 
				ColorArg);

		

		AAGUIEXEC(L"contentView", L"ComboFont", L"Set",
			AAGuiArgs("Value", str_conv::wstr2str(viewConfig_.FONT)));

		AAGUIEXEC(L"contentView", L"ComboFontSize", L"Set", 
			AAGuiArgs("Value", str_conv::int_2_string(viewConfig_.FONTSIZE)));

		AAGuiArgs zoomArg;
		std::string zoom_string = str_conv::int_2_string(viewConfig_.VIEWZOOM);
		zoomArg.WriteString("Value", zoom_string);
		AAGUIEXEC(L"contentView", L"DefaultContet", L"SetZomm", 
			zoomArg);
		zoom_string += "%";
		GUI_SET(L"contentView", L"ComboZoomPage", zoom_string);

		AAGUIEXEC(L"contentView", L"checkBOLD", L"Set", 
			AAGuiArgs("Value", viewConfig_.BOLD ? "true" : "false"));

		AAGUIEXEC(L"contentView", L"checkUNDERLINE", L"Set", 
			AAGuiArgs("Value", viewConfig_.UNDERLINE ? "true" : "false"));

		AAGUIEXEC(L"contentView", L"checkIMAGE", L"Set", 
			AAGuiArgs("Value", viewConfig_.VIEW_IMAGE ? "true" : "false"));
		
		/*AAGUIEXEC(L"contentView", L"checkHDRAG", L"Set", 
			AAGuiArgs("Value", viewConfig_.HDRAG ? "true" : "false"));*/
	
		AAGUIEXEC(L"contentView", L"DefaultContet", L"FixBug", 
			AAGuiArgs());

		if(!init_open_file.empty())
		{
			AAGuiArgs openArg;

			openArg.WriteStringW("filew", init_open_file);
			dataExec(0, "obj_document", "open_txt",
				openArg, ReaderWindow::OnParserDocumentEnd);
		}
		
		/*AAGuiArgs* css_list = new AAGuiArgs();


		AAGuiArgs* one_css = new AAGuiArgs();
		one_css->WriteString("Value", "css2\\default.css");
		css_list->WriteValue(one_css);

		AAGuiArgs* one_css2 = new AAGuiArgs();
		one_css2->WriteString("Value", "css2\\table.css");
		css_list->WriteValue(one_css2);

		AAGuiArgs* one_css3 = new AAGuiArgs();
		one_css3->WriteString("Value", "css2\\bkg.css");
		css_list->WriteValue(one_css3);

		AAGuiArgs* one_css4 = new AAGuiArgs();
		one_css4->WriteString("Value", "css2\\text.css");
		css_list->WriteValue(one_css4);

		AAGuiArgs* one_css5 = new AAGuiArgs();
		one_css5->WriteString("Value", "css2\\combo.css");
		css_list->WriteValue(one_css5);

		AAGuiArgs* one_css6 = new AAGuiArgs();
		one_css6->WriteString("Value", "css2\\check.css");
		css_list->WriteValue(one_css6);


		AAGuiArgs reloadArg;

		reloadArg.WriteValue("css_list", css_list);


		reloadArg.WriteString("file_name", "blue.skin");

		AAGUIEXEC(0, L"root", L"ReloadSkin", reloadArg);*/
			/*AAGuiArgs timeArg;

			dataExec(0, "obj_document", "update_time",
				timeArg, ReaderWindow::OnTimeUpdate);*/
	}
}
void ReaderWindow::OnNewFrameCreated(LPCTSTR  frame_id)
{
	__FRAME_BEGIN__(L"ExitConfig", OnExitFrame())
	else if(frame_id_ == L"text-color-option")
	{
		GUI_BIND_AREA(L"root", L"text-color-option", ReaderWindow::OnOptionPopupEvent);
	}
	else if(frame_id_ == L"text-shadow-option")
	{
		GUI_BIND_AREA(L"root", L"text-shadow-option", ReaderWindow::OnOptionPopupEvent);
	}
	else if(frame_id_ == L"page-option")
	{
		GUI_BIND_AREA(L"root", L"page-option", ReaderWindow::OnPageOptionEvent);
	}
	else if(frame_id_ == L"text-option")
	{
		GUI_BIND_AREA(L"root", L"text-option", ReaderWindow::OnTextOptionEvent);
		LoadTextColorOption();
	}
	else if(frame_id_ == L"color-revise-select")
	{
		TEXT_CONFIG&  textConfig_ = AppConfigure::Instance()->GetTextConfig();

		int RValue = GetRValue(textConfig_.CLRSEL);
		int GValue = GetGValue(textConfig_.CLRSEL);
		int BValue = GetBValue(textConfig_.CLRSEL);

		AAGuiArgs ColorArg;

		ColorArg.WriteString("RValue", str_conv::int_2_string(RValue));
		ColorArg.WriteString("GValue", str_conv::int_2_string(GValue));
		ColorArg.WriteString("BValue", str_conv::int_2_string(BValue));

		AAGUIEXEC(L"color-revise-select", L"ColorSelReviseSelect", L"SetBkColor", 
			ColorArg);
	}
	else if(frame_id_ == L"color-revise-text")
	{
		TEXT_CONFIG&  textConfig_ = AppConfigure::Instance()->GetTextConfig();

		int RValue2 = GetRValue(textConfig_.CLRSELTEXT);
		int GValue2 = GetGValue(textConfig_.CLRSELTEXT);
		int BValue2 = GetBValue(textConfig_.CLRSELTEXT);

		AAGuiArgs ColorArg2;
		ColorArg2.WriteString("RValue", str_conv::int_2_string(RValue2));
		ColorArg2.WriteString("GValue", str_conv::int_2_string(GValue2));
		ColorArg2.WriteString("BValue", str_conv::int_2_string(BValue2));

		AAGUIEXEC(L"color-revise-text", L"ColorSelReviseText", L"SetBkColor", 
			ColorArg2);
	}
	else if(frame_id_ == L"user-login")
	{
		GUI_BIND_AREA(L"root", L"user-login", ReaderWindow::OnLoginEvent);

		CheckLoginInfo();
	}else if(frame_id_ == L"upload-avatar")
	{
		GUI_BIND_AREA(L"root", L"upload-avatar", ReaderWindow::OnUserEvent);
	}
	else if(frame_id_ == L"aoe-main-option")
	{
		GUI_BIND_AREA(L"root", L"aoe-main-option", ReaderWindow::OnMainOption);
		WriteMainOption();
	}
	else if(frame_id_ == L"edit-user-profile")
	{
		GUI_BIND_AREA(L"root", L"edit-user-profile", ReaderWindow::OnEditUserProfile);
		WriteNowProfile();

	}/*else if(frame_id_ == L"edit-selfintro")
	{
		GUI_BIND_AREA(L"root", L"edit-selfintro", ReaderWindow::OnEditUserProfile);
		WriteNowSelfIntro();
	}*/
	else if(frame_id_ == L"history-view")
	{
		GUI_BIND_AREA(L"root", L"history-view", ReaderWindow::OnHistoryEvent);
		QueryHistory();
	}
	else if(frame_id_ == L"aoe-version")
	{
		AAGuiArgs guiArg;
		guiArg.WriteStringW("Value", RrightString);
		AAGUIEXEC(L"aoe-version", L"AoeText", L"Set",  guiArg);
		GUI_SETW(L"aoe-version", L"AoeHome", L"http://www.AoeReader.com");
		GUI_BIND_AREA(L"root", L"aoe-version", ReaderWindow::OnVersionEvent);

	}
	else if(frame_id_ == L"aoe-support")
	{
		
		GUI_SETW(L"aoe-support", L"AoeMail", L"tonoshiki123@gmail.com");
		GUI_BIND_AREA(L"root", L"aoe-version", ReaderWindow::OnVersionEvent);

	}
	__FRAME_END__;
}
void ReaderWindow::OnExitFrame()
{
	GUI_BIND_AREA(0, L"ExitConfig", ReaderWindow::OnExit);

	std::wstring close_value = L" ";
	close_value += open_file_name;

	GUI_SETW(L"ExitConfig", L"FileName", close_value);
}
void  ReaderWindow::OnToolbarEvent(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();

	if(element_id == "btnMainAdd")
	{

		AAGuiArgs open_file_arg;
		READ_CONFIG& readConfig_ = AppConfigure::Instance()->GetReadConfig();

		if(!readConfig_.last_txt_path.empty())
		{
			open_file_arg.WriteStringW("path", readConfig_.last_txt_path);
		}
		open_file_arg.WriteString("filter1", "txt文件(*.txt)|*.txt|所有文件(*.*)|*.*");
		open_file_arg.WriteString("filter2", "*.txt;*.txt;");
		open_file_arg.WriteString("ext", "conf");
		open_file_arg.WriteString("title", "打开文件");

		AAGUIEXEC(0, L"root", L"OpenFile", open_file_arg);

		std::string sel_path_result_ = open_file_arg.GetString("result");
		std::wstring sel_path_result_w = open_file_arg.GetStringW("resultw");
		if(!sel_path_result_w.empty())
		{
			AppConfigure::Instance()->WriteLastOpenDir(sel_path_result_w, tOpenDirTxt);

			if(!has_open_file)
			{
				AAGuiArgs openArg;

				openArg.WriteString("file", sel_path_result_);
				openArg.WriteStringW("filew", sel_path_result_w);
				dataExec(0, "obj_document", "open_txt",
					openArg, ReaderWindow::OnParserDocumentEnd);
			}
			else
			{
				AAGuiArgs openArg;

				openArg.WriteStringW("file_name", sel_path_result_w);
				dataExecNULL(0, "obj_document", "open_new_window",
					openArg);
			}

		}
	}
	/*else if(element_id == "btnOption")
	{
		GUI_SET(NULL, L"WorkTab", "3");

		}*//*else if(element_id == "btnHistory")
		{
		GUI_SET(NULL, L"WorkTab", "4");

		}*/else if(element_id == "btnReaderPage")
	{
		/*if(has_open_file)
		{
			GUI_SET(NULL, L"WorkTab", "2");
		}
		else
		{
			GUI_SET(NULL, L"WorkTab", "1");
		}*/
		//<stylesheet src="css2\\default.css" />
		//	<stylesheet src="css2\\datepicker.css" />
		//	<stylesheet src="css2\\table.css" />
		//	<stylesheet src="css2\\bkg.css" />
		//	<stylesheet src="css2\\check.css" />
		//	<stylesheet src="css2\\text.css" />
		//	<stylesheet src="css2\\combo.css" />
		
	}
}
void  ReaderWindow::OnTitleFuncBarEvent(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();

	if(element_id == "btnDragView")
	{
		AAGUIEXEC(L"TitleFuncBar", L"btnDragView", L"Enable", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"TitleFuncBar", L"btnSelText", L"Enable", AAGuiArgs("Value", "true"));
	}
	else if(element_id == "btnSelText")
	{
		AAGUIEXEC(L"TitleFuncBar", L"btnDragView", L"Enable", AAGuiArgs("Value", "true"));
		AAGUIEXEC(L"TitleFuncBar", L"btnSelText", L"Enable", AAGuiArgs("Value", "false"));
	}
	//else if(element_id == "btnFindText")
	//{
	//	GUI_SET(L"FindTextBar", L"EditSearchText", "");
	//	AAGUIEXEC(L"contentView", L"FindTextBar", L"ChangeVisible", AAGuiArgs("Value", "true"));
	//	AAGUIEXEC(L"contentView", L"FindBarReDrawArea", L"Update", AAGuiArgs());
	//}
	else if(element_id == "btnHideFindBar")
	{
		AAGUIEXEC(L"contentView", L"FindTextBar", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"contentView", L"FindBarReDrawArea", L"Update", AAGuiArgs());

	}
	else if(element_id == "checkSidebar")
	{
		std::string bool_value = e.GetString("Value");


		if(bool_value == "true")
		{
			ModuleConfig conf;
			conf.write_string(L"USERINTER", L"SHOWBOOKMARK", L"false");
			conf.write_string(L"USERINTER", L"SHOWTOOLBAR", L"false");

			AAGUIEXEC(L"contentView", L"BookMark", L"SetVisible", AAGuiArgs("Value", "false"));
			AAGUIEXEC(L"contentView", L"SepraterLeft", L"SetVisible", AAGuiArgs("Value", "false"));
			AAGUIEXEC(L"contentView", L"tabshowBookMarkArea", L"SetVisible", AAGuiArgs("Value", "true"));
			AAGUIEXEC(L"contentView", L"tabshowBookMarkArea", L"Set", AAGuiArgs("Value", "2"));
			AAGUIEXEC(L"contentView", L"tabHideBookMark", L"Set", AAGuiArgs("Value", "2"));
			AAGUIEXEC(L"root", L"contentView", L"Update", AAGuiArgs());

			AAGUIEXEC(L"contentView", L"ReaderOptionBar", L"SetVisible", AAGuiArgs("Value", "false"));
			AAGUIEXEC(L"root", L"contentView", L"Update", AAGuiArgs());
			AAGUIEXEC(L"contentView", L"PageShowHideTab", L"Set", AAGuiArgs("Value", "1"));
		}
		else
		{
			ModuleConfig conf;
			conf.write_string(L"USERINTER", L"SHOWBOOKMARK", L"true");
			conf.write_string(L"USERINTER", L"SHOWTOOLBAR", L"true");

			AAGUIEXEC(L"contentView", L"BookMark", L"SetVisible", AAGuiArgs("Value", "true"));
			AAGUIEXEC(L"contentView", L"SepraterLeft", L"SetVisible", AAGuiArgs("Value", "true"));
			AAGUIEXEC(L"contentView", L"tabshowBookMarkArea", L"SetVisible", AAGuiArgs("Value", "false"));
			AAGUIEXEC(L"contentView", L"tabshowBookMarkArea", L"Set", AAGuiArgs("Value", "1"));
			AAGUIEXEC(L"contentView", L"tabHideBookMark", L"Set", AAGuiArgs("Value", "1"));
			AAGUIEXEC(L"root", L"contentView", L"Update", AAGuiArgs());


			AAGUIEXEC(L"contentView", L"ReaderOptionBar", L"SetVisible", AAGuiArgs("Value", "true"));
			AAGUIEXEC(L"root", L"contentView", L"Update", AAGuiArgs());
			AAGUIEXEC(L"contentView", L"PageShowHideTab", L"Set", AAGuiArgs("Value", "2"));
		}
	}else if(element_id == "btnMainAdd")
	{
		OnToolbarEvent(sender, e);
	//	AAGUIEXEC(L"contentView", L"DocTab", L"AddTab", AAGuiArgs());
	}
}
void  ReaderWindow::OnPreviewFrameEvent(const AAObject* sender, const AAGuiArgs& e)
{

	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();

	if(element_id == "btnPreviewOpen")
	{

		AAGuiArgs open_file_arg;

		READ_CONFIG& readConfig_ = AppConfigure::Instance()->GetReadConfig();

		if(!readConfig_.last_txt_path.empty())
		{
			open_file_arg.WriteStringW("path", readConfig_.last_txt_path);
		}
		open_file_arg.WriteString("filter1", "txt文件(*.txt)|*.txt|所有文件(*.*)|*.*");
		open_file_arg.WriteString("filter2", "*.txt;*.txt;");
		open_file_arg.WriteString("ext", "conf");
		open_file_arg.WriteString("title", "打开文件");

		AAGUIEXEC(0, L"root", L"OpenFile", open_file_arg);

		std::string sel_path_result_ = open_file_arg.GetString("result");
		std::wstring sel_path_result_w = open_file_arg.GetStringW("resultw");

		if(!sel_path_result_w.empty())
		{
			AppConfigure::Instance()->WriteLastOpenDir(sel_path_result_w, tOpenDirTxt);
			AAGuiArgs openArg;

			openArg.WriteString("file", sel_path_result_);
			openArg.WriteStringW("filew", sel_path_result_w);
			dataExec(0, "obj_document", "open_txt",
				openArg, ReaderWindow::OnParserDocumentEnd);

		}

	}else if(element_id == "btnAllDeleteRecent")
	{

		AAGuiArgs recentArg;
		dataExecNULL(0, "obj_document", "delete_all_recent",
			recentArg);
		AAGUIEXEC(L"ReaderPreview", L"ListRecentRead", L"RemoveAll", recentArg);
		AAGUIEXEC(L"ReaderPreview", L"btnAllDeleteRecent", L"SetVisible", AAGuiArgs("Value", "false"));

	}else if(element_id == "btnDeleteRecent")
	{

		AAGuiArgs recentArg;

		std::string record_id = e.GetString("id");

		recentArg.WriteString("id", record_id);

		dataExecNULL(0, "obj_document", "delete_recent",
			recentArg);
		AAGUIEXEC(L"ReaderPreview", L"ListRecentRead", L"RemoveSelectDelay", recentArg);
	
	}
	else if(element_id == "btnLinkRecentRead")
	{

		AAGuiArgs recentArg;

		std::string record_id = e.GetString("id");

		std::wstring path = e.GetStringW("path");
		std::string page = e.GetString("page");
		std::wstring other = e.GetStringW("other");
		if(!path.empty())
		{


			AAGuiArgs openArg;

			openArg.WriteStringW("filew", path);
			openArg.WriteStringW("other", other);
			openArg.WriteString("page", page);
			dataExec(0, "obj_document", "open_txt",
				openArg, ReaderWindow::OnParserDocumentEnd);
		}

	}
	else if(element_id == "btnHome")
	{
		::ShellExecute(NULL, L"open", L"http://www.AoeReader.com", NULL, NULL, SW_SHOWNORMAL);
	}
	else if(element_id == "btnTwitter")
	{
		::ShellExecute(NULL, L"open", L"https://twitter.com/tonoshikipiyoma", NULL, NULL, SW_SHOWNORMAL);
	}

}
void ReaderWindow::OnParserDocumentEnd(const dataObj *sender, const AAGuiArgs& value)
{
	//_CrtSetBreakAlloc(316881);
	//std::wstring   content = value.GetStringW("Value");
	std::string   error = value.GetString("error");
	if(error != "false")
	{
		std::wstring file_name = value.GetStringW("doc_name");
		std::wstring errorMsg = L"载入文档";
		errorMsg += file_name;
		errorMsg += L"时发生错误";
		GUI_SETW(L"ReaderPreview", L"ErrorOpenMsg", errorMsg);
		GUI_SET(L"ReaderPreview", L"error_open_tab", "2");
		return;
	}
	std::string   total_page = value.GetString("total_page");
	std::wstring file_name = value.GetStringW("file_name");
	std::string chapter_page_offset = value.GetString("page");
	std::wstring chapter_page_other = value.GetStringW("other");
	std::string text_max_line_len = value.GetString("max_line_len");
	open_file_name = file_name;//str_conv::str2wstr(file_name);
	
	std::wstring window_title = file_name;
	window_title += L" - AoeReader";
	SetWindowText(widget_window_->GetNativeWindow(), window_title.c_str());
	//GUI_SET(L"contentView", L"PageInfo", total_page);
//	GUI_SET(L"contentView", L"DefaultContet", content);

	//AAGuiArgs newContent;
	//newContent.WriteStringW("Value", content);

	AAGuiArgs* newContent = value.GetValue("pages");
	
	if(newContent)
	{
		if(!text_max_line_len.empty())
		{
			newContent->WriteString("max_line_len", text_max_line_len);
		}
		AAGUIEXEC(L"contentView", L"DefaultContet", L"Set", *newContent)
	}
	AAGuiArgs scrollArg;
	scrollArg.WriteString("Value", chapter_page_offset);
	scrollArg.WriteStringW("other", chapter_page_other);
	AAGUIEXEC(L"contentView", L"DefaultContet", L"ScrollToPage", scrollArg);

	AAGuiArgs* chapterList = value.GetValue("chapter");
	AAGUIEXEC(L"contentView", L"ListChapter", L"RemoveAll", AAGuiArgs());
	if(chapterList)
	{
		int chapterListSize = chapterList->GetValue().size();

		std::string IndexText = "目录(";
		IndexText += str_conv::int_2_string(chapterListSize);
		IndexText += ")";
		//i18n
		GUI_SET(L"contentView", L"LableIndexText", IndexText);
		if(chapterListSize)
		AAGUIEXEC(L"contentView", L"ListChapter", L"AddItem", *chapterList);
	}
	


	AAGuiArgs* bookmarkList = value.GetValue("bookmark");
	AAGUIEXEC(L"contentView", L"ListBookMark", L"RemoveAll", AAGuiArgs());
	if(bookmarkList && bookmarkList->HasListValue())
		AAGUIEXEC(L"contentView", L"ListBookMark", L"AddItem", *bookmarkList);

	if(AppConfigure::Instance()->IsFirstRunApp())
	{
		RECT rcDlg = { 0 };

		HWND window_handle_ = widget_window_->GetNativeWindow();
		::GetWindowRect(window_handle_, &rcDlg);
		RECT rcArea = { 0 };
		RECT rcCenter = { 0 };
		
		HWND hWndCenter = NULL;
		::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);

		if( hWndCenter == NULL ) 
			rcCenter = rcArea; 
		else 
			::GetWindowRect(hWndCenter, &rcCenter);

		int DlgWidth = 1086;
		int DlgHeight = 992;

		
		int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
		int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

	
		if( xLeft < rcArea.left ) xLeft = rcArea.left;
		else if( xLeft + DlgWidth > rcArea.right ) xLeft = rcArea.right - DlgWidth;
		if( yTop < rcArea.top ) yTop = rcArea.top;
		else if( yTop + DlgHeight > rcArea.bottom ) yTop = rcArea.bottom - DlgHeight;
		::SetWindowPos(window_handle_, NULL, xLeft, yTop, DlgWidth, DlgHeight,  SWP_NOZORDER | SWP_NOACTIVATE);
	}
	else
	{
		if(widget_window_ && !has_open_file)
		{
			RECT window_rect = AppConfigure::Instance()->GetPosValue();

			if(window_rect.right > window_rect.left && window_rect.bottom > window_rect.top
				&& window_rect.left > 0 && window_rect.right > 0
				&& window_rect.top > 0 && window_rect.bottom > 0 )
			{
				HWND window_handle_ = widget_window_->GetNativeWindow();

				if(!IsZoomed(window_handle_))
					MoveWindow(window_handle_, window_rect.left, window_rect.top, window_rect.right - window_rect.left,
					window_rect.bottom - window_rect.top, true);

			}

		}
	}
	
	has_open_file = true;
	CheckAvatarProcedure();
	/*if(is_big_avatar)
	{
		AAGUIEXEC(L"root", L"SystemBar", L"SetHeight", AAGuiArgs("Value", "50"));
	}
	else
	{

		AAGUIEXEC(L"root", L"SystemBar", L"SetHeight", AAGuiArgs("Value", "35"));
	}*/

	if(is_big_avatar && is_user_login)
	{
		AAGUIEXEC(L"root", L"SystemBar", L"SetHeight", AAGuiArgs("Value", "50"));
	}
	else
	{

		AAGUIEXEC(L"root", L"SystemBar", L"SetHeight", AAGuiArgs("Value", "35"));
	}

	AAGUIEXEC(L"root", L"FirstCutLine", L"SetVisible", AAGuiArgs("Value", "false"));

	GUI_SET(L"root", L"WorkTab", "2");
	GUI_SETW(L"root", L"file_name", file_name);

	AAGUIEXEC(L"contentView", L"DocTab", L"AddTab", AAGuiArgs());

	AAGUIEXEC(L"ViewMenu", L"MenuTextColor1", L"Enable", AAGuiArgs("Value", "true"));
	AAGUIEXEC(L"ViewMenu", L"MenuTextColor2", L"Enable", AAGuiArgs("Value", "true"));
	AAGUIEXEC(L"ViewMenu", L"MenuTextColor3", L"Enable", AAGuiArgs("Value", "true"));
	AAGUIEXEC(L"ViewMenu", L"MenuPageStyle", L"Enable", AAGuiArgs("Value", "true"));
	//AAGUIEXEC(L"contentView", L"DefaultContet",L"FixBug2", AAGuiArgs());
//	char* pChars = new char[10];
	//_CrtDumpMemoryLeaks();
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
}
void ReaderWindow::OnTimeUpdate(const dataObj *sender, const AAGuiArgs& value)
{

	//std::string time_string_ = value.GetString("time");

	//GUI_SET(L"TitleFuncBar", L"ClientTime", time_string_);
}
void  ReaderWindow::OnBookMarkEvent(const AAObject* sender, const AAGuiArgs& e)
{
	std::wstring   content = e.GetStringW("Value1");
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();

	if(element_id == "btnAddBookMark")
	{
		/*AAGuiArgs testArg;
		testArg.WriteString("page", "233");
		testArg.WriteStringW("name", L"测试项");
		AAGUIEXEC(L"contentView", L"ListBookMark", L"AddItemFront", testArg);*/

		AAGuiArgs PageArg;

		AAGUIEXEC(L"contentView", L"DefaultContet", L"GetCurrentPage", PageArg);

		dataExecNULL(0, "obj_document", "add_bookmark", PageArg);

		AAGUIEXEC(L"contentView", L"ListBookMark", L"AddItemFront", PageArg);

	}
	else if(element_id == "btnEditBookMark")
	{

		
		AAGUIEXEC(L"contentView", L"ListBookMark", L"EditHoverNow", AAGuiArgs());
	}
	else if(element_id == "btnDeleteBookMark")
	{
		std::string record_id = e.GetString("id");
		AAGuiArgs bookmarkArg;
		bookmarkArg.WriteString("id", record_id);
		dataExecNULL(0, "obj_document", "delete_bookmark", bookmarkArg);

		AAGUIEXEC(L"contentView", L"ListBookMark", L"RemoveHoverDelay", AAGuiArgs());
	}
	else if(element_id == "btnHideBookmark")
	{
		ModuleConfig conf;
		conf.write_string(L"USERINTER", L"SHOWBOOKMARK", L"false");
		AAGuiArgs isVisibleArg;

		AAGUIEXEC(L"contentView", L"ReaderOptionBar", L"IsVisible", isVisibleArg);

		if(isVisibleArg.GetString("Value") != "true")
		{
			AAGUIEXEC(L"contentView", L"checkSidebar", L"Set", AAGuiArgs("Value", "true"));
		}

		
		
		AAGUIEXEC(L"contentView", L"BookMark", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"contentView", L"SepraterLeft", L"SetVisible", AAGuiArgs("Value", "false"));
		/*AAGUIEXEC(L"contentView", L"showBookMarkArea", L"SetVisible", AAGuiArgs("Value", "true"));*/
		AAGUIEXEC(L"contentView", L"tabshowBookMarkArea", L"SetVisible", AAGuiArgs("Value", "true"));
		AAGUIEXEC(L"contentView", L"tabshowBookMarkArea", L"Set", AAGuiArgs("Value", "2"));
		AAGUIEXEC(L"contentView", L"tabHideBookMark", L"Set", AAGuiArgs("Value", "2"));
		AAGUIEXEC(L"root", L"contentView", L"Update", AAGuiArgs());
	}
	else if(element_id == "btnShowBookmark")
	{
		ModuleConfig conf;
	
		conf.write_string(L"USERINTER", L"SHOWBOOKMARK", L"true");
		AAGUIEXEC(L"contentView", L"checkSidebar", L"Set", AAGuiArgs("Value", "false"));
		
		AAGUIEXEC(L"contentView", L"BookMark", L"SetVisible", AAGuiArgs("Value", "true"));
		AAGUIEXEC(L"contentView", L"SepraterLeft", L"SetVisible", AAGuiArgs("Value", "true"));
	//	AAGUIEXEC(L"contentView", L"showBookMarkArea", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"contentView", L"tabshowBookMarkArea", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"contentView", L"tabshowBookMarkArea", L"Set", AAGuiArgs("Value", "1"));
		AAGUIEXEC(L"contentView", L"tabHideBookMark", L"Set", AAGuiArgs("Value", "1"));
		AAGUIEXEC(L"root", L"contentView", L"Update", AAGuiArgs());
	}
	if(event_name_ == "EditEnd")
	{
		std::wstring data_text = e.GetStringW("text");
		std::wstring record_text = e.GetStringW("name");
		
		if(data_text != record_text)
		{
			std::string record_id = e.GetString("id");
			AAGuiArgs bookmarkArg;
			bookmarkArg.WriteString("id", record_id);
			bookmarkArg.WriteStringW("name", data_text);
		

			dataExecNULL(0, "obj_document", "update_bookmark", bookmarkArg);
		}
	}
	else if(event_name_ == "ListViewSelectChanged" || event_name_ == "ListViewSelectActive")
	{
		std::string list_view_id_ = e.GetString("ListView");

		if(list_view_id_ == "ListChapter")
		{
			std::string chapter_page_offset = e.GetString("offset");
			std::wstring other = e.GetStringW("other");
			int convert_page = str_conv::string_2_int(chapter_page_offset);
			convert_page += 1;
			chapter_page_offset = str_conv::int_2_string(convert_page);

			AAGuiArgs scrollArg;
			scrollArg.WriteString("Value", chapter_page_offset);
			scrollArg.WriteStringW("other", other);
			AAGUIEXEC(L"contentView", L"DefaultContet", L"ScrollToPage", scrollArg);
		}
		else if(list_view_id_ == "ListBookMark")
		{
			std::string chapter_page_offset = e.GetString("page");
			std::wstring other = e.GetStringW("other");
			AAGuiArgs scrollArg;
			scrollArg.WriteString("Value", chapter_page_offset);
			scrollArg.WriteStringW("other", other);
			AAGUIEXEC(L"contentView", L"DefaultContet", L"ScrollToPage", scrollArg);
		}
	}

}
void  ReaderWindow::OnExit(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();

	if(widget_window_ && has_open_file)
	{
		HWND window_handle_ = widget_window_->GetNativeWindow();

		RECT window_rect;
		GetWindowRect(window_handle_, &window_rect);

		AppConfigure::Instance()->WritePosValue(window_rect.left, window_rect.right, 
			window_rect.top, window_rect.bottom);
	}

	if(element_id == "ExitNow")
	{
		if(has_open_file)
		{
			AAGuiArgs PageArg;

			AAGUIEXEC(L"contentView", L"DefaultContet", L"GetCurrentPage", PageArg);

			dataExecNULL(0, "obj_document", "exit_module", PageArg);
			AAGUIEXEC(0, L"root", L"ExitMainLoop", AAGuiArgs());

			
		}
		else
		{
			AAGUIEXEC(0, L"root", L"ExitMainLoop", AAGuiArgs());
		}
	}
	else if(element_id == "checkCloseQuery")
	{
		std::string is_enable =	e.GetString("Value");

		
		ModuleConfig conf;

		if(is_enable == "true")
		{
			conf.write_string(L"USER", L"CLOSEQUERY", L"false");
		}
		else
		{
			conf.write_string(L"USER", L"CLOSEQUERY", L"true");
		}
	}

	
}
void  ReaderWindow::OnNativeClose(const AAObject* sender, const AAGuiArgs& e)
{
	if(has_open_file)
	{
		AAGuiArgs PageArg;

		AAGUIEXEC(L"contentView", L"DefaultContet", L"GetCurrentPage", PageArg);

		dataExecNULL(0, "obj_document", "exit_module", PageArg);
		AAGUIEXEC(0, L"root", L"ExitMainLoop", AAGuiArgs());
	}
	else
	{
		AAGUIEXEC(0, L"root", L"ExitMainLoop", AAGuiArgs());
	}

	//if(widget_window_)
	//{
	//	HWND window_handle_ = widget_window_->GetNativeWindow();

	//	RECT window_rect;
	//	GetWindowRect(window_handle_, &window_rect);

	//	AppConfigure::Instance()->WritePosValue(window_rect.left, window_rect.right, 
	//		window_rect.top, window_rect.bottom);
	//}
}
void ReaderWindow::InitOpenFile(std::wstring file_)
{
	init_open_file = file_;
}
void ReaderWindow::WriteMainOption()
{
	ModuleConfig conf;

	std::wstring closeQuery = conf.query_string(L"USER", L"CLOSEQUERY");
	std::wstring is_auto_login	= conf.query_string(L"USER", L"AUTOLOGIN");
	std::wstring is_view_big	= conf.query_string(L"USER", L"VIEWBIG");
	std::wstring is_join_proj	= conf.query_string(L"AOEPROJ", L"JOIN");

	GUI_SET(L"aoe-main-option", L"checkAUTOLOGIN", str_conv::wstr2str(is_auto_login));
	GUI_SET(L"aoe-main-option", L"checkVIEWBIG", str_conv::wstr2str(is_view_big));

	if(is_join_proj == L"true")
	{
		GUI_SET(L"aoe-main-option", L"ClientUE", "ClientUEYES");
	}
	else
	{
		GUI_SET(L"aoe-main-option", L"ClientUE", "ClientUENO");
	}
	GUI_SET(L"aoe-main-option", L"checkCLOSEQUERY", str_conv::wstr2str(closeQuery));
}
void ReaderWindow::CheckSideProcedure()
{
	ModuleConfig conf;
	std::wstring SHOWBOOKMARK = conf.query_string(L"USERINTER", L"SHOWBOOKMARK");
	std::wstring SHOWTOOLBAR	= conf.query_string(L"USERINTER", L"SHOWTOOLBAR");

	if(SHOWBOOKMARK == L"false" && SHOWTOOLBAR == L"false")
	{
		AAGUIEXEC(L"contentView", L"checkSidebar", L"Set", AAGuiArgs("Value", "true"));
		AAGUIEXEC(L"contentView", L"BookMark", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"contentView", L"SepraterLeft", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"contentView", L"tabshowBookMarkArea", L"SetVisible", AAGuiArgs("Value", "true"));
		AAGUIEXEC(L"contentView", L"tabshowBookMarkArea", L"SetSilent", AAGuiArgs("Value", "2"));
		AAGUIEXEC(L"contentView", L"tabHideBookMark", L"SetSilent", AAGuiArgs("Value", "2"));
		AAGUIEXEC(L"root", L"contentView", L"Update", AAGuiArgs());

		AAGUIEXEC(L"contentView", L"ReaderOptionBar", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"root", L"contentView", L"Update", AAGuiArgs());
		AAGUIEXEC(L"contentView", L"PageShowHideTab", L"SetSilent", AAGuiArgs("Value", "1"));
	}
	else if(SHOWBOOKMARK == L"false" && SHOWTOOLBAR != L"false")
	{
		AAGuiArgs isVisibleArg;

		AAGUIEXEC(L"contentView", L"ReaderOptionBar", L"IsVisible", isVisibleArg);

		if(isVisibleArg.GetString("Value") != "true")
		{
			AAGUIEXEC(L"contentView", L"checkSidebar", L"Set", AAGuiArgs("Value", "true"));
		}

		AAGUIEXEC(L"contentView", L"BookMark", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"contentView", L"SepraterLeft", L"SetVisible", AAGuiArgs("Value", "false"));
	
		AAGUIEXEC(L"contentView", L"tabshowBookMarkArea", L"SetVisible", AAGuiArgs("Value", "true"));
		AAGUIEXEC(L"contentView", L"tabshowBookMarkArea", L"SetSilent", AAGuiArgs("Value", "2"));
		AAGUIEXEC(L"contentView", L"tabHideBookMark", L"SetSilent", AAGuiArgs("Value", "2"));

			//bug
			AAGUIEXEC(L"contentView", L"PageShowHideTab", L"SetSilent", AAGuiArgs("Value", "2"));
	
	}
	else if(SHOWTOOLBAR == L"false" && SHOWBOOKMARK != L"false")
	{
		AAGuiArgs isVisibleArg;

		AAGUIEXEC(L"contentView", L"BookMark", L"IsVisible", isVisibleArg);

		if(isVisibleArg.GetString("Value") != "true")
		{
			AAGUIEXEC(L"contentView", L"checkSidebar", L"Set", AAGuiArgs("Value", "true"));
		}
		AAGUIEXEC(L"contentView", L"ReaderOptionBar", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"root", L"contentView", L"Update", AAGuiArgs());
		AAGUIEXEC(L"contentView", L"PageShowHideTab", L"SetSilent", AAGuiArgs("Value", "1"));
	}
	else if(SHOWTOOLBAR != L"false" && SHOWBOOKMARK != L"false")
	{
	}
}