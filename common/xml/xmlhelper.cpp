#include "stdafx.h"
#include "util/base.h"
#include "xmlhelper.h"

CXmlHelper::CXmlHelper() {}

CXmlHelper::~CXmlHelper() {}

BOOL CXmlHelper::LoadFile(LPCTSTR pFileName) {
#ifdef UNICODE
  USES_CONVERSION;
  LPCSTR pAnsiFileName = T2A(pFileName);
  return m_xmlDoc.LoadFile(pAnsiFileName);
#else
  return m_xmlDoc.LoadFile(pFileName);
#endif
}

BOOL CXmlHelper::loadResFile(HINSTANCE hInstance, LPCTSTR lpszResName,
                             LPCTSTR lpszResType) {
  HRSRC hRes = FindResource(hInstance, lpszResName, lpszResType);
  if (hRes) {
    HGLOBAL hGlobal = LoadResource(hInstance, hRes);
    if (hGlobal) {
      LPVOID lpResource = ::LockResource(hGlobal);
      if (lpResource) {

        TCHAR szTempPath[MAX_PATH] = {0};
        TCHAR szTempFile[MAX_PATH] = {0};
        GetTempPath(SIZEOF(szTempPath) - 1, szTempPath);
        GetTempFileName(szTempPath, _T("GH_"), 0, szTempFile);

        HANDLE hFile = CreateFile(szTempFile, GENERIC_WRITE, 0, NULL,
                                  CREATE_ALWAYS, 0, NULL);
        DWORD dwBytesWritten = 0;
        if (hFile != INVALID_HANDLE_VALUE) {
          WriteFile(hFile, lpResource, SizeofResource(_hInstance, hRes),
                    &dwBytesWritten, NULL);
          CloseHandle(hFile);
        }

        FreeResource(hGlobal);

        BOOL bRet = LoadFile(szTempFile);
        SetFileAttributes(szTempFile, FILE_ATTRIBUTE_NORMAL);
        DeleteFile(szTempFile);
        return bRet;
      }
      FreeResource(hGlobal);
    }
  }
  return FALSE;
}

BOOL CXmlHelper::LoadStringW(WCHAR *lpzXmlStr) {
  string strXml = Util::Base::UnicodeToUTF8(lpzXmlStr);
  m_xmlDoc.Parse(strXml.c_str(), 0, TIXML_ENCODING_UTF8);
  return TRUE;
}

BOOL CXmlHelper::LoadString(LPCTSTR pXmlStr) {
#ifdef UNICODE
  USES_CONVERSION;
  LPCSTR pAnsiXmlStr = T2A(pXmlStr);
  m_xmlDoc.Parse(pAnsiXmlStr);
#else
  m_xmlDoc.Parse(pXmlStr);
#endif

  return TRUE;
}

XmlElementPtr CXmlHelper::GetRoot() { return m_xmlDoc.RootElement(); }

tstring CXmlHelper::GetElementName(XmlElementPtr pElement) {
  return Util::Base::UTF8ToUnicode(pElement->Value());
}

tstring CXmlHelper::GetElementValue(XmlElementPtr pElement) {
  return Util::Base::UTF8ToUnicode(pElement->GetText());
}

std::vector<XmlElementPtr> CXmlHelper::GetChild(XmlElementPtr pElement) {
  std::vector<XmlElementPtr> vecChild;
  XmlElementPtr pChild = pElement->FirstChildElement();
  while (pChild) {
    vecChild.push_back(pChild);

    pChild = pChild->NextSiblingElement();
  }

  return vecChild;
}

tstring CXmlHelper::GetAttributeString(XmlElementPtr pElement,
                                       string lpszAttriName) {
  LPCSTR lpszValue = pElement->Attribute(lpszAttriName.c_str());
  return Util::Base::UTF8ToUnicode(lpszValue);
}

int CXmlHelper::GetAttributeInt(XmlElementPtr pElement, string lpszAttriName) {
  int nValue = 0;
  pElement->Attribute(lpszAttriName.c_str(), &nValue);
  return nValue;
}

BOOL CXmlHelper::GetAttributeBool(XmlElementPtr pElement,
                                  string lpszAttriName) {
  bool bValue = TRUE;
  pElement->QueryBoolAttribute(lpszAttriName.c_str(), &bValue);
  if (bValue)
    return TRUE;
  else
    return FALSE;
}

double CXmlHelper::GetAttributeDouble(XmlElementPtr pElement,
                                      string lpszAttriName) {
  double dValue = 0.0;
  pElement->QueryDoubleAttribute(lpszAttriName.c_str(), &dValue);
  return dValue;
}

void CXmlHelper::InsertDeclaration(string strVer, string strEncode) {
  TiXmlDeclaration *dec =
      new TiXmlDeclaration(strVer.c_str(), strEncode.c_str(), "");
  m_xmlDoc.LinkEndChild(dec);
}
XmlElementPtr CXmlHelper::InsertChild(string strName) {
  TiXmlElement *pChild = new TiXmlElement(strName.c_str());
  m_xmlDoc.LinkEndChild(pChild);

  return pChild;
}

void CXmlHelper::InsertChild(XmlElementPtr pElement) {
  m_xmlDoc.LinkEndChild(pElement);
}

XmlElementPtr CXmlHelper::InsertChild(XmlElementPtr pElement, string strName) {
  TiXmlElement *pChild = new TiXmlElement(strName.c_str());
  if (pChild) {
    pElement->LinkEndChild(pChild);
  }

  return pChild;
}

void CXmlHelper::SetElementString(XmlElementPtr pElement, tstring strVal) {
#ifdef UNICODE
  TiXmlText *pStrName = new TiXmlText(Util::Base::UnicodeToUTF8(strVal.c_str()).c_str());
#else
  TiXmlText *pStrName = new TiXmlText(strVal.c_str());
#endif
  pElement->LinkEndChild(pStrName);
}

void CXmlHelper::SetElementInt(XmlElementPtr pElement, int nVal) {
  TCHAR szVal[64] = {0};
  StringCchPrintf(szVal, 64, _T("%d"), nVal);
  SetElementString(pElement, szVal);
}

void CXmlHelper::SetAttributeString(XmlElementPtr pElement, string strAttriName,
                                    tstring strAttriVal) {
#ifdef UNICODE
  pElement->SetAttribute(strAttriName.c_str(),
                         Util::Base::UnicodeToUTF8(strAttriVal.c_str()).c_str());
#else
  pElement->SetAttribute(strAttriName.c_str(), strAttriVal.c_str());
#endif
}

void CXmlHelper::SetAttributeInt(XmlElementPtr pElement, string strAttriName,
                                 int nAttriVal) {
  TCHAR szVal[64] = {0};
  StringCchPrintf(szVal, 64, _T("%d"), nAttriVal);
  SetAttributeString(pElement, strAttriName, szVal);
}

void CXmlHelper::SaveFile(LPCTSTR pFileName) {
#ifdef UNICODE
  USES_CONVERSION;
  LPCSTR pAnsiFileName = T2A(pFileName);
  m_xmlDoc.SaveFile(pAnsiFileName);
#else
  m_xmlDoc.SaveFile(pFileName);
#endif
}
