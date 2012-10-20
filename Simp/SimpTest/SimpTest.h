// SimpTest.h
//

#pragma once

////////////////////////////////////////////////////////////////////////////////
// ȫ����
////////////////////////////////////////////////////////////////////////////////

// ����ģ����
#undef MODULE_NAME
#define MODULE_NAME     "SimpTest"

// ���û�������С
#define BUF_SIZE        512

// ��־����
extern Simp::LogBaseT<char>*    LogA;
extern Simp::LogBaseT<wchar_t>* LogW;
extern Simp::LogBaseT<_TCHAR>*  Log;

////////////////////////////////////////////////////////////////////////////////
// ���Թ���
////////////////////////////////////////////////////////////////////////////////

// �� "ģ����!������:" ��ʽ������Ա���
#define FMT(fmt)    SIMP_RPTFMT(MODULE_NAME, fmt)
#define FMTA(fmt)   SIMP_RPTFMTA(MODULE_NAME, fmt)

#define MY_RPT(rptType, fmt, ...)   SIMP_FMTRPT(MODULE_NAME, rptType, fmt, __VA_ARGS__)
#define MY_RPTA(rptType, fmt, ...)  SIMP_FMTRPTA(MODULE_NAME, rptType, fmt, __VA_ARGS__)

// ��ӡ������
#define PRINT_FILE          stdout
#define PRINT_FUNC          _ftprintf(PRINT_FILE, _T("----- : %s\n"), _T(__FUNCTION__))
#define PRINT_FUNC_BEGIN    _ftprintf(PRINT_FILE, _T("----- BEGIN: %s\n"), _T(__FUNCTION__))
#define PRINT_FUNC_END      _ftprintf(PRINT_FILE, _T("-----   END: %s\n"), _T(__FUNCTION__))

#define MY_CALL_RPT(rptType, ret, func, paramList)  SIMP_CALL_RPT(rptType, _T(MODULE_NAME) _T("!") _T(__FUNCTION__) _T(": "), ret, func, paramList)
