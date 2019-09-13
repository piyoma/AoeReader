#ifndef __aa_gui_event_handler_h__
#define __aa_gui_event_handler_h__


#include <vector>
#include <string>
#include <map>
#include "AAObject.h"

class AAGuiArgs;

template<typename D>
class AARawData{
public:
	virtual std::vector<D>& GetDataVecotor(int index) = 0;
	virtual void RecyclingData(int index) = 0;
	//virtual std::map<Q, D>& GetDataMap(int index) = 0;
};
class AAGuiArgs : AAObject
{
public:

	typedef std::vector<AAGuiArgs*> ListValue;

	typedef std::map<std::string, AAGuiArgs*> DictionaryValue;

	AAGuiArgs():data_ptr_(nullptr)
	{

	};
	AAGuiArgs(int value) : int_value(value), data_ptr_(nullptr)
	{

	};

	AAGuiArgs( std::string value) : string_value(value), data_ptr_(nullptr)
	{

	};
	AAGuiArgs(std::wstring value) : wstring_value(value), data_ptr_(nullptr)
	{

	};
	AAGuiArgs(std::string value1, std::wstring value2) : string_value(value1), wstring_value(value2), data_ptr_(nullptr)
	{

	};
	AAGuiArgs(char* value) : string_value(value), data_ptr_(nullptr)
	{

	};
	AAGuiArgs(std::string key, int value) : data_ptr_(nullptr)
	{
		WriteInt(key, value);
	}
	AAGuiArgs(std::string key, std::string value): data_ptr_(nullptr)
	{
		WriteString(key, value);
	}
	AAGuiArgs(std::string key, char* value) : data_ptr_(nullptr)
	{
		WriteString(key, value);
	}
	virtual ~AAGuiArgs()
	{
		Clear();
	};

	operator  int() const
	{
		return int_value;
	};
	int GetInt() const
	{
		return int_value;
	}
	operator std::string() const
	{
		return string_value;
	};
	std::string GetString() const
	{
		return string_value;
	}
	std::wstring GetStringW() const
	{
		return wstring_value;
	}

	std::string GetString(const std::string& key) const
	{
		AAGuiArgs* value = GetValue(key);

		if(value)
		{
			return value->GetString();
		}
		return "";
	}
	std::wstring GetStringW(const std::string& key) const
	{
		AAGuiArgs* value = GetValue(key);

		if(value)
		{
			return value->GetStringW();
		}
		return L"";
	}
	int GetInt(const std::string& key) const
	{
		AAGuiArgs* value = GetValue(key);

		if(value)
		{
			return value->GetInt();
		}
		return -1;
	}
	void WriteInt(int value)
	{
		int_value = value;
	}
	void WriteString(std::string value)
	{
		string_value = value;
	}
	void WriteStringW(std::wstring value)
	{
		wstring_value = value;
	}
	void WriteInt(std::string key, int value)
	{
		m_DictionaryValue[key] = new AAGuiArgs(value);
	}
	void WriteString(std::string key, std::string value)
	{
		WriteValue(key, new AAGuiArgs(value));
	}
	void WriteStringW(std::string key, std::wstring value)
	{
		WriteValue(key, new AAGuiArgs(value));
	}
	void WriteString(std::string key, std::string value, std::wstring value2)
	{
		WriteValue(key, new AAGuiArgs(value, value2));
	}
	void WriteValue(std::string key, AAGuiArgs* value)
	{
		AAGuiArgs* old_value_ = GetValue(key);

		if(old_value_)
		{
			delete old_value_;
			old_value_ = NULL;
		}
		m_DictionaryValue[key] = value;
	}
	void WriteValue(AAGuiArgs* value)
	{
		m_ListValue.push_back(value);
	}
	void InsertListValue(std::string key, std::string value)
	{
		AAGuiArgs* item_value = new AAGuiArgs;
		item_value->WriteString(key, value);
		m_ListValue.push_back(item_value);
	}
	inline AAGuiArgs* operator[](const std::string& key)
	{
		if(m_DictionaryValue.count(key) > 0)
		{
			return m_DictionaryValue[key];
		}
		return NULL;
	}
	inline AAGuiArgs* GetValue(const std::string& key)
	{
		return operator[](key);
	}
	AAGuiArgs* operator[](const std::string& key) const
	{
		if(m_DictionaryValue.count(key) > 0)
		{
			return m_DictionaryValue.at(key);
		}
		return NULL;
	}
	AAGuiArgs* GetValue(const std::string& key) const
	{
		return operator[](key);
	}
	inline AAGuiArgs* GetValue(const int index)
	{
		return operator[](index);
	}
	inline AAGuiArgs* operator[](const int index)
	{
		if(index >= 0 && index < m_ListValue.size())
		return m_ListValue[index];
	
		return NULL;
	}
	
