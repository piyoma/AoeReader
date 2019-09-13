#ifndef __aa_object_h__
#define __aa_object_h__

#include <windows.h>


class AAObject;
class AAGuiArgs;
class AAGuiEventHandler;
class AAObject
{
public:
	
	AAObject::AAObject() : m_uRef(0)
	{
		AddRef();
	}

	AAObject::~AAObject()
	{

	}

	void AAObject::AddRef()
	{
		InterlockedIncrement((long *)&m_uRef);
	}

	void AAObject::Release()
	{
		InterlockedDecrement((long *)&m_uRef);
		if (m_uRef == 0)
		{
			delete this;
		}
	}
	virtual void AAGuiAddEventHandler(AAGuiEventHandler* handler)
	{

	}
	virtual void AsuraExecCmd(std::wstring& func_name, AAGuiArgs& args){};

	virtual AAObject* getAsuraObjById(std::wstring& id){return NULL;}

	virtual bool isAsuraObjectId(std::wstring& id){return false;};

	AAObject* AsAsuraObject(){return this;};

	const char* AAObject::ToString()
	{
		return "";
	}
	unsigned int GetRefCounted(){
		return m_uRef;
	}
protected:

	unsigned int m_uRef;
};

#endif