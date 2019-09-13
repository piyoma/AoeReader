#include "ReaderWindow.h"
#include "AppConfig.h"

void ReaderWindow::OnDownloadEnd(const dataObj *sender, const AAGuiArgs& value)
{

	std::wstring file_path = value.GetStringW("file");
	
	if(widget_window_ && !file_path.empty())
	{
		download_image = file_path;
		widget_window_->PostTask(AAGuiNewTask(&ReaderWindow::uiTaskGenerateImage, this));
	}
}
void ReaderWindow::uiTaskGenerateImage(AAGuiArgs& e)
{

	AAGuiArgs arg;

	arg.WriteStringW("path", download_image);
	AAGUIEXEC(NULL, L"root", L"ConvertImage", arg);


	if(oneAoeUser)
	{

		std::wstring save_path = L"";
		int dot_pos = download_image.rfind(L".");

		if(dot_pos != std::wstring::npos)
		{
			save_path = download_image.substr(0, dot_pos);

		}

		if(save_path.empty())
		{
			return;
		}
		std::string picture = oneAoeUser->GetString("picture");
		std::wstring photo_ = str_conv::str2wstr(picture);


		int dot_pos2 = photo_.rfind(L".");
		std::wstring photo_filename = L"";
		if(dot_pos2 != std::wstring::npos)
		{
			photo_filename = photo_.substr(0, dot_pos2);

		}

		if(photo_filename.empty())
		{
			return;
		}

		std::wstring path_big = save_path;
		std::wstring path_small20 = save_path;


		path_small20 += L"20";
		path_small20 += L".png";

		path_big += L".png";

	
		
		

		ModuleConfig conf;
		conf.write_string(L"USER", L"PHOTOBIG", path_big);
		conf.write_string(L"USER", L"PHOTOSMALL", path_small20);
		conf.write_string(L"USER", L"PHOTONAME", photo_filename);

		LoadLocalUserImageProcedure(photo_filename);
	}
	


}