	 void Clear()
	 {
		 DictionaryValue::iterator dict_iterator = m_DictionaryValue.begin();
		 while(dict_iterator != m_DictionaryValue.end())
		 {
			 delete dict_iterator->second;
			 ++dict_iterator;
		 }

		 m_DictionaryValue.clear();

		 for(ListValue::iterator i=m_ListValue.begin(); i!=m_ListValue.end(); ++i)
		 {
			 delete *i;
		 }
		 m_ListValue.clear();
		 ListValue(m_ListValue).swap(m_ListValue);
	 }
	 bool HasListValue() const 
	 {
		 return !m_ListValue.empty(); 
	 }
	 bool HasDictionaryValue() const 
	 {
		 return !m_DictionaryValue.empty(); 
	 }
	 const ListValue& GetValue() const
	 {
		 return m_ListValue;
	 }
	 const DictionaryValue& GetDictionaryValue() const
	 {
		 return m_DictionaryValue;
	 }
	 AAGuiArgs* Copy()
	 {
		 AAGuiArgs* arg = new AAGuiArgs;
		 Copy(arg);
		 return arg;
	 }
	 void Copy(AAGuiArgs* arg)
	 {
		CopyValue(arg);
		CopyListValue(arg);
		CopyDictionaryValue(arg);
		CopyRawData(arg);
	 }
	 void CopyValue(AAGuiArgs* arg)
	 {
		 if(arg)
		 {
			 arg->WriteInt(int_value);
			 arg->WriteString(string_value);
			 arg->WriteStringW(wstring_value);
		 }
	 }
	 void CopyListValue(AAGuiArgs* arg)
	 {
		 if(arg)
		 {
			ListValue::iterator it = m_ListValue.begin();
			for(;it!=m_ListValue.end(); ++it)
			{
				arg->WriteValue((*it)->Copy());
			}
		 }
	 }
	 void CopyDictionaryValue(AAGuiArgs* arg)
	 {
		 if(arg)
		 {
			 DictionaryValue::iterator it = m_DictionaryValue.begin();
			 for(;it!=m_DictionaryValue.end(); ++it)
			 {
				 arg->WriteValue(it->first, it->second->Copy());
			 }
		 }
	 }
	 void CopyRawData(AAGuiArgs* arg)
	 {
		 if(arg)
		 {
			arg->WriteData(GetData());
		 }
	 }
	 void WriteData(void* ptr)
	 {
		 data_ptr_ = ptr;
	 }
	 void* GetData()
	 {
		 return data_ptr_;
	 }
private:
	int int_value;
	void* data_ptr_;
	std::string string_value;
	std::wstring wstring_value;
	DictionaryValue m_DictionaryValue;
	ListValue   m_ListValue;
};
class  AAGuiTask
	: public  AAObject
{
public:
	AAGuiTask(void* obj, void* sig)
		:obj_(obj), Sig_(sig){};

	virtual ~AAGuiTask(){};

	virtual void Run(AAGuiArgs& e) = 0;

	void* object() 
	{
		return obj_; 
	}
private:
	void* obj_;
	void* Sig_;
};
template<typename P, typename T>
class AAGuiTaskImpl : public AAGuiTask
{
	typedef void (T::* Sig)(AAGuiArgs& e);

public:
	AAGuiTaskImpl(P* obj, Sig sig) : AAGuiTask(obj, &sig), func_(sig) { };

	virtual void Run(AAGuiArgs& e)
	{

		P* pObj = (P*) object();
		(pObj->*func_)(e); 
	};

private:
	Sig func_;
};
class  AAGuiEventHandler
	: public  AAObject
{
public:
	AAGuiEventHandler(){};

	virtual ~AAGuiEventHandler(){};

	virtual void Invoke(const AAObject* sender,const AAGuiArgs& e) = 0;

};

template<typename P, typename T>
class AAGuiEventHandlerImpl : public AAGuiEventHandler
{
	typedef void (T::* Sig)(const AAObject *sender, const AAGuiArgs& e);

public:
	friend class AAGuiEvent;

	 AAGuiEventHandlerImpl(P* obj, Sig sig) : obj_(obj), func_(sig) {};

	 virtual void Invoke(const AAObject *sender, const AAGuiArgs& e)
	 {
		 (obj_->*func_)(sender, e); 
	 };

private:
	Sig func_;
	T*  obj_;
};
template <class P, class T>
AAGuiEventHandlerImpl<P, T>* AAGuiBind(void (T::* sig)(const AAObject *sender,const AAGuiArgs& e), P* o)
{
	return new AAGuiEventHandlerImpl<P, T>(o, sig);
}
template <class P, class T>
AAGuiTaskImpl<P, T>* AAGuiNewTask(void (T::* sig)(AAGuiArgs& e), P* o)
{
	return new AAGuiTaskImpl<P, T>(o, sig);
}

struct asura_setting_type{
	bool use_zip;
	bool use_font;
	bool disable_dpi;
	LPCTSTR resource_filename;
	LPCTSTR font_path;
	LPCTSTR default_res_foldname;
};
struct AASettingsTraits {
	typedef asura_setting_type struct_t;

	static inline void init(struct_t* s) {
		s->use_zip = false;
		s->use_font = true;
		s->resource_filename = nullptr;
		s->font_path = nullptr;
		s->default_res_foldname = nullptr;
		s->disable_dpi = false;
	}
};

template <typename struct_traits>
class AAStructType : public struct_traits::struct_t {
public:
	typedef typename struct_traits::struct_t struct_t;
	AAStructType(){
	   Init();
	}
	~AAStructType(){
	}
protected:
	void Init() {
		memset(static_cast<struct_t*>(this), 0, sizeof(struct_t));
		struct_traits::init(this);
	}
};

template <typename struct_type = AASettingsTraits, 
	typename base_type = AAStructType<struct_type>>

class AAOption : public base_type{};

typedef AAOption<> AAGuiOption;

#endif