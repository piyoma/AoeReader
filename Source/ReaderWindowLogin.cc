#include "ReaderWindow.h"
#include "AppConfig.h"
#include <codecvt>
#include <Shlwapi.h>
//static std::wstring AAGuiGetW(LPCTSTR  frame_id, LPCTSTR  control_id, AAWidgetWindow* widget_window_)
//{
//	if(widget_window_ == NULL) return L"";
//
//	AAGuiArgs Arg;
//
//	widget_window_->ExcuteCommand(frame_id, control_id, L"Get", Arg);
//
//	return Arg.GetStringW("ValueW");
//
//}
void  ReaderWindow::OnLoginEvent(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();

	if(element_id == "btnUserLogin" || element_id == "user-login")
	{

		AAGuiArgs checkFocus;
		AAGUIEXEC(L"user-login", L"btnCancelUserLogin", L"IsFocus", checkFocus);

		if(checkFocus.GetString("Value") == "true")
		{
			AAGUIEXEC(0, L"root", L"popup_layer", AAGuiArgs("command", "anime_destory"));
			return;
		}


		std::wstring user_mail = GUI_GETW(L"user-login", L"EditUserEmail", widget_window_);
		std::wstring user_password = GUI_GETW(L"user-login", L"UserPassWord", widget_window_);
	
		std::string str_user_mail = GUI_GET(L"user-login", L"EditUserEmail");
		std::string str_user_pass = GUI_GET(L"user-login", L"UserPassWord");

		
		std::string chkRemPass = GUI_GET(L"user-login", L"chkRemPass");
		std::string chkAutoLogin = GUI_GET(L"user-login", L"chkAutoLogin");

		if(str_user_mail.empty())
		{
			GUI_SET(L"user-login", L"TabTipMailEmpty", "2");
			return;
		}

		int size_pos_at = str_user_mail.find("@");

		if(size_pos_at == std::string::npos)
		{
			GUI_SET(L"user-login", L"TabTipMailEmpty", "3");
			return;
		}
		int dot_pos = str_user_mail.find( ".",size_pos_at);

		bool right_dot = false;
		if(dot_pos != size_pos_at + 1 && dot_pos != std::string::npos)
		{
			right_dot = true;
		}
		if(!right_dot)
		{
			GUI_SET(L"user-login", L"TabTipMailEmpty", "3");
			return;
		}
		if(str_user_pass.empty())
		{
			GUI_SET(L"user-login", L"TabTipPassEmpty", "2");
			return;
		}

		ModuleConfig conf;

		conf.write_string(L"USER", L"REMPASS", str_conv::str2wstr(chkRemPass));
		conf.write_string(L"USER", L"AUTOLOGIN", str_conv::str2wstr(chkAutoLogin));

		GUI_SET(L"user-login", L"loginWorkTab", "2");
		AAGuiArgs loginArg;

		loginArg.WriteStringW("mail", user_mail);
		loginArg.WriteStringW("pass", user_password);

		dataExec(0, "obj_document", "user_login",
			loginArg, ReaderWindow::OnWebData);

	}
	else if(element_id == "chkRemPass")
	{
		std::string boolValue = e.GetString("Value");

		if(!boolValue.empty())
		{
			ModuleConfig conf;
			conf.write_string(L"USER", L"REMPASS", str_conv::str2wstr(boolValue));

			if(boolValue == "false")
			{
				GUI_SET(L"user-login", L"chkAutoLogin", "false");
				ModuleConfig conf;
				conf.write_string(L"USER", L"AUTOLOGIN", L"false");
			}
		}
	}
	else if(element_id == "chkAutoLogin")
	{
		std::string boolValue = e.GetString("Value");

		if(!boolValue.empty())
		{

			ModuleConfig conf;
			conf.write_string(L"USER", L"AUTOLOGIN", str_conv::str2wstr(boolValue));

			if(boolValue == "true")
			{
				GUI_SET(L"user-login", L"chkRemPass", "true");
				ModuleConfig conf;
				conf.write_string(L"USER", L"REMPASS", L"true");
			}
		}
	}
}
void ReaderWindow::CheckLoginInfo()
{
	ModuleConfig conf;

	std::wstring ClientInfo = conf.query_string(L"USER", L"CLIENT");

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
			std::string mail = UserValue->GetString("mail");
			std::string pass = UserValue->GetString("pass");

			
			GUI_SET(L"user-login", L"EditUserEmail", mail);
			if(REMPASS == L"true")
				GUI_SET(L"user-login", L"UserPassWord", pass);

			//AAGUIEXEC(L"user-login", L"EditUserEmail", L"SetFocus", AAGuiArgs());

			

			
		}
	}
	

}
void ReaderWindow::OnWebData(const dataObj *sender, const AAGuiArgs& value)
{

	//std::string retValue = value.GetString("retvalue");

	std::string data_type = value.GetString("data_type");

	if(data_type == "login")
	{
		std::string succeed =  value.GetString("succeed");

		if(succeed == "true")
		{
			ModuleConfig conf;

			std::string  ClientInfo =  value.GetString("ClientInfo");
		     
			std::wstring photo_namew = conf.query_string(L"USER", L"PHOTONAME");

		

			std::string photo_name = str_conv::wstr2str(photo_namew);

			std::string photo_name_jpg = photo_name + ".jpg";

			if(oneAoeUser)
			{
				delete oneAoeUser;
				oneAoeUser = NULL;

			}

			oneAoeUser = new AAGuiArgs();

			AAGuiArgs*   newUser =  value.GetValue("AoeUser");

			std::string timeStr = "";
			if(newUser)
			{
				std::string userName = newUser->GetString("username");
				std::string country = newUser->GetString("country");
				std::string city = newUser->GetString("city");
				std::string intro = newUser->GetString("intro");
				std::string picture = newUser->GetString("picture");

				bool local_avatar = false;
				bool download_avatar = false;

		

				/*std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

				std::wstring wuserName = conv.from_bytes(userName);
				std::wstring wcountry = conv.from_bytes(country);
				std::wstring wcity = conv.from_bytes(city);
				std::wstring wintro = conv.from_bytes(intro);*/

				std::wstring wuserName = str_conv::utf8_2_wstring(userName);
				std::wstring wcountry = str_conv::utf8_2_wstring(country);
				std::wstring wcity = str_conv::utf8_2_wstring(city);
				std::wstring wintro = str_conv::utf8_2_wstring(intro);

			//	newUser->WriteStringW()
				newUser->WriteStringW("username", wuserName);
				newUser->WriteStringW("country", wcountry);
				newUser->WriteStringW("city", wcity);
				newUser->WriteStringW("intro", wintro);

				std::wstring country_city = wcountry;
				if(!wcity.empty())
				{
					country_city += L"/";
					country_city += wcity;
				}

				newUser->WriteStringW("country_city", country_city);

		


				int time_int = str_conv::string_2_int(newUser->GetString("regtime"));

				time_t time_timet = static_cast<time_t>(time_int);


				std::wstring country_name = newUser->GetStringW("country");
				std::wstring city_name = newUser->GetStringW("city");


			/*	std::string city = newUser->GetString("city");

				std::string intro = newUser->GetString("intro");
*/
				if(country_name.empty() && city.empty())
				{
					country_name = L"UnknownArea";
					newUser->WriteStringW("country", L"UnknownArea");
					newUser->WriteStringW("country_city", L"UnknownArea/city");
				}
				
				if(intro.empty())
				{
					//		newUser->WriteString("intro", "没有个人简介");

					AAGuiArgs introArg;
					
					introArg.WriteStringW("Value", L"编辑个人简介");
					
					AAGUIEXEC(L"BigAvatarProfile", L"intro", L"Set", introArg);
				}

				newUser->Copy(oneAoeUser);

				if(!picture.empty() && picture != photo_name_jpg)
				{
					download_avatar = true;

					AAGuiArgs AvatarArg;
					AvatarArg.WriteString("url", picture);
					dataExec(0, "obj_document", "download_user_image",
						AvatarArg,ReaderWindow::OnDownloadEnd);
					//下载头像
				}
				else if(!photo_name.empty() && !picture.empty())
				{
					local_avatar = true;

					bool load_ready = LoadLocalUserImageProcedure(photo_namew);

					if(!load_ready)
					{
						if(!picture.empty())
						{
							AAGuiArgs AvatarArg;
							AvatarArg.WriteString("url", picture);
							dataExec(0, "obj_document", "download_user_image",
								AvatarArg,ReaderWindow::OnDownloadEnd);
						}
					}
					//加载本地头像
				}
				else if(picture.empty() && !photo_name.empty())
				{
					local_avatar = true;

					bool load_ready = LoadLocalUserImageProcedure(photo_namew);

					if(!load_ready)
					{
						if(!picture.empty())
						{
							AAGuiArgs AvatarArg;
							AvatarArg.WriteString("url", picture);
							dataExec(0, "obj_document", "download_user_image",
								AvatarArg,ReaderWindow::OnDownloadEnd);
						}
					}
				}
				if(picture.empty() && !local_avatar)
				{
					AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetAvatarClear", AAGuiArgs());
					AAGUIEXEC(L"SystemBar", L"btnAoeUserDropDown", L"SetAvatarClear", AAGuiArgs());
					CheckAvatarProcedure();
					is_set_avatar = false;
				}
				timeStr = str_conv::time_t2str(time_timet);

				AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"LoadJsonData", *newUser);

				AAGUIEXEC(L"SystemBar", L"btnLogin", L"SetVisible", AAGuiArgs("Value", "false"));
				AAGUIEXEC(L"SystemBar", L"btnAutoLogin", L"SetVisible", AAGuiArgs("Value", "false"));
			

				is_user_login = true;
				AAGuiArgs nameArg;
				nameArg.WriteStringW("Value", wuserName);
				AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"Set", nameArg);
				AAGUIEXEC(L"MenuUser", L"MenuEditProfile", L"Enable", AAGuiArgs("Value", "true"));
				AAGUIEXEC(L"MenuUser", L"MenuEditAvatar", L"Enable", AAGuiArgs("Value", "true"));
				CheckAvatarProcedure();	
				/*AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetVisible", AAGuiArgs("Value", "true"));*/
			}
			conf.write_string(L"USER", L"CLIENT", str_conv::str2wstr(ClientInfo));
			AAGUIEXEC(0, L"root", L"popup_layer", AAGuiArgs("command", "anime_destory"));
			AAGUIEXEC(0, L"root", L"Update", AAGuiArgs());
		}
		else
		{
			std::string error_type_ = value.GetString("etype");

			std::string errorString1 = value.GetString("error");

			if(error_type_ == "e0")
			{
				std::string errorString = value.GetString("error");
				GUI_SET(L"user-login", L"loginWorkTab", "1");
				GUI_SET(L"user-login", L"LoginErrorMsg2", errorString);
				GUI_SET(L"user-login", L"TabTipOther", "2");
			}
			else if (error_type_ == "edecode")
			{
				std::string errorString = value.GetString("error");
				GUI_SET(L"user-login", L"loginWorkTab", "1");
				GUI_SET(L"user-login", L"LoginErrorMsg2", errorString);
				GUI_SET(L"user-login", L"TabTipOther", "2");
			}
			else if(error_type_ == "equery")
			{
				std::string errorString = value.GetString("error");
				GUI_SET(L"user-login", L"loginWorkTab", "1");
				GUI_SET(L"user-login", L"LoginErrorMsg2", errorString);
				GUI_SET(L"user-login", L"TabTipOther", "2");
			}
			else if(error_type_ == "epass")
			{

				GUI_SET(L"user-login", L"loginWorkTab", "1");
				GUI_SET(L"user-login", L"TabTipPassEmpty", "3");
			}
			else if(error_type_ == "enouer")
			{
				std::string errorString = value.GetString("error");

				GUI_SET(L"user-login", L"loginWorkTab", "1");
				GUI_SET(L"user-login", L"LoginErrorMsg", errorString);
				GUI_SET(L"user-login", L"TabTipMailEmpty", "4");
			}
			else
			{
				std::string errorString = value.GetString("error");
				GUI_SET(L"user-login", L"loginWorkTab", "1");
				GUI_SET(L"user-login", L"LoginErrorMsg2", errorString);
				GUI_SET(L"user-login", L"TabTipOther", "2");
			}
			if(!errorString1.empty())
			{
				AAGUIEXEC(L"root", L"SignInErrorInfo", L"Set", AAGuiArgs("Value", errorString1));
			}
			AAGUIEXEC(L"SystemBar", L"btnAutoLogin", L"Set", AAGuiArgs("Value", "登录失败"));
			//AAGUIEXEC(L"SystemBar", L"btnAutoLogin", L"SetWidth", AAGuiArgs("Value", "100"));
			AAGUIEXEC(L"root", L"AutoLoginContextTab", L"Set", AAGuiArgs("Value", "2"));
			AAGUIEXEC(L"root", L"RetryAutoLogin", L"Enable", AAGuiArgs("Value", "true"));
			AAGUIEXEC(L"", L"root", L"Update", AAGuiArgs("Value", "true"));
		}
	}
	
	
	//std::string time_string_ = value.GetString("time");

	//GUI_SET(L"TitleFuncBar", L"ClientTime", time_string_);
}
bool ReaderWindow::LoadLocalUserImageProcedure(std::wstring file_name)
{
	AAGuiArgs oneArg;

	dataExecNULL(0, "obj_document", "get_user_image_dir",
		oneArg);

	std::wstring file_path = oneArg.GetStringW("image_dir");

	file_path += file_name;

	std::wstring path_big = file_path;
	std::wstring path_big_jpg = file_path;
	std::wstring path_small60 = file_path;
	std::wstring path_small30 = file_path;
	std::wstring path_small20 = file_path;

	path_small60 += L"60";
	path_small60 += L".png";

	path_small30 += L"30";
	path_small30 += L".png";

	path_small20 += L"20";
	path_small20 += L".png";

	path_big += L".png";
	path_big_jpg += L".jpg";


	bool all_file_ready = false;
	if(::PathFileExistsW(path_small60.c_str()) && PathFileExistsW(path_small20.c_str())
		&& PathFileExistsW(path_big.c_str()))
	{
		all_file_ready = true;
	}

	if(!all_file_ready)
	{
		
		return false;
	}

	is_set_avatar = true;
	AAGuiArgs avatarArgTitle;
	avatarArgTitle.WriteStringW("ValueBig", path_small60);
	avatarArgTitle.WriteStringW("ValueSmall", path_small20);

	if(is_big_avatar)
	{

		avatarArgTitle.WriteStringW("Size", L"Big");
	}
	else
	{
		avatarArgTitle.WriteStringW("Size", L"small");
	}


	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetAvatarValue", avatarArgTitle);
//	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"text_padding", AAGuiArgs("Value", "12,0,0,0"));


	AAGuiArgs avatarArgDropdown;
	avatarArgDropdown.WriteStringW("ValueBig", path_big);
	avatarArgDropdown.WriteStringW("ValueSmall", path_small20);
	avatarArgDropdown.WriteStringW("Size", L"Big");


	AAGUIEXEC(L"SystemBar", L"btnAoeUserDropDown", L"SetPadding", AAGuiArgs("Value", "10,10,0,0"));

	AAGUIEXEC(L"SystemBar", L"btnAoeUserDropDown", L"SetAvatarValue", avatarArgDropdown);

	CheckAvatarProcedure();

	return true;

}