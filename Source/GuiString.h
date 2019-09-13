#ifndef __Gui_String_h__
#define __Gui_String_h__

#include <tchar.h>

class AGuiString
{
public:
	enum { DEFAULT_LEN = MAX_PATH };

	AGuiString(void);
	AGuiString(const TCHAR ch);
	AGuiString(LPCTSTR lpsz, int nLen = -1);
	~AGuiString();

	operator LPCTSTR() const;
	LPCTSTR GetData();
	int __cdecl Format(LPCTSTR pstrFormat, ...);
	void Assign(LPCTSTR pstr, int nLength = -1);
	int Length() const;

protected:
	LPTSTR m_pstr;
	TCHAR m_szBuffer[DEFAULT_LEN + 1];
};

#endif


