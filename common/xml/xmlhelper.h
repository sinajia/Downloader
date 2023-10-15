#pragma once

#include <strsafe.h>
#include <string>
#include <vector>
#include "tinyxml.h"

#ifndef tstring
#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif
#endif

typedef TiXmlDocument XmlDocument;
typedef TiXmlElement *XmlElementPtr;
typedef TiXmlAttribute *XmlAttributePtr;

class CXmlHelper {
  using string = std::string;
  using wstring = std::wstring;
public:
  CXmlHelper();
  virtual ~CXmlHelper();

  static CXmlHelper &GetInstance() {
    static CXmlHelper This;
    return This;
  }

  BOOL LoadFile(LPCTSTR pFileName);
  BOOL loadResFile(HINSTANCE hInstance, LPCTSTR lpszResName,
                   LPCTSTR lpszResType);
  BOOL LoadString(LPCTSTR lpszXmlStr);
  BOOL LoadStringW(WCHAR *lpzXmlStr);
  XmlElementPtr GetRoot();
  std::vector<XmlElementPtr> GetChild(XmlElementPtr pElement);
  tstring GetElementName(XmlElementPtr pElement);
  tstring GetElementValue(XmlElementPtr pElement);
  tstring GetAttributeString(XmlElementPtr pElement, string lpszAttriName);
  int GetAttributeInt(XmlElementPtr pElement, string lpszAttriName);
  BOOL GetAttributeBool(XmlElementPtr pElement, string lpszAttriName);
  double GetAttributeDouble(XmlElementPtr pElement, string lpszAttriName);

  void InsertDeclaration(string strVer = "1.0", string strEncode = "UTF-8");
  void InsertChild(XmlElementPtr pElement);
  XmlElementPtr InsertChild(string strName);
  XmlElementPtr InsertChild(XmlElementPtr pElement, string strName);
  void SetElementString(XmlElementPtr pElement, tstring strVal);
  void SetElementInt(XmlElementPtr pElement, int nVal);
  void SetAttributeString(XmlElementPtr pElement, string strAttriName,
                          tstring strAttriVal);
  void SetAttributeInt(XmlElementPtr pElement, string strAttriName,
                       int nAttriVal);
  void SaveFile(LPCTSTR pFileName);

private:
  XmlDocument m_xmlDoc;

};
