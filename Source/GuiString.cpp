#include "stdafx.h"
#include "GuiString.h"
#include <assert.h>

AGuiString::AGuiString(void) : m_pstr(m_szBuffer)
{
	memset(m_szBuffer, 0, sizeof(TCHAR)*64);
	m_szBuffer[0] = '\0';
}
AGuiString::~AGuiString()
{
	if (m_pstr != m_szBuffer) free(m_pstr);
}
int AGuiString::Format(LPCTSTR pstrFormat, ...)
{
	AGuiString sFormat = pstrFormat;
	TCHAR szBuffer[1025] = { 0 };
	va_list argList;
	va_start(argList, pstrFormat);
	int iRet = ::wvsprintf(szBuffer, sFormat, argList);
	va_end(argList);
	Assign(szBuffer);
	return iRet;
}
AGuiString::AGuiString(const TCHAR ch) : m_pstr(m_szBuffer)
{
	memset(m_szBuffer, 0, sizeof(TCHAR)*64);
	m_szBuffer[0] = ch;
	m_szBuffer[1] = '\0';
}
AGuiString::AGuiString(LPCTSTR lpsz, int nLen) : m_pstr(m_szBuffer)
{
	memset(m_szBuffer, 0, sizeof(TCHAR)*64);
	assert(!::IsBadStringPtr(lpsz,-1) || lpsz==NULL);
	m_szBuffer[0] = '\0';
	Assign(lpsz, nLen);
}
AGuiString::operator LPCTSTR() const 
{ 
	return m_pstr; 
}
LPCTSTR AGuiString::GetData()
{
	return m_pstr;
}
void AGuiString::Assign(LPCTSTR pstr, int cchMax)
{
	if (pstr == NULL) pstr = _T("");
	cchMax = (cchMax < 0 ? (int) _tcslen(pstr) : cchMax);
	if (cchMax < DEFAULT_LEN) {
		if (m_pstr != m_szBuffer) {
			free(m_pstr);
			m_pstr = m_szBuffer;
		}
	}
	else if (cchMax > Length() || m_pstr == m_szBuffer) {
		if (m_pstr == m_szBuffer) m_pstr = NULL;
		m_pstr = static_cast<LPTSTR>(realloc(m_pstr, (cchMax + 1) * sizeof(TCHAR)));
	}
	_tcsncpy(m_pstr, pstr, cchMax);
	m_pstr[cchMax] = '\0';
}
int AGuiString::Length() const
{ 
	return (int) _tcslen(m_pstr); 
}
