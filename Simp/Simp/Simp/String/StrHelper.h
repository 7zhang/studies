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

SIMP_NS_END
