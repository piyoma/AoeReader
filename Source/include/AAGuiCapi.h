#ifndef __aa_web_ctrl_widget_h__
#define __aa_web_ctrl_widget_h__

#pragma once
#include <windows.h>
#include "AAGuiEventHandler.h"

#if defined(AA_WIDGET_CTRL_BUILD)
#define AA_WIDGET_CTRL_EXPORT __declspec(dllexport)
#else
#define AA_WIDGET_CTRL_EXPORT __declspec(dllimport)
#pragma comment(lib, "AsuraGui.lib")
#endif


class AAGuiClient;
class AAWidgetWindow{
public:
	virtual HWND GetNativeWindow() = 0;
	virtual void CloseWindow(bool force_close) =0;
	virtual void SetVisible(bool bShow) = 0;
	virtual void SetPos(int x,int y,int w,int h) = 0;
	//UI元素控制与获取参数
	virtual void ExcuteCommand(LPCTSTR  frame_id, LPCTSTR  element_id, LPCTSTR func_name, AAGuiArgs& args) = 0; 
	//绑定UI事件回调
	virtual void BindEventHandler(LPCTSTR  frame_id, LPCTSTR  element_id, LPCTSTR event_name, AAGuiEventHandler* handler) = 0; 
	//在UI线程投递一个任务
	virtual void PostTask(AAGuiTask* task) = 0;
	//判断是非在UI线程执行
	virtual bool IsCurrentOnUIThread() = 0;
	virtual int MessageBox(LPCTSTR document) = 0;

};

extern "C" {
	AA_WIDGET_CTRL_EXPORT void  AAGuiInit(AAOption<> option = AAOption<>());
}
extern "C" {
	AA_WIDGET_CTRL_EXPORT void  AAGuiCreateWindow(AAGuiClient* client, 
		LPCTSTR document, HWND window = NULL, LPRECT rect = NULL);
}
extern "C" {
	AA_WIDGET_CTRL_EXPORT void  AAGuiRun();
}

class AAGuiClient
{
public:
	virtual void OnNativeWidgetCreated(HWND window, AAWidgetWindow* widget_window) = 0;

	virtual void OnNewFrameCreated(LPCTSTR  frame_id) = 0;

	virtual LPCTSTR GetDocument() = 0;

	virtual void Create(HWND window = NULL, LPRECT rect = NULL)
	{
		AAGuiCreateWindow(this, GetDocument(), window, NULL);
	}

};
class AAGuiAux{
public:
	static void AAGuiListViewSet(LPCTSTR  frame_id, LPCTSTR  list_view_id, std::string key_id, std::string  value_id, std::string control_id, std::string value, AAWidgetWindow* widget_window_)
	{
		if(widget_window_ == NULL) return;

		AAGuiArgs Arg;
		Arg.InsertListValue(key_id, value_id); //项id
		Arg.WriteString("id", control_id); //控件id
		Arg.WriteString("func_name", "Set"); //方法名称
		Arg.WriteString("Value", value); //值
		widget_window_->ExcuteCommand(frame_id, list_view_id, L"ListItemExcuteCommand", Arg);

	}
	static std::string AAGuiListViewGet(LPCTSTR  frame_id, LPCTSTR  list_view_id, std::string key_id, std::string  value_id, std::string control_id, AAWidgetWindow* widget_window_)
	{
		if(widget_window_ == NULL) return "";

		AAGuiArgs Arg;
		Arg.InsertListValue(key_id, value_id); //项id
		Arg.WriteString("id", control_id); //控件id
		Arg.WriteString("func_name", "Get"); //方法名称

		widget_window_->ExcuteCommand(frame_id, list_view_id, L"ListItemExcuteCommand", Arg);

		return Arg.GetString("Value");

	}
	static void AAGuiSet(LPCTSTR  frame_id, LPCTSTR  control_id, std::string value, AAWidgetWindow* widget_window_)
	{
		if(widget_window_ == NULL) return;

		AAGuiArgs Arg;
		Arg.WriteString("Value", value); //值
		widget_window_->ExcuteCommand(frame_id, control_id, L"Set", Arg);

	}
	static std::string AAGuiGet(LPCTSTR  frame_id, LPCTSTR  control_id, AAWidgetWindow* widget_window_)
	{
		if(widget_window_ == NULL) return "";

		AAGuiArgs Arg;

		widget_window_->ExcuteCommand(frame_id, control_id, L"Get", Arg);

		return Arg.GetString("Value");

	}
	static void AAGuiListExcuteCommand(LPCTSTR  frame_id, LPCTSTR  list_view_id, std::string key_id, std::string  value_id, std::string control_id, std::string func_name, std::string value, AAWidgetWindow* widget_window_)
	{
		if(widget_window_ == NULL) return;

		AAGuiArgs Arg;
		Arg.InsertListValue(key_id, value_id); //项id
		Arg.WriteString("id", control_id); //控件id
		Arg.WriteString("func_name", func_name); //方法名称
		Arg.WriteString("Value", value); //值
		widget_window_->ExcuteCommand(frame_id, list_view_id, L"ListItemExcuteCommand", Arg);

	}
};
#define GUI_BIND_AREA(frame_, where_, handler) \
	if(widget_window_){widget_window_->ExcuteCommand(frame_, where_, L"BindAllEvent",AAGuiArgs()); \
	widget_window_->BindEventHandler(frame_, where_, L"AllEvent", AAGuiBind(&handler, this)); }

#define GUI_BIND_WIDGET(frame_, where_, handler) \
	if(widget_window_){ \
	widget_window_->BindEventHandler(frame_, where_, L"AllEvent", AAGuiBind(&handler, this)); }

#define GUI_DECL_HANDLER(name) \
	void name(const AAObject* sender, const AAGuiArgs& e)

#define GUI_GET(frame,element) \
	AAGuiAux::AAGuiGet(frame, element, widget_window_);

#define GUI_SET(frame,element, value) \
	AAGuiAux::AAGuiSet(frame, element, value, widget_window_);

#endif
