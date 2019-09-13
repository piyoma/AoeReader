#include "ReaderWindow.h"
#include "AppConfig.h"

void  ReaderWindow::OnMainOption(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();

	if(element_id == "checkAUTOLOGIN" || element_id == "ClientUE"
		|| element_id == "checkCLOSEQUERY" || element_id == "checkVIEWBIG")
	{
		AAGUIEXEC(L"aoe-main-option", L"btnSaveMainOption", L"Enable", AAGuiArgs("Value", "true"));
	}
	else if(element_id == "btnSaveMainOption")
	{
		std::string is_auto_login = GUI_GET(L"aoe-main-option", L"checkAUTOLOGIN");

		std::string is_ClientUE = GUI_GET(L"aoe-main-option", L"ClientUE");

		std::string is_CLOSEQUERY= GUI_GET(L"aoe-main-option", L"checkCLOSEQUERY");

		std::string is_VIEWBIG = GUI_GET(L"aoe-main-option", L"checkVIEWBIG");

		ModuleConfig conf;

		if(is_auto_login == "true")
		{
			conf.write_string(L"USER", L"AUTOLOGIN", L"true");
		}
		else
		{
			conf.write_string(L"USER", L"AUTOLOGIN", L"false");
		}

		if(is_ClientUE == "ClientUEYES")
		{
			conf.write_string(L"AOEPROJ", L"JOIN", L"true");
		}
		else
		{
			conf.write_string(L"AOEPROJ", L"JOIN", L"false");
		}

		if(is_CLOSEQUERY == "true")
		{
			conf.write_string(L"USER", L"CLOSEQUERY", L"true");
		}
		else
		{
			conf.write_string(L"USER", L"CLOSEQUERY", L"false");
		}

		if(is_VIEWBIG == "true")
		{
			conf.write_string(L"USER", L"VIEWBIG", L"true");
			is_big_avatar = true;
			if(is_user_login)
			{
				GUI_SET(L"btnAoeUser", L"checkBIGAVATAR", "true");
				CheckBigAvatar();
			}
		}
		else
		{
			conf.write_string(L"USER", L"VIEWBIG", L"false");
			is_big_avatar = false;
			if(is_user_login)
			{
				GUI_SET(L"btnAoeUser", L"checkBIGAVATAR", "false");
				CheckSmallAvatar();
			}
		}

		AAGUIEXEC(0, L"root", L"popup_layer", AAGuiArgs("command", "anime_destory"));
	}
}