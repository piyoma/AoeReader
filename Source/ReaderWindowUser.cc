#include "ReaderWindow.h"
#include "AppConfig.h"


void  ReaderWindow::OnUserEvent(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();
	/*L"ClientGetArgs"*/
	if(element_id == "btnChangeAvatar" || element_id == "btnAoeUserDropDown")
	{
	
		AAGUIEXEC(NULL, L"root", L"HideContextLayer", AAGuiArgs());

		AAGuiArgs arg("document", "viewclient\\uploadavatar.xml");
		arg.WriteString("command", "create");
		widget_window_->ExcuteCommand(0, L"root", L"popup_layer", arg);

	}
	else if(element_id == "btnApplyAvatar")
	{
		is_set_avatar = true;

		AAGuiArgs getUser;
		getUser.WriteString("key", "uid");
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"ClientGetArgs", getUser);
		
		std::string uid_ = getUser.GetString("uid");

		std::string user = "user";

		user += uid_;

		AAGuiArgs oneArg;

		oneArg.WriteStringW("user_id", str_conv::str2wstr(user));

		dataExecNULL(0, "obj_document", "generate_user_image_path",
			oneArg);

		std::wstring file_path = oneArg.GetStringW("file_path");
		std::wstring photo_name = oneArg.GetStringW("file_name");


		AAGuiArgs bkgArg;
		bkgArg.WriteStringW("Value", file_path);
		AAGUIEXEC(L"upload-avatar", L"AvatarSelect", L"SaveToFile", bkgArg);

		ModuleConfig conf;
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

		conf.write_string(L"USER", L"PHOTOBIG", path_big);
		conf.write_string(L"USER", L"PHOTOSMALL", path_small20);
		conf.write_string(L"USER", L"PHOTONAME", photo_name);

		getUser.WriteString("key", "autoken");
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"ClientGetArgs", getUser);
		std::string autoken = getUser.GetString("autoken");

		getUser.WriteString("key", "email");
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"ClientGetArgs", getUser);
		std::string email = getUser.GetString("email");

		AAGuiArgs uploadArg;

		uploadArg.WriteStringW("file", path_big_jpg);
		uploadArg.WriteString("autoken", autoken);
		uploadArg.WriteString("mail", email);
		dataExecNULL(0, "obj_document", "upload_photo",
			uploadArg);

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
		//AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"text_padding", AAGuiArgs("Value", "12,0,0,0"));

		AAGuiArgs avatarArgDropdown;
		avatarArgDropdown.WriteStringW("ValueBig", path_big);
		avatarArgDropdown.WriteStringW("ValueSmall", path_small20);
		avatarArgDropdown.WriteStringW("Size", L"Big");

		//check title image

		//if(is_set_avatar)
		//{
		//	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetEmpty", AAGuiArgs());
		//	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetWidth", AAGuiArgs("Value", "65"));
		//	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetHeight", AAGuiArgs("Value", "65"));
		//	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetPadding", AAGuiArgs("Value", "20,8,0,0"));
		//}
		//else
		//{
		//	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetEmpty", AAGuiArgs());
		//	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetWidth", AAGuiArgs("Value", "70"));
		//	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetHeight", AAGuiArgs("Value", "70"));
		//	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetPadding", AAGuiArgs("Value", "20,0,0,0"));
		//}

		////check drop down
		//if(is_big_avatar)
		//{
		//	if(is_set_avatar)
		//	{
		//		AAGUIEXEC(L"root", L"UserInfoContext", L"SetBounds", AAGuiArgs("Value", "-10,60,330,305"));
		//	}
		//	else
		//	{
		//		AAGUIEXEC(L"root", L"UserInfoContext", L"SetBounds", AAGuiArgs("Value", "-10,70,330,305"));
		//	}
		//	AAGUIEXEC(L"root", L"UserInfoContextShadow", L"SetArrowOffset", AAGuiArgs("Value", "30"));
		//}
		//else
		//{
		//	AAGUIEXEC(L"root", L"UserInfoContext", L"SetBounds", AAGuiArgs("Value", "-202,20,330,305"));
		//	AAGUIEXEC(L"root", L"UserInfoContextShadow", L"SetArrowOffset", AAGuiArgs("Value", "228"));
		//}

		AAGUIEXEC(L"SystemBar", L"btnAoeUserDropDown", L"SetPadding", AAGuiArgs("Value", "10,10,0,0"));

		AAGUIEXEC(L"SystemBar", L"btnAoeUserDropDown", L"SetAvatarValue", avatarArgDropdown);

		CheckAvatarProcedure();
		AAGUIEXEC(0, L"root", L"popup_layer", AAGuiArgs("command", "anime_destory"));

		

	}
	else if(element_id == "btnSelectAvatarImage")
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

			GUI_SET(L"upload-avatar", L"AvatarTab", "2");

			AAGuiArgs bkgArg;
			bkgArg.WriteStringW("Value", sel_path_result_w);
			AAGUIEXEC(L"upload-avatar", L"AvatarSelect", L"Set", bkgArg);
			AAGUIEXEC(L"upload-avatar", L"btnApplyAvatar", L"Enable", AAGuiArgs("Value", "true") );
		}

	

	}
	else if(element_id == "checkBIGAVATAR")
	{
		std::string isBigAvatar = e.GetString("Value");

	

		if(isBigAvatar == "true")
		{
			CheckBigAvatar();
		}
		else
		{
			CheckSmallAvatar();
		}
	}
	else if(element_id == "SignOut")
	{
		AAGUIEXEC(NULL, L"root", L"HideContextLayer", AAGuiArgs());
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"SystemBar", L"btnLogin", L"SetVisible", AAGuiArgs("Value", "true"));
		AAGUIEXEC(L"root", L"SystemBar", L"SetHeight", AAGuiArgs("Value", "35"));
		AAGUIEXEC(L"root", L"MainToolBar", L"SetVisible", AAGuiArgs("Value", "true"));
		AAGUIEXEC(L"root", L"BigAvatarProfile", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"MenuUser", L"MenuEditProfile", L"Enable", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"MenuUser", L"MenuEditAvatar", L"Enable", AAGuiArgs("Value", "false"));
		is_user_login = false;
		CheckAvatarProcedure();
	}
	else if(element_id == "RetryAutoLogin")
	{
	
		AAGuiArgs loginArg;

		loginArg.WriteStringW("ClientInfo", ClientInfoMsg);
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

				AAGUIEXEC(L"SystemBar", L"btnAutoLogin", L"Set", AAGuiArgs("Value", "正在登录.."));
				InvalidateRect(widget_window_->GetNativeWindow(), NULL, false);
				loginArg.WriteStringW("mail", str_conv::str2wstr(user_mail));
				loginArg.WriteStringW("pass", str_conv::str2wstr(user_password));
				login_session_count++;
				loginArg.WriteString("login_session_count", str_conv::int_2_string(login_session_count));
				AAGUIEXEC(L"root", L"AutoLoginContextTab", L"Set", AAGuiArgs("Value", "1"));
				AAGUIEXEC(L"root", L"RetryAutoLogin", L"Enable", AAGuiArgs("Value", "false"));
				dataExec(0, "obj_document", "user_login",
					loginArg, ReaderWindow::OnWebData);

			}
		}
	}
	else if(element_id == "CancelAutoLogin")
	{
		AAGUIEXEC(L"SystemBar", L"btnAutoLogin", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"SystemBar", L"btnLogin", L"SetVisible", AAGuiArgs("Value", "true"));
	}
	else if(element_id == "EditUserProfile")
	{
		AAGUIEXEC(NULL, L"root", L"HideContextLayer", AAGuiArgs());

		AAGuiArgs arg("document", "viewclient\\editprofile.xml");
		arg.WriteString("command", "create");
		widget_window_->ExcuteCommand(0, L"root", L"popup_layer", arg);
	}
	else if(element_id == "btnEditUserIntro")
	{
		if(oneAoeUser)
		{
			std::wstring intro = oneAoeUser->GetStringW("intro");
			AAGuiArgs arg;
			arg.WriteStringW("Value",intro);
			AAGUIEXEC(L"SystemBar", L"LayoutUserIntro", L"EditHoverNow", arg);
		}
	}
	else if(element_id == "LayoutUserIntro" && event_name_ == "LayoutEditEnd")
	{
		std::wstring newIntro = e.GetStringW("text");

		if(oneAoeUser)
		{
			if(newIntro.length() > 128)
			{
				newIntro = newIntro.substr(0, 128);
			}

			std::wstring intro = oneAoeUser->GetStringW("intro");


			if(intro != newIntro)
			{
				AAGuiArgs introArg;

				oneAoeUser->WriteStringW("intro", newIntro);

			
				introArg.WriteStringW("Value", newIntro);

					if(newIntro.empty())
					introArg.WriteStringW("Value", L"编辑个人简介");
				AAGUIEXEC(L"BigAvatarProfile", L"intro", L"Set", introArg);
				AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"LoadJsonData", *oneAoeUser);

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
						editProfileArg.WriteStringW("userintro", newIntro);

						dataExec(0, "obj_document", "user_edit_intro",
							editProfileArg, ReaderWindow::OnWebData);
					}
					
				}

		
			}
		}

	}
	else if(element_id == "btnMainAdd")
	{
		OnToolbarEvent(sender, e);
	}
	else if(element_id == "BtnClose")
	{

		if(has_open_file)
		{
			ModuleConfig conf;

			std::wstring closeQuery = conf.query_string(L"USER", L"CLOSEQUERY");

			if(closeQuery != L"false")
			{
				AAGUIEXEC(NULL, L"root", L"HideContextLayer", AAGuiArgs());

				AAGuiArgs arg("document", "message_box\\logout.xml");
				arg.WriteString("command", "create");
				widget_window_->ExcuteCommand(0, L"root", L"popup_layer", arg);
				return;
			}

			if(widget_window_)
			{
				HWND window_handle_ = widget_window_->GetNativeWindow();

				RECT window_rect;
				GetWindowRect(window_handle_, &window_rect);

				AppConfigure::Instance()->WritePosValue(window_rect.left, window_rect.right, 
					window_rect.top, window_rect.bottom);
			}

			
				
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

	OnMainMenuEvent(sender, e);
}
void ReaderWindow::CheckAvatarProcedure()
{
	
	if(!is_user_login)
	{
		AAGUIEXEC(L"SystemBar", L"SamllSysbar", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"TitleFuncBar", L"SamllSysbar2", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"root", L"file_name", L"SetPadding", AAGuiArgs("Value", "90,0,0,0"));
		return;
	}
	if(is_big_avatar)
	{
		
	
		if(is_set_avatar)
		{
			AAGUIEXEC(L"root", L"UserInfoContext", L"SetBounds", AAGuiArgs("Value", "-10,60,330,305"));
		}
		else
		{
			AAGUIEXEC(L"root", L"UserInfoContext", L"SetBounds", AAGuiArgs("Value", "-10,70,330,305"));
		}
		AAGUIEXEC(L"root", L"UserInfoContextShadow", L"SetArrowOffset", AAGuiArgs("Value", "30"));


		if(is_set_avatar)
		{
			AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetEmpty", AAGuiArgs());
			AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetWidth", AAGuiArgs("Value", "65"));
			AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetHeight", AAGuiArgs("Value", "65"));
			AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetPadding", AAGuiArgs("Value", "20,8,0,0"));
		}
		else
		{
			AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetEmpty", AAGuiArgs());
			AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetWidth", AAGuiArgs("Value", "70"));
			AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetHeight", AAGuiArgs("Value", "70"));
			AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetPadding", AAGuiArgs("Value", "20,0,0,0"));
		}

		if(!has_open_file)
		{
			AAGUIEXEC(L"root", L"SystemBar", L"SetHeight", AAGuiArgs("Value", "80"));
		}
		else
		{
			AAGUIEXEC(L"root", L"SystemBar", L"SetHeight", AAGuiArgs("Value", "50"));
		}

		if(oneAoeUser)
		{
			std::wstring userName = oneAoeUser->GetStringW("username");
			std::wstring userIntro = oneAoeUser->GetStringW("intro");
			AAGuiArgs nameArg;
			nameArg.WriteStringW("Value", userName);
			AAGUIEXEC(L"BigAvatarProfile", L"username", L"Set", nameArg);

			AAGuiArgs introArg;
			introArg.WriteStringW("Value", userIntro);
			if(userIntro.empty())
			{
				introArg.WriteStringW("Value", L"编辑个人简介");
			}
			AAGUIEXEC(L"BigAvatarProfile", L"intro", L"Set", introArg);
		}
		AAGuiArgs sizeTileAvatar;

		sizeTileAvatar.WriteStringW("Size", L"Big");
		sizeTileAvatar.WriteString("size_value", "55");
		sizeTileAvatar.WriteString("font_size", "11");
		sizeTileAvatar.WriteString("circle", "true");
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetAvatarSize", sizeTileAvatar);

		AAGUIEXEC(L"root", L"MainToolBar", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"toFisrt", AAGuiArgs());

		//显示顺序
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetVisible", AAGuiArgs("Value", "true"));

		AAGUIEXEC(L"root", L"BigAvatarProfile", L"SetVisible", AAGuiArgs("Value", "true"));
		AAGUIEXEC(L"UserInfoContext", L"checkBIGAVATAR", L"Set", AAGuiArgs("Value", "true"));
		AAGUIEXEC(L"root", L"file_name", L"SetPadding", AAGuiArgs("Value", "-100,0,0,0"));

		
		if(has_open_file)
		{
			AAGUIEXEC(L"TitleFuncBar", L"SamllSysbar2", L"SetVisible", AAGuiArgs("Value", "true"));
			AAGUIEXEC(L"SystemBar", L"SamllSysbar", L"SetVisible", AAGuiArgs("Value", "false"));
		}
		else
		{
			AAGUIEXEC(L"SystemBar", L"SamllSysbar", L"SetPadding", AAGuiArgs("Value", "0,20,0,0"));
			AAGUIEXEC(L"SystemBar", L"SamllSysbar", L"SetVisible", AAGuiArgs("Value", "true"));
			AAGUIEXEC(L"SystemBar", L"SamllSysbar", L"Update", AAGuiArgs("Value", "true"));
		}
		
	}
	else
	{
		AAGUIEXEC(L"SystemBar", L"SamllSysbar", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"TitleFuncBar", L"SamllSysbar2", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"root", L"file_name", L"SetPadding", AAGuiArgs("Value", "90,0,0,0"));

		AAGUIEXEC(L"root", L"BigAvatarProfile", L"SetVisible", AAGuiArgs("Value", "false"));
		AAGUIEXEC(L"root", L"MainToolBar", L"SetVisible", AAGuiArgs("Value", "true"));
		AAGUIEXEC(L"root", L"UserInfoContext", L"SetBounds", AAGuiArgs("Value", "-202,20,330,305"));
		AAGUIEXEC(L"root", L"UserInfoContextShadow", L"SetArrowOffset", AAGuiArgs("Value", "228"));
	
		if(!has_open_file)
		{
			AAGUIEXEC(L"root", L"SystemBar", L"SetHeight", AAGuiArgs("Value", "35"));
		}
		else
		{
			AAGUIEXEC(L"root", L"SystemBar", L"SetHeight", AAGuiArgs("Value", "35"));
		}

		if(oneAoeUser)
		{

			if(is_set_avatar)
			{
				AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetHeight", AAGuiArgs("Value", "30"));
			}
			else
			{
				AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetHeight", AAGuiArgs("Value", "27"));
			}

			std::wstring userName = oneAoeUser->GetStringW("username");
			std::wstring userIntro = oneAoeUser->GetStringW("intro");
			AAGuiArgs nameArg;
			nameArg.WriteStringW("Value", userName);
			AAGUIEXEC(L"BigAvatarProfile", L"username", L"Set", nameArg);

			AAGuiArgs introArg;
			
			introArg.WriteStringW("Value", userIntro);

			if(userIntro.empty())
			{
				introArg.WriteStringW("Value", L"编辑个人简介");
			}
			AAGUIEXEC(L"BigAvatarProfile", L"intro", L"Set", introArg);
			//头像顺序
			AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetVisible", AAGuiArgs("Value", "true"));
		}
	}
}
void ReaderWindow::CheckSmallAvatar()
{
	/*if(!is_big_avatar)
		return;*/
	AAGUIEXEC(L"SystemBar", L"SamllSysbar", L"SetVisible", AAGuiArgs("Value", "false"));
	AAGUIEXEC(L"TitleFuncBar", L"SamllSysbar2", L"SetVisible", AAGuiArgs("Value", "false"));

	ModuleConfig conf;

	conf.write_string(L"USER", L"VIEWBIG", L"false");
	is_big_avatar = false;

	AAGuiArgs getUser;
	getUser.WriteString("key", "username");
	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"ClientGetArgs", getUser);



	AAGuiArgs sizeTileAvatar;

	sizeTileAvatar.WriteStringW("Size", L"Small");
	sizeTileAvatar.WriteString("size_value", "13");
	sizeTileAvatar.WriteString("font_size", "9");
	sizeTileAvatar.WriteString("circle", "true");
	AAGUIEXEC(L"root", L"file_name", L"SetPadding", AAGuiArgs("Value", "90,0,0,0"));
	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetAvatarSize", sizeTileAvatar);

	if(oneAoeUser)
	{
		std::wstring userName = oneAoeUser->GetStringW("username");
		AAGuiArgs nameArg;
		nameArg.WriteStringW("Value", userName);
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"Set", nameArg);
	}


	if(is_set_avatar)
	{
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetHeight", AAGuiArgs("Value", "30"));
	}
	else
	{
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetHeight", AAGuiArgs("Value", "27"));
	}
	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetPadding", AAGuiArgs("Value", "-16,2,0,0"));
	AAGUIEXEC(L"root", L"UserInfoContext", L"SetBounds", AAGuiArgs("Value", "-202,20,330,305"));
	AAGUIEXEC(L"root", L"UserInfoContextShadow", L"SetArrowOffset", AAGuiArgs("Value", "228"));
	AAGUIEXEC(L"root", L"MainToolBar", L"SetVisible", AAGuiArgs("Value", "true"));
	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"toBefore", AAGuiArgs("Value", "2"));
	AAGUIEXEC(L"root", L"BigAvatarProfile", L"SetVisible", AAGuiArgs("Value", "false"));

	if(!has_open_file)
	{
		AAGUIEXEC(L"root", L"SystemBar", L"SetHeight", AAGuiArgs("Value", "35"));
	}
	else
	{
		AAGUIEXEC(L"root", L"SystemBar", L"SetHeight", AAGuiArgs("Value", "35"));
	}
}
void ReaderWindow::CheckBigAvatar()
{
	/*if(is_big_avatar)
		return;*/
	ModuleConfig conf;

	conf.write_string(L"USER", L"VIEWBIG", L"true");
	is_big_avatar = true;
	AAGuiArgs sizeTileAvatar;

	sizeTileAvatar.WriteStringW("Size", L"Big");
	sizeTileAvatar.WriteString("size_value", "55");
	sizeTileAvatar.WriteString("font_size", "11");
	sizeTileAvatar.WriteString("circle", "true");

	if(oneAoeUser)
	{
		std::wstring userName = oneAoeUser->GetStringW("username");
		std::wstring userIntro = oneAoeUser->GetStringW("intro");
		AAGuiArgs nameArg;
		nameArg.WriteStringW("Value", userName);
		AAGUIEXEC(L"BigAvatarProfile", L"username", L"Set", nameArg);

		AAGuiArgs introArg;
		introArg.WriteStringW("Value", userIntro);
		if(userIntro.empty())
		{
			introArg.WriteStringW("Value", L"编辑个人简介");
		}
		AAGUIEXEC(L"BigAvatarProfile", L"intro", L"Set", introArg);
	}

	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetAvatarSize", sizeTileAvatar);

	if(is_set_avatar)
	{
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetEmpty", AAGuiArgs());
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetWidth", AAGuiArgs("Value", "65"));
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetHeight", AAGuiArgs("Value", "65"));
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetPadding", AAGuiArgs("Value", "20,8,0,0"));
	}
	else
	{
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetEmpty", AAGuiArgs());
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetWidth", AAGuiArgs("Value", "70"));
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetHeight", AAGuiArgs("Value", "70"));
		AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"SetPadding", AAGuiArgs("Value", "20,0,0,0"));
	}
	if(is_big_avatar)
	{
		if(is_set_avatar)
		{
			AAGUIEXEC(L"root", L"UserInfoContext", L"SetBounds", AAGuiArgs("Value", "-10,60,330,305"));
		}
		else
		{
			AAGUIEXEC(L"root", L"UserInfoContext", L"SetBounds", AAGuiArgs("Value", "-10,70,330,305"));
		}
		AAGUIEXEC(L"root", L"UserInfoContextShadow", L"SetArrowOffset", AAGuiArgs("Value", "30"));
	}
	else
	{
		AAGUIEXEC(L"root", L"UserInfoContext", L"SetBounds", AAGuiArgs("Value", "-202,20,330,305"));
		AAGUIEXEC(L"root", L"UserInfoContextShadow", L"SetArrowOffset", AAGuiArgs("Value", "228"));
	}

	AAGUIEXEC(L"root", L"MainToolBar", L"SetVisible", AAGuiArgs("Value", "false"));
	AAGUIEXEC(L"SystemBar", L"btnAoeUser", L"toFisrt", AAGuiArgs());
	AAGUIEXEC(L"root", L"BigAvatarProfile", L"SetVisible", AAGuiArgs("Value", "true"));

	AAGUIEXEC(L"root", L"file_name", L"SetPadding", AAGuiArgs("Value", "-100,0,0,0"));

	if(!has_open_file)
	{
		AAGUIEXEC(L"root", L"SystemBar", L"SetHeight", AAGuiArgs("Value", "80"));
	}
	else
	{
		AAGUIEXEC(L"root", L"SystemBar", L"SetHeight", AAGuiArgs("Value", "50"));
	}

	if(has_open_file)
	{
		AAGUIEXEC(L"TitleFuncBar", L"SamllSysbar2", L"SetVisible", AAGuiArgs("Value", "true"));
		AAGUIEXEC(L"SystemBar", L"SamllSysbar", L"SetVisible", AAGuiArgs("Value", "false"));
	}
	else
	{
		AAGUIEXEC(L"SystemBar", L"SamllSysbar", L"SetPadding", AAGuiArgs("Value", "0,20,0,0"));
		AAGUIEXEC(L"SystemBar", L"SamllSysbar", L"SetVisible", AAGuiArgs("Value", "true"));
		AAGUIEXEC(L"SystemBar", L"SamllSysbar", L"Update", AAGuiArgs("Value", "true"));
	}
}