#include "ReaderWindow.h"
#include "AppConfig.h"
#include <ShellAPI.h>

void  ReaderWindow::OnMainMenuEvent(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();


	if(element_id == "MenuHistory")
	{
		MenuProcudure("viewclient\\history.xml");
	}
	else if(element_id == "MenuOption")
	{
		AAGUIEXEC(NULL, L"root", L"HideContextLayer", AAGuiArgs());

		AAGuiArgs arg("document", "viewclient\\option.xml");
		arg.WriteString("command", "create");
		widget_window_->ExcuteCommand(0, L"root", L"popup_layer", arg);
	}
	else if(element_id == "MenuOpen")
	{
		AAGUIEXEC(NULL, L"root", L"HideContextLayer", AAGuiArgs());
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
	else if(element_id == "MenuNewWindow")
	{	

		AAGUIEXEC(NULL, L"root", L"HideContextLayer", AAGuiArgs());

		AAGuiArgs openArg;

		openArg.WriteStringW("file_name", L"");
		dataExecNULL(0, "obj_document", "open_new_window",
			openArg);
	}
	else if(element_id == "MenuEditProfile")
	{
		MenuProcudure("viewclient\\editprofile.xml");
	}
	else if(element_id == "MenuEditAvatar")
	{
		MenuProcudure("viewclient\\uploadavatar.xml");
	}
	else if(element_id == "MenuClearData")
	{
	//	MenuProcudure("viewclient\\clear_success.xml");
		ModuleConfig conf;
		conf.write_string(L"USER", L"CLIENT", L"");

		AAGUIEXEC(L"MenuUser", L"TabClearUserData", L"Set", AAGuiArgs("Value", "2"));
	}
}
bool ReaderWindow::MenuProcudure(std::string file_name)
{
	AAGUIEXEC(NULL, L"root", L"HideContextLayer", AAGuiArgs());

	AAGuiArgs arg("document", file_name);
	arg.WriteString("command", "create");
	widget_window_->ExcuteCommand(0, L"root", L"popup_layer", arg);

	return true;
}
void  ReaderWindow::OnVersionEvent(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();

	if(element_id == "btnAoeHome" || element_id == "AoeHome")
	{
		::ShellExecute(NULL, L"open", L"http://www.AoeReader.com", NULL, NULL, SW_SHOWNORMAL);
	}
}