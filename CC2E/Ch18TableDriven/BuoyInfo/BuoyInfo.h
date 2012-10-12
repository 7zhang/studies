// BuoyInfo.h
//

#ifndef __BUOYINFO_H__
#define __BUOYINFO_H__

////////////////////////////////////////////////////////////////////////////////
// ȫ����
////////////////////////////////////////////////////////////////////////////////

// ����ģ����
#undef MODULE_NAME
#define MODULE_NAME     "BuoyInfo"

// ���û�������С
#define BUF_SIZE        512

// errno ���û�����ֵ����
#define ERR_USER        128

////////////////////////////////////////////////////////////////////////////////
// ���Թ���
////////////////////////////////////////////////////////////////////////////////

// �� "ģ����!������:" ��ʽ������Ա���
#define FMT(fmt)    _T(MODULE_NAME) _T("!") _T(__FUNCTION__) _T(": ")  _T(fmt) _T("\n")
#define FMTA(fmt)   MODULE_NAME "!" __FUNCTION__ ": " fmt "\n"

#define MY_RPT(rptType, fmt, ...)   SIMP_RPT(rptType, FMT(fmt), __VA_ARGS__)
#define MY_RPTA(rptType, fmt, ...)  SIMP_RPTA(rptType, FMTA(fmt), __VA_ARGS__)

// ��ӡ������
#define PRINT_FILE          stdout
#define PRINT_FUNC          _ftprintf(PRINT_FILE, _T("----- : %s\n"), _T(__FUNCTION__))
#define PRINT_FUNC_BEGIN    _ftprintf(PRINT_FILE, _T("----- BEGIN: %s\n"), _T(__FUNCTION__))
#define PRINT_FUNC_END      _ftprintf(PRINT_FILE, _T("-----   END: %s\n"), _T(__FUNCTION__))

#endif  // __BUOYINFO_H__
