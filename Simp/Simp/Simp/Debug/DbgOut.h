////////////////////////////////////////////////////////////////////////////////
// ��    �� : DbgOut.h
// ����˵�� :
//   �����������
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

#ifdef _DEBUG

////////////////////////////////////////////////////////////////////////////////
// ����������������� SIMP_RPT
//

// char ��
#define SIMP_RPTA(rptType, fmt, ...) \
    (void) ((1 != _CrtDbgReport(rptType, 0, 0, 0, fmt, __VA_ARGS__)) || \
    (_CrtDbgBreak(), 0))

#define SIMP_RPTFA(rptType, fmt, ...) \
    (void) ((1 != _CrtDbgReport(rptType, __FILE__, __LINE__, 0, fmt, __VA_ARGS__)) || \
    (_CrtDbgBreak(), 0))

// wchar_t ��
#define SIMP_RPTW(rptType, fmt, ...) \
    (void) ((1 != _CrtDbgReportW(rptType, 0, 0, 0, fmt, __VA_ARGS__)) || \
    (_CrtDbgBreak(), 0))

#define SIMP_RPTFW(rptType, fmt, ...) \
    (void) ((1 != _CrtDbgReportW(rptType, SIMP_WIDE(__FILE__), __LINE__, 0, fmt, __VA_ARGS__)) || \
    (_CrtDbgBreak(), 0))

// _TCHAR ��
#ifdef _UNICODE
#define SIMP_RPT    SIMP_RPTW
#define SIMP_RPTF   SIMP_RPTFW
#else
#define SIMP_RPT    SIMP_RPTA
#define SIMP_RPTF   SIMP_RPTFA
#endif

//
////////////////////////////////////////////////////////////////////////////////

// ���ú��� func(paramList), ���� SIMP_RPT ����䷵��ֵ
//
// [Ҫ��]
//   1. paramList �����Ű�����, �� (p1, p2)
//   2. func �ķ���ֵ����������, �� ��ʾ����ִ�н����״ֵ̬, ������
#define SIMP_CALL_RPT(rptType, prefix, ret, func, paramList) { \
        ret = func##paramList; \
        SIMP_RPT(rptType, prefix _T("%s: return=%d\n"), SIMP_STRINGIZET(func), ret); \
    }

////////////////////////////////////////////////////////////////////////////////
// Assert ����
// ����������������� Assert
// ʹ�� !!, ����ʹ�� expr �ıȽ������ (operator==), ���߿�����Ϊ���ض�������, Ӱ�� || ��������
//

// char ��
#define SIMP_ASSERTEA(expr, fmt, ...) \
    (void) ((!!(expr)) || \
    (1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, 0, fmt, __VA_ARGS__)) || \
    (_CrtDbgBreak(), 0))

// wchar_t ��
#define SIMP_ASSERTEW(expr, fmt, ...) \
    (void) ((!!(expr)) || \
    (1 != _CrtDbgReportW(_CRT_ASSERT, SIMP_WIDE(__FILE__), __LINE__, 0, fmt, __VA_ARGS__)) || \
    (_CrtDbgBreak(), 0))

// _TCHAR ��
#ifdef _UNICODE
#define SIMP_ASSERT SIMP_ASSERTEW
#else
#define SIMP_ASSERT SIMP_ASSERTEA
#endif

//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// ���һ���ڴ��Ƿ��ܹ���д
// �ܹ�����ʱ���� SIMP_RPT �������, ���ܷ���ʱ���� Assert
//

#define SIMP_MEMORY_READ            0
#define SIMP_MEMORY_READ_WRITE      1
#define SIMP_MEMORY_ACCESS_MODES    2

// NOTE:
// _CrtIsValidPointer ֻ�� Debug �����¶���
// VC 2005 �� _CrtIsValidPointer ʵ��ֻ�Ǽ��ж�һ�µ�ַ�Ƿ�Ϊ 0, ��û��
// ����ַ�ռ���ڴ�����, ����ĵ�ַ���� size �ͷ��ʷ�ʽ access ��û����

#define SIMP_ASSERT_MEMORY(rptType, prefix, addr, Size, access) { \
        if (access != SIMP_MEMORY_READ && access != SIMP_MEMORY_READ_WRITE) \
            SIMP_RPT(_CRT_ERROR, prefix _T("can't support such memory access method\n")); \
        else { \
            if (_CrtIsValidPointer(addr, (unsigned int) Size, access)) \
                SIMP_RPT(rptType, prefix _T("access memory passed: addr=0x%08X, size=%d, access=%s\n"), \
                           addr, Size, (access == SIMP_MEMORY_READ ? _T("read") : _T("read write"))); \
            else \
                SIMP_ASSERT_EXPR(0, _T("\n") prefix _T("access memory failed: addr=0x%08X, size=%d, access=%s\n"), \
                                 addr, Size, (access == SIMP_MEMORY_READ ? _T("read") : _T("read write"))); \
        } \
    }

//
////////////////////////////////////////////////////////////////////////////////

#define SIMP_DEBUG_ONLY(expr)   expr    // ֻ�� Debug ����Ч�����
#define SIMP_RELEASE_ONLY(expr) 0       // ֻ�� Release ����Ч�����

#else
// _DEBUG NOT defined

#define SIMP_RPTA(rptType, fmt, ...)    0
#define SIMP_RPTFA(rptType, fmt, ...)   0
#define SIMP_RPTW(rptType, fmt, ...)    0
#define SIMP_RPTFW(rptType, fmt, ...)   0
#define SIMP_RPT(rptType, fmt, ...)     0
#define SIMP_RPTF(rptType, fmt, ...)    0

#define SIMP_CALL_RPT(rptType, prefix, ret, func, paramList)    0

#define SIMP_ASSERT_EXPR(expr, fmt, ...)                        0

#define SIMP_ASSERT_MEMORY(rptType, prefix, addr, Size, access) 0

#define SIMP_DEBUG_ONLY(expr)       0
#define SIMP_RELEASE_ONLY(expr)     expr
#endif

// �� "ģ����!������:" ��ʽ������Ա���
#define SIMP_RPTFMT(module, fmt)    _T(module) _T("!") _T(__FUNCTION__) _T(": ")  _T(fmt) _T("\n")
#define SIMP_RPTFMTA(module, fmt)   module "!" __FUNCTION__ ": " fmt "\n"

#define SIMP_FMTRPT(module, rptType, fmt, ...)  SIMP_RPT(rptType, SIMP_RPTFMT(module, fmt), __VA_ARGS__)
#define SIMP_FMTRPTA(module, rptType, fmt, ...) SIMP_RPTA(rptType, SIMP_RPTFMTA(module, fmt), __VA_ARGS__)

////////////////////////////////////////////////////////////////////////////////
// Windows API �������
////////////////////////////////////////////////////////////////////////////////

// ת�� Windows API OutputDebugString �ĺ���ģ��, ��һ�㻯����
template <typename CharT>
void OutputDebug(const CharT* str);

// OutputDebug �� char �ػ�
template <>
inline
void OutputDebug(const char* str) {
    OutputDebugStringA(str);
}

// OutputDebug �� wchar_t �ػ�
template <>
inline
void OutputDebug(const wchar_t* str) {
    OutputDebugStringW(str);
}

SIMP_NS_END
