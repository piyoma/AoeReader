#ifndef __reader_window_h__
#define __reader_window_h__

#include "stdafx.h"
#include "unicode_str.h"

class ReaderWindow
	: public AAGuiClient
{
public:

	virtual LPCTSTR GetDocument()
	{return L"ReaderViewer.xml";}

	ReaderWindow();~ReaderWindow();
	virtual void OnNativeWidgetCreated(HWND windowm, AAWidgetWindow* widget_window);
	virtual void OnNewFrameCreated(LPCTSTR  frame_id); 
	GUI_DECL_HANDLER(OnPreviewFrameEvent);  
	GUI_DECL_HANDLER(OnToolbarEvent); 
	GUI_DECL_HANDLER(OnBookMarkEvent);  
	GUI_DECL_HANDLER(OnTitleFuncBarEvent);  
	GUI_DECL_HANDLER(OnNativeClose);  
	GUI_DECL_HANDLER(OnExit);
	GUI_DECL_HANDLER(OnReaderOptionBar);
	GUI_DECL_HANDLER(OnOptionPopupEvent);
	GUI_DECL_HANDLER(OnPageOptionEvent);
	GUI_DECL_HANDLER(OnTextOptionEvent);
	GUI_DECL_HANDLER(OnLoginEvent);
	GUI_DECL_HANDLER(OnUserEvent);
	GUI_DECL_HANDLER(OnMainOption);
	GUI_DECL_HANDLER(OnEditUserProfile);
	GUI_DECL_HANDLER(OnHistoryEvent);
	GUI_DECL_HANDLER(OnMainMenuEvent);
	GUI_DECL_HANDLER(OnVersionEvent);
	DATA_DECL(OnParserDocumentEnd);  
	DATA_DECL(OnTimeUpdate);  
	DATA_DECL(OnWebData);  
	DATA_DECL(OnDownloadEnd);  
	DATA_DECL(OnHistoryData);  

	void LoadReaderViewFont();
	void LoadReaderPageParam();
	void LoadTextColor();
	void ApplyTextColor();
	void LoadTextColorOption();
	void CheckLoginInfo();
	void CheckAvatarProcedure();
	void WriteNowProfile();
	void WriteNowSelfIntro();
	void WriteMainOption();
	void QueryHistory();
	void OnExitFrame();
	void QueryHistoryProcedure(std::string page, bool alldata = false);
	bool LoadLocalUserImageProcedure(std::wstring file_name);
	bool MenuProcudure(std::string file_name);

	void CheckBigAvatar();
	void CheckSmallAvatar();
	void CheckSideProcedure();
	GUI_TASK(uiTaskGenerateImage);
	GUI_TASK(uiTaskRefreshHistory);

	void InitOpenFile(std::wstring file_);

private:

	AAWidgetWindow* widget_window_;
	AAGuiArgs* oneAoeUser;
	std::wstring last_image_path;
	std::wstring ClientInfoMsg;
	std::wstring download_image;
	std::wstring init_open_file;
	std::wstring last_key_word;
	std::wstring open_file_name;
	int history_current_page;
	HICON m_hIcon;
	int login_session_count;
	int history_total_count;
	bool is_user_login;
	bool has_open_file;
	bool is_big_avatar;
	bool is_set_avatar;
	bool enabled_selfintro;
	bool enabled_profile;
};

#endif