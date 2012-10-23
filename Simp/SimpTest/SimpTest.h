// SimpTest.h
//

#pragma once

////////////////////////////////////////////////////////////////////////////////
// ȫ����
////////////////////////////////////////////////////////////////////////////////

// ģ����
const char      MODULE_NAMEA[]  = "SimpTest";
const wchar_t   MODULE_NAMEW[]  = L"SimpTest";
const _TCHAR    MODULE_NAME[]   = _T("SimpTest");

// ���û�������С
const int BUF_SIZE = 256;

// ��־����
extern Simp::LogBase<char>*     LogA;
extern Simp::LogBase<wchar_t>*  LogW;
extern Simp::LogBase<_TCHAR>*   Log;

////////////////////////////////////////////////////////////////////////////////
// ���Թ���
////////////////////////////////////////////////////////////////////////////////

// �� "ģ����!������:" ��ʽ������Ա���
#define FMT(fmt)    SIMP_RPTFMT(fmt), MODULE_NAME
#define FMTA(fmt)   SIMP_RPTFMTA(fmt), MODULE_NAMEA

#define MY_RPT(rptType, fmt, ...)   SIMP_FMTRPT(MODULE_NAME, rptType, fmt, __VA_ARGS__)
#define MY_RPTA(rptType, fmt, ...)  SIMP_FMTRPTA(MODULE_NAMEA, rptType, fmt, __VA_ARGS__)

// ��ӡ������
#define PRINT_FILE          stdout
#define PRINT_FUNC          _ftprintf(PRINT_FILE, _T("----- : %s\n"), _T(__FUNCTION__))
#define PRINT_FUNC_BEGIN    _ftprintf(PRINT_FILE, _T("----- BEGIN: %s\n"), _T(__FUNCTION__))
#define PRINT_FUNC_END      _ftprintf(PRINT_FILE, _T("-----   END: %s\n"), _T(__FUNCTION__))

#define MY_CALL_RPT(rptType, ret, func, paramList)  SIMP_CALL_RPT(rptType, SIMP_MAKESTR(BUF_SIZE, _T("%s!") _T(__FUNCTION__) _T(": "), MODULE_NAME), ret, func, paramList)
