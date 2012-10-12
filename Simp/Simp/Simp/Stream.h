////////////////////////////////////////////////////////////////////////////////
// ��    �� : Stream.h
// ����˵�� :
//   std ����������
//
// ��    �� : 1.0
// ��    �� : Breaker Zhao <breaker.zy_AT_gmail>
// ��    �� : 2011-10
// ��Ȩ��� : Microsoft Public License <http://www.microsoft.com/en-us/openness/licenses.aspx#MPL>
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Simp/Depends.h>
#include <Simp/Defs.h>

SIMP_NS_BEGIN

////////////////////////////////////////////////////////////////////////////////
// _TCHAR �ַ����͵� std �����Ͷ���
////////////////////////////////////////////////////////////////////////////////

typedef std::basic_ios<_TCHAR, std::char_traits<_TCHAR> >                                   tios;
typedef std::basic_streambuf<_TCHAR, std::char_traits<_TCHAR> >                             tstreambuf;
typedef std::basic_istream<_TCHAR, std::char_traits<_TCHAR> >                               tistream;
typedef std::basic_ostream<_TCHAR, std::char_traits<_TCHAR> >                               tostream;
typedef std::basic_iostream<_TCHAR, std::char_traits<_TCHAR> >                              tiostream;
typedef std::basic_stringbuf<_TCHAR, std::char_traits<_TCHAR>, std::allocator<_TCHAR> >     tstringbuf;
typedef std::basic_istringstream<_TCHAR, std::char_traits<_TCHAR>, std::allocator<_TCHAR> > tistringstream;
typedef std::basic_ostringstream<_TCHAR, std::char_traits<_TCHAR>, std::allocator<_TCHAR> > tostringstream;
typedef std::basic_stringstream<_TCHAR, std::char_traits<_TCHAR>, std::allocator<_TCHAR> >  tstringstream;
typedef std::basic_filebuf<_TCHAR, std::char_traits<_TCHAR> >                               tfilebuf;
typedef std::basic_ifstream<_TCHAR, std::char_traits<_TCHAR> >                              tifstream;
typedef std::basic_ofstream<_TCHAR, std::char_traits<_TCHAR> >                              tofstream;
typedef std::basic_fstream<_TCHAR, std::char_traits<_TCHAR> >                               tfstream;

////////////////////////////////////////////////////////////////////////////////
// _TCHAR �ַ����͵� std ��ȫ�ֱ���
////////////////////////////////////////////////////////////////////////////////

// NOTE:
// 1. ʹ����������, ������������, ֻ�� std �б�׼����ı���
// 2. ʹ�� static �ڲ����ӷ�ʽ, �Ա��� .h �ļ��ж���, ������������ȫ�ֱ���, �� header-only
//    ��Ϊȫ�ֱ���Ĭ��ʹ���ⲿ���ӷ�ʽ, ��� .cpp ������ .h ������ζ���, �������Ӵ��� LNK2005

#ifdef _UNICODE
static std::wistream& tcin  = std::wcin;
static std::wostream& tcout = std::wcout;
static std::wostream& tcerr = std::wcerr;
static std::wostream& tclog = std::wclog;
#else
// _UNICODE NOT defined
static std::istream& tcin   = std::cin;
static std::ostream& tcout  = std::cout;
static std::ostream& tcerr  = std::cerr;
static std::ostream& tclog  = std::clog;
#endif

SIMP_NS_END
