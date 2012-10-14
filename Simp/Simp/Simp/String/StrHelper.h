////////////////////////////////////////////////////////////////////////////////
// ��    �� : StrHelper.h
// ����˵�� :
//   �ַ������ò���
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
// ���Ͷ���
////////////////////////////////////////////////////////////////////////////////

typedef std::basic_string<_TCHAR> tstring;

////////////////////////////////////////////////////////////////////////////////
// �ַ������� CharTraits
////////////////////////////////////////////////////////////////////////////////

template <typename CharT>
struct CharTraits {
    typedef CharT   CharType;
    typedef int     IntType;
    static const IntType XEOF = -1;
};

// char �ػ�
template <>
struct CharTraits<char> {
    typedef char    CharType;
    typedef int     IntType;
    static const IntType XEOF = EOF;
};

// wchar_t �ػ�
template <>
struct CharTraits<wchar_t> {
    typedef wchar_t CharType;
    typedef wint_t  IntType;
    static const IntType XEOF = WEOF;
};

////////////////////////////////////////////////////////////////////////////////
// char/wchar_t ���ַ�/�ַ�����������ͳһ��ʾ
// ��;:
//   �ڽ��� CharType ��ģ���б����ַ�/�ַ�����������д��ͳһ
////////////////////////////////////////////////////////////////////////////////

// ��һ�㻯����
template <typename CharT>
const CharT* Str(const char* str, const wchar_t* wstr);
template <typename CharT>
const CharT Char(const char ch, const wchar_t wch);

// char �ػ�
template <>
inline const char* Str(const char* str, const wchar_t* wstr) {
    return str;
}
template <>
inline const char Char(const char ch, const wchar_t wch) {
    return ch;
}

// wchar_t �ػ�
template <>
inline const wchar_t* Str(const char* str, const wchar_t* wstr) {
    return wstr;
}
template <>
inline const wchar_t Char(const char ch, const wchar_t wch) {
    return wch;
}

#define SIMP_STR(type, str) Simp::Str<type>(str, SIMP_WIDE(str))
#define SIMP_CHAR(type, ch) Simp::Char<type>(ch, SIMP_WIDE(ch))

////////////////////////////////////////////////////////////////////////////////
// �ַ�����ʽ������
////////////////////////////////////////////////////////////////////////////////

// ת�� CRT vsprintf �ĺ���ģ��, ��һ�㻯����
template <typename CharT>
int vsprintf_t(CharT* buf, size_t charSize, const CharT* format, va_list args);

// vsprintf_t �� char �ػ�
template <>
inline
int vsprintf_t(char* buf, size_t charSize, const char* format, va_list args) {
    return vsprintf_s(buf, charSize, format, args);
}

// vsprintf_t �� wchar_t �ػ�
template <>
inline
int vsprintf_t(wchar_t* buf, size_t charSize, const wchar_t* format, va_list args) {
    return vswprintf_s(buf, charSize, format, args);
}

// ��ʽ������ string
template <size_t BufSize, typename CharT>
inline
std::basic_string<CharT> MakeString(const CharT* format, ...) {
    CharT buf[BufSize];
    va_list args;
    va_start(args, format);
    vsprintf_t(buf, BufSize, format, args);
    va_end(args);
    return std::basic_string<CharT>(buf);
}

////////////////////////////////////////////////////////////////////////////////
// CRT errno ������Ϣ
////////////////////////////////////////////////////////////////////////////////

// ת�� CRT strerror �ĺ���ģ��, ��һ�㻯����
template <typename CharT>
errno_t strerror_t(__out CharT* buf, size_t size, int errnum);

// char �ػ�
template <>
inline
errno_t strerror_t(__out char* buf, size_t size, int errnum) {
    return strerror_s(buf, size, errnum);
}

// wchar_t �ػ�
template <>
inline
errno_t strerror_t(__out wchar_t* buf, size_t size, int errnum) {
    return _wcserror_s(buf, size, errnum);
}

////////////////////////////////////////////////////////////////////////////////
// ��ģ�� CrtStrErr
// ��;:
//   ������ʱ�����������ʱ���ٻ���, �ڻ�� CRT errno ������Ϣʱ (strerror), ���ÿ��Ƿ�������ٻ�����
// ����:
//   _tprintf(_T("error %d: %s\n"), errno, CrtStrErr<BUF_SIZE>()(errno));
////////////////////////////////////////////////////////////////////////////////

#define SIMP_CRTSTRERRA(size, errnum)   Simp::CrtStrErr<size, char>()(errnum)
#define SIMP_CRTSTRERRW(size, errnum)   Simp::CrtStrErr<size, wchar_t>()(errnum)
#define SIMP_CRTSTRERR(size, errnum)    Simp::CrtStrErr<size, _TCHAR>()(errnum)

template <size_t Size, typename CharT>
struct CrtStrErr {

    CharT* operator()(int errnum) {
        strerror_t(Msg, Size, errnum);
        return Msg;
    }

    CharT Msg[Size];
};

SIMP_NS_END
