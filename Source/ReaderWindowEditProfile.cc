#include "ReaderWindow.h"
#include "AppConfig.h"
#include <codecvt>

void  ReaderWindow::OnEditUserProfile(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();
	/*L"ClientGetArgs"*/
	bool enable_save = false;

	if(element_id == "editUserIntro" || element_id == "editUserName"
		|| element_id == "ComboRegion" || element_id == "ComboCity"
		|| element_id == "ComboTimeZone" || element_id == "ComboMale")
	{
		enable_save = true;
	}
	else if(element_id == "time_birthday" && event_name_ == "TimeSelect")
	{
		enable_save = true;
	}
	else if(element_id == "btnApplyUserProfile")
	{
		///////
		std::wstring user_name = GUI_GETW(L"edit-user-profile", L"editUserName");

		if(user_name.empty())
		{
			GUI_SET(L"edit-user-profile", L"ProfileTipTab", "2");
			return;
		}
		/////////
		std::wstring user_intro = GUI_GETW(L"edit-user-profile", L"editUserIntro");

		AAGuiArgs checkLimt;
		AAGUIEXEC(L"edit-user-profile", L"editUserIntro", L"CheckLimit", checkLimt);
		if(checkLimt.GetString("Value") != "true")
		{
			GUI_SET(L"edit-user-profile", L"ProfileTipTab2", "2");
			return;
		}
		AAGuiArgs getCountryArg;
		AAGUIEXEC(L"edit-user-profile", L"ComboRegion", L"Get", getCountryArg);

		//////////
		std::wstring country = getCountryArg.GetStringW("ValueW");
		std::string countrycode = getCountryArg.GetString("countrycode");
		//std::codecvt_utf8 

		//std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	
		AAGuiArgs getCityArg;
		AAGUIEXEC(L"edit-user-profile", L"ComboCity", L"Get", getCityArg);
		////////////
		std::wstring city = getCityArg.GetStringW("ValueW"); 
		///////
		std::string timezone = GUI_GET(L"edit-user-profile", L"ComboTimeZone");
		std::string male = GUI_GET(L"edit-user-profile", L"ComboMale");
		std::string birthday = GUI_GET(L"edit-user-profile", L"time_birthday");

		if(male == "男")
		{
			male = "0";
		}
		else
		{
			male = "1";
		}
		/*std::string uft8Contry = conv.to_bytes(country);
		std::string utf8City = conv.to_bytes(city);*/

		//std::wstring wideStr = conv.from_bytes(uft8Contry);

		ModuleConfig conf;

		std::wstring ClientInfo = conf.query_string(L"USER", L"CLIENT");

		ClientInfoMsg = ClientInfo;;

		AAGuiArgs loginArg;

		loginArg.WriteStringW("ClientInfo", ClientInfo);
		dataExecNULL(0, "obj_document", "get_client_info",
			loginArg);

		std::string succeed = loginArg.GetString("succeed");

		if(succeed == "true"){
			AAGuiArgs* UserValue = loginArg.GetValue("AoeUser");
			if(UserValue)
			{
				std::string user_mail = UserValue->GetString("mail");
				std::string user_password = UserValue->GetString("pass");

				AAGuiArgs editProfileArg;

				editProfileArg.WriteStringW("mail", str_conv::str2wstr(user_mail));
				editProfileArg.WriteStringW("pass", str_conv::str2wstr(user_password));

				editProfileArg.WriteStringW("country", country);
				editProfileArg.WriteStringW("city", city);
				editProfileArg.WriteStringW("username", user_name);
				editProfileArg.WriteStringW("userintro", user_intro);
				editProfileArg.WriteString("timezone", timezone);
				editProfileArg.WriteString("male", male);
				editProfileArg.WriteString("birthday", birthday);
				editProfileArg.WriteString("countrycode", countrycode);
				
				dataExec(0, "obj_document", "user_edit_profile",
					editProfileArg, ReaderWindow::OnWebData);
			}

			if(oneAoeUser)
			{
				oneAoeUser->WriteStringW("username", user_name);
				oneAoeUser->WriteStringW("country", country);
				oneAoeUser->WriteStringW("city", city);
				oneAoeUser->WriteStringW("intro", user_intro);

				oneAoeUser->WriteString("male", male);
				oneAoeUser->WriteString("birthday", birthday);
				oneAoeUser->WriteString("birthdaystr", birthday);
				oneAoeUser->WriteString("timezone", timezone);
				oneAoeUser->WriteString("countrycode", countrycode);

				std::wstring country_city = country;
				if(!city.empty())
				{
					country_city += L"/";
					country_city += city;
				}

				oneAoeUser->WriteStringW("country_city", country_city);

				AAGuiArgs nameArg;
				nameArg.WriteStringW("Value", user_name);
				AAGUIEXEC(L"BigAvatarProfile", L"username", L"Set", nameArg);

				if(!is_big_avatar)
				AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"Set", nameArg);

				AAGuiArgs introArg;
				introArg.WriteStringW("Value", user_intro);
				AAGUIEXEC(L"BigAvatarProfile", L"intro", L"Set", introArg);

			
				AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"LoadJsonData", *oneAoeUser);
			}


		}
	
		AAGUIEXEC(0, L"root", L"popup_layer", AAGuiArgs("command", "anime_destory"));
	}
	if(!enabled_profile && enable_save )
	{
		AAGUIEXEC(L"edit-user-profile", L"btnApplyUserProfile", L"Enable", AAGuiArgs("Value", "true") );
		enabled_profile = true;
	}


	if(!enabled_selfintro && element_id == "editUserIntro2")
	{
		AAGUIEXEC(L"edit-selfintro", L"SaveUserIntro", L"Enable", AAGuiArgs("Value", "true") );
		enabled_selfintro = true;
	}
}
void ReaderWindow::WriteNowProfile()
{
	
	if(oneAoeUser)
	{
		enabled_profile = false;
		std::wstring user_name = oneAoeUser->GetStringW("username");
		std::wstring country_name = oneAoeUser->GetStringW("country");

		std::wstring city = oneAoeUser->GetStringW("city");

		std::wstring intro = oneAoeUser->GetStringW("intro");
		std::string male = oneAoeUser->GetString("male");

		std::string birthday = oneAoeUser->GetString("birthday");

		std::string birthdaystr = oneAoeUser->GetString("birthdaystr");
		std::string timezone = oneAoeUser->GetString("timezone");

		std::string country_code = oneAoeUser->GetString("countrycode");

		if(birthday == "0")
		{
			birthday = oneAoeUser->GetString("regtime");

		}
		if(male == "0")
		{
			GUI_SET(L"edit-user-profile", L"ComboMale", "男");
		}
		else
		{
			GUI_SET(L"edit-user-profile", L"ComboMale", "女");
		}

		GUI_SET(L"edit-user-profile", L"ComboTimeZone", timezone);

		
		if(!country_code.empty())
		{
			AAGuiArgs countryCode;
			countryCode.WriteString("Value", country_code);
			AAGUIEXEC(L"edit-user-profile", L"ComboRegion", L"SetCountryCode", countryCode);
		}
		if(!city.empty())
		{
			GUI_SETW(L"edit-user-profile", L"ComboCity", city);
		}

		if(birthdaystr.empty())
		{	
			__int64 time_birth = str_conv::string_2_int(birthday);

			std::string time_str = str_conv::int_2_timestr(time_birth);

			GUI_SET(L"edit-user-profile", L"time_birthday", time_str)

		}
		else
		{

			GUI_SET(L"edit-user-profile", L"time_birthday", birthdaystr)
		}

		GUI_SETW(L"edit-user-profile", L"editUserName", user_name);

		if(!intro.empty())
		{
			GUI_SETW(L"edit-user-profile", L"editUserIntro", intro);
			
		}
	
		//std::string user_name = oneAoeUser->GetString("username");
	}
}
void ReaderWindow::WriteNowSelfIntro()
{
	if(oneAoeUser)
	{
		enabled_selfintro = false;
		std::wstring user_name = oneAoeUser->GetStringW("username");
		std::wstring country_name = oneAoeUser->GetStringW("country");

		std::wstring city = oneAoeUser->GetStringW("city");

		std::wstring intro = oneAoeUser->GetStringW("intro");
		std::string male = oneAoeUser->GetString("male");

		std::string birthday = oneAoeUser->GetString("birthday");
		std::string timezone = oneAoeUser->GetString("timezone");

		std::string country_code = oneAoeUser->GetString("countrycode");


		if(!intro.empty())
		{
	//		GUI_SETW(L"edit-selfintro", L"user_name", user_name);
			GUI_SETW(L"edit-selfintro", L"editUserIntro2", intro);
	//		AAGUIEXEC(L"edit-selfintro", L"editUserIntro2", L"SetFocus", AAGuiArgs() );
		}
		
	}
}