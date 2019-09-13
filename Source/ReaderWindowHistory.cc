#include "ReaderWindow.h"
#include "AppConfig.h"

void  ReaderWindow::OnHistoryEvent(const AAObject* sender, const AAGuiArgs& e)
{
	std::string event_name_ = e.GetString("EventName");
	std::string element_id = e.GetString();

	if(element_id == "btnQueryHistory")
	{
		std::string alldata = GUI_GET(L"history-view", L"checkQueryAllData");
		if(alldata == "false")
		QueryHistoryProcedure("1", false);
		else
		QueryHistoryProcedure("1", true);
	}
	else if(element_id == "LogPageWidget")
	{

		if(e.GetString("EventName") == "PageSelect")
		{
			std::string alldata = GUI_GET(L"history-view", L"checkQueryAllData");
			if(alldata == "false")
			QueryHistoryProcedure(e.GetString("Value").c_str(), false);
			else
			QueryHistoryProcedure(e.GetString("Value").c_str(), true);

		}
	}
	else if(event_name_ == "TimeSelect" )
	{
		QueryHistoryProcedure("1", false);
	}
	else if(event_name_ == "TextChangeEvent")
	{
		std::wstring key_word = GUI_GETW(L"history-view", L"EditKeyWord");	
		if(last_key_word == key_word)
			return;
	
		last_key_word = key_word;
		
		AAGUIEXEC(L"history-view", L"EmptyText", L"Set", AAGuiArgs("Value", "正在加载..."));
		AAGUIEXEC(L"history-view", L"HistoryListView", L"RemoveAll", AAGuiArgs());

		std::string alldata = GUI_GET(L"history-view", L"checkQueryAllData");
		std::string start_time = GUI_GET(L"history-view", L"date_time");

		std::string end_time = start_time;

		start_time += " 00:00:00";

		end_time += " 23:59:59";

		AAGuiArgs historyArg;

		historyArg.WriteStringW("start_time", str_conv::str2wstr(start_time));
		historyArg.WriteStringW("end_time",  str_conv::str2wstr(end_time));
		historyArg.WriteStringW("key_word", key_word.c_str());
		historyArg.WriteString("log_page", "1");
		historyArg.WriteString("alldata", alldata);
		dataExec(0, "obj_document", "query_log",
			historyArg, ReaderWindow::OnHistoryData);
	}
	else if(element_id == "checkQueryAllData")
	{
		std::string is_enable =	e.GetString("Value");

		if(is_enable == "true")
		{
			AAGUIEXEC(L"history-view", L"date_time", L"Enable", AAGuiArgs("Value", "false"));
			QueryHistoryProcedure("1", true);
		}
		else
		{
			AAGUIEXEC(L"history-view", L"date_time", L"Enable", AAGuiArgs("Value", "true"));
			QueryHistoryProcedure("1", false);
		}
	}
	else if(element_id == "btnDeleteRecord")
	{
		std::string Period = GUI_GET(L"history-view", L"ComboPeriod");

		std::string start_time = "";

		std::string end_time = "";
		if(Period == "过去一小时")
		{
			AAGuiArgs periodArg;

			periodArg.WriteString("type", "prev_hour");
			AAGUIEXEC(L"history-view", L"date_time", L"GetPeriod", periodArg);

			start_time = periodArg.GetString("prev");
			end_time = periodArg.GetString("now");

			
		}
		else if(Period == "过去一天")
		{
			AAGuiArgs periodArg;

			periodArg.WriteString("type", "prev_day");
			AAGUIEXEC(L"history-view", L"date_time", L"GetPeriod", periodArg);

			start_time = periodArg.GetString("prev");
			end_time = periodArg.GetString("now");
		}
		else if(Period == "过去一周")
		{
			AAGuiArgs periodArg;

			periodArg.WriteString("type", "prev_week");
			AAGUIEXEC(L"history-view", L"date_time", L"GetPeriod", periodArg);

			start_time = periodArg.GetString("prev");
			end_time = periodArg.GetString("now");
		}
		else if(Period == "过去一个月")
		{
			AAGuiArgs periodArg;

			periodArg.WriteString("type", "prev_month");
			AAGUIEXEC(L"history-view", L"date_time", L"GetPeriod", periodArg);

			start_time = periodArg.GetString("prev");
			end_time = periodArg.GetString("now");
		}
		else if(Period == "全部")
		{
			AAGuiArgs historyArg;

			historyArg.WriteStringW("start_time", str_conv::str2wstr(start_time));
			historyArg.WriteStringW("end_time",  str_conv::str2wstr(end_time));
			historyArg.WriteString("alldata", "true");
			dataExec(0, "obj_document", "query_log_delete",
				historyArg, ReaderWindow::OnHistoryData);
			AAGUIEXEC(L"root", L"history-view", L"popup_layer",  AAGuiArgs("command", "anime_destory"));
			return;
		}

		AAGuiArgs historyArg;

		historyArg.WriteStringW("start_time", str_conv::str2wstr(start_time));
		historyArg.WriteStringW("end_time",  str_conv::str2wstr(end_time));
		historyArg.WriteString("alldata", "false");
		dataExec(0, "obj_document", "query_log_delete",
			historyArg, ReaderWindow::OnHistoryData);

		AAGUIEXEC(L"root", L"history-view", L"popup_layer",  AAGuiArgs("command", "anime_destory"));
	}
	else if(element_id == "btnDeleteRecordConfig")
	{
		
		if(history_total_count)
		{
			AAGuiArgs arg("document", "message_box\\delete_history_configure.xml");
			arg.WriteString("command", "create");
			widget_window_->ExcuteCommand(L"root", L"history-view", L"popup_layer", arg);
		}

	}
	else if(element_id == "MenuDeleteRecord")
	{

		AAGuiArgs listItemArg;
		AAGUIEXEC(L"history-view", L"HistoryListView", L"GetSelectData", listItemArg);

		std::string item_id = listItemArg.GetString("id");
		std::wstring item_path = listItemArg.GetStringW("path");

		AAGuiArgs pageArg;
		AAGUIEXEC(L"history-view", L"LogPageWidget", L"GetCurrentPage", pageArg);

		std::string current_page = pageArg.GetString("Value");
		AAGuiArgs historyArg;

		historyArg.WriteString("id", item_id);
		historyArg.WriteString("current_page", current_page);
		dataExec(0, "obj_document", "query_log_delete_id",
			historyArg, ReaderWindow::OnHistoryData);


		AAGUIEXEC(L"history-view", L"HistoryListView", L"RemoveSelect", AAGuiArgs());
		AAGUIEXEC(NULL, L"root", L"HideContextLayer", AAGuiArgs());
	}
	else if(element_id == "MenuShowFolder")
	{

		AAGuiArgs listItemArg;
		AAGUIEXEC(L"history-view", L"HistoryListView", L"GetSelectData", listItemArg);

		std::string item_id = listItemArg.GetString("id");
		std::wstring item_path = listItemArg.GetStringW("path");

		AAGUIEXEC(NULL, L"root", L"HideContextLayer", AAGuiArgs());

		if(!item_path.empty())
		{
			AAGuiArgs load_arg;
			load_arg.WriteStringW("Value", item_path);
			AAGUIEXEC(0, L"root", L"FileInFolder", load_arg);
		}
	}
	else if(element_id == "MenuOpen")
	{

		AAGuiArgs listItemArg;
		AAGUIEXEC(L"history-view", L"HistoryListView", L"GetSelectData", listItemArg);

		std::string item_id = listItemArg.GetString("id");
		std::wstring item_path = listItemArg.GetStringW("path");
		AAGUIEXEC(NULL, L"root", L"HideContextLayer", AAGuiArgs());

		
		AAGuiArgs openArg;

		openArg.WriteStringW("file_name", item_path);
		dataExecNULL(0, "obj_document", "open_new_window",
			openArg);
		

	}
	if(event_name_ == "BeforeMenuShow")
	{
		std::string exist_ = e.GetString("exist");
	
		if(!exist_.empty())
		{
			AAGuiArgs menuArg;
			menuArg.WriteString("menuid1", "MenuOpen");
			menuArg.WriteString("menuid2", "MenuShowFolder");
			menuArg.WriteString("newheight", "46");
			menuArg.WriteString("newwidth", "165");
			menuArg.WriteString("Value", "false");
			AAGUIEXEC(L"history-view", L"HistoryListView", L"SetVisibleSubMenu", menuArg);
		}
		else
		{
			AAGuiArgs menuArg;
			menuArg.WriteString("menuid1", "MenuOpen");
			menuArg.WriteString("menuid2", "MenuShowFolder");
			menuArg.WriteString("newheight", "102");
			menuArg.WriteString("newwidth", "185");
			menuArg.WriteString("Value", "true");
			AAGUIEXEC(L"history-view", L"HistoryListView", L"SetVisibleSubMenu", menuArg);
		}
	}
}
void ReaderWindow::QueryHistoryProcedure(std::string page, bool alldata)
{

	if(!alldata)
	{
		std::wstring key_word = GUI_GETW(L"history-view", L"EditKeyWord");	

		AAGUIEXEC(L"history-view", L"EmptyText", L"Set", AAGuiArgs("Value", "正在加载..."));
		AAGUIEXEC(L"history-view", L"HistoryListView", L"RemoveAll", AAGuiArgs());

		std::string start_time = GUI_GET(L"history-view", L"date_time");

		std::string end_time = start_time;

		start_time += " 00:00:00";

		end_time += " 23:59:59";

		AAGuiArgs historyArg;

		historyArg.WriteStringW("start_time", str_conv::str2wstr(start_time));
		historyArg.WriteStringW("end_time",  str_conv::str2wstr(end_time));
		historyArg.WriteStringW("key_word", key_word.c_str());
		historyArg.WriteString("log_page", page);
		historyArg.WriteString("alldata", "false");
		dataExec(0, "obj_document", "query_log",
			historyArg, ReaderWindow::OnHistoryData);
	}
	else
	{

		std::wstring key_word = GUI_GETW(L"history-view", L"EditKeyWord");	

		AAGUIEXEC(L"history-view", L"EmptyText", L"Set", AAGuiArgs("Value", "正在加载..."));
		AAGUIEXEC(L"history-view", L"HistoryListView", L"RemoveAll", AAGuiArgs());

		std::string start_time = GUI_GET(L"history-view", L"date_time");

		std::string end_time = start_time;

		start_time += " 00:00:00";

		end_time += " 23:59:59";

		AAGuiArgs historyArg;

		historyArg.WriteStringW("start_time", str_conv::str2wstr(start_time));
		historyArg.WriteStringW("end_time",  str_conv::str2wstr(end_time));
		historyArg.WriteStringW("key_word", key_word.c_str());
		historyArg.WriteString("log_page", page);
		historyArg.WriteString("alldata", "true");
		dataExec(0, "obj_document", "query_log",
			historyArg, ReaderWindow::OnHistoryData);
	}
}
void ReaderWindow::QueryHistory()
{
	/*AAGuiArgs arg("document", "loading\\query_history_loading.xml");
	arg.WriteString("command", "create");
	widget_window_->ExcuteCommand(L"root", L"history-view", L"popup_layer", arg);*/
	//GUI_SET(L"history_empty", L"EmptyText", "fuck");
	last_key_word = L"";
	std::string start_time = GUI_GET(L"history-view", L"date_time");

	std::string end_time = start_time;

	start_time += " 00:00:00";

	end_time += " 23:59:59";

	std::wstring key_word = GUI_GETW(L"history-view", L"EditKeyWord");	
	AAGuiArgs historyArg;

	historyArg.WriteStringW("start_time", str_conv::str2wstr(start_time));
	historyArg.WriteStringW("end_time",  str_conv::str2wstr(end_time));
	historyArg.WriteStringW("key_word", key_word.c_str());
	historyArg.WriteString("log_page", "1");

	dataExec(0, "obj_document", "query_log",
		historyArg, ReaderWindow::OnHistoryData);
}
void ReaderWindow::OnHistoryData(const dataObj *sender, const AAGuiArgs& value)
{

	//std::string retValue = value.GetString("retvalue");

	std::string success = value.GetString("success");
	std::string type = value.GetString("type");

	if(type == "delete")
	{
		if(widget_window_ )
		{
			history_current_page = str_conv::string_2_int(value.GetString("current_page"));
			widget_window_->PostTask(AAGuiNewTask(&ReaderWindow::uiTaskRefreshHistory, this));
		}
		return;
	}
	if(success == "true")
	{
		history_total_count = str_conv::string_2_int(value.GetString("total_count"));
		AAGUIEXEC(L"history-view", L"LogPageWidget", L"SetTotalPage", AAGuiArgs("Value", value.GetString("total_page")));
		AAGUIEXEC(L"history-view", L"LogPageWidget", L"SetCurrentPage", AAGuiArgs("Value", value.GetString("cur_page")));

		AAGUIEXEC(L"history-view", L"HistoryListView", L"RemoveAll", AAGuiArgs());
		AAGuiArgs* historyList = value.GetValue("list");
		if(historyList)
		AAGUIEXEC(L"history-view", L"HistoryListView", L"AddItem", *historyList);
	}
	else
	{
		history_total_count = str_conv::string_2_int(value.GetString("total_count"));
		AAGUIEXEC(L"history-view", L"EmptyText", L"Set", AAGuiArgs("Value", "未搜索到任何记录"));
		AAGUIEXEC(L"history-view", L"HistoryListView", L"RemoveAll", AAGuiArgs());

		AAGUIEXEC(L"history-view", L"LogPageWidget", L"SetTotalPage", AAGuiArgs("Value", "1"));
		AAGUIEXEC(L"history-view", L"LogPageWidget", L"SetCurrentPage", AAGuiArgs("Value", "1"));
	}
}
void ReaderWindow::uiTaskRefreshHistory(AAGuiArgs& e)
{
	std::string alldata = GUI_GET(L"history-view", L"checkQueryAllData");

	if(history_current_page > 1)
	{
		if(alldata == "false")
			QueryHistoryProcedure(str_conv::int_2_string(history_current_page), false);
		else
			QueryHistoryProcedure(str_conv::int_2_string(history_current_page), true);
	}
	else
	{
		if(alldata == "false")
			QueryHistoryProcedure("1", false);
		else
			QueryHistoryProcedure("1", true);
	}
	return;
}