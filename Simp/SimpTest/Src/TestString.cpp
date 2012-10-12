// TestString.cpp
//

#include "StdAfx.h"
#include "SimpTest.h"
#include "TestString.h"

////////////////////////////////////////////////////////////////////////////////
// ���ֿռ�
////////////////////////////////////////////////////////////////////////////////

using Simp::tstring;
using Simp::tcin;
using Simp::tcout;
using Simp::tcerr;
using Simp::ToTchar;

////////////////////////////////////////////////////////////////////////////////
// ȫ�ֱ���
////////////////////////////////////////////////////////////////////////////////

// ����ȫ�� locale ����
static _TCHAR OldLocale[BUF_SIZE];

////////////////////////////////////////////////////////////////////////////////
// ���Ը���
////////////////////////////////////////////////////////////////////////////////

// ���������ǰ�� locale
// �� callFunc = 0 ʱ, ��� module!report_current_locale: ǰ׺
// �� callFunc != 0 ʱ, ��� module!callFunc: ǰ׺
void ReportCurrentLocale(const _TCHAR* callFunc, BOOL turnOn);

// ����ȫ�� locale, ���� locale �����浽 oldLoc
void SetGlobalLocale(const _TCHAR* loc, __out _TCHAR* oldLoc, size_t bufSize, const _TCHAR* callFunc, BOOL reportOn);

// ����ȫ�� locale, ���� locale �����浽ȫ�ֱ��� OldLocale
#define SET_GLOBAL_LOCALE(loc, reportOn) \
    SetGlobalLocale(loc, OldLocale, _countof(OldLocale), _T(__FUNCTION__), reportOn)

// �ָ� locale
void RestoreGlobalLocale(_TCHAR* loc, const _TCHAR* callFunc, BOOL reportOn);

// �ָ� locale Ϊȫ�ֱ��� OldLocale
#define RESTORE_GLOBAL_LOCALE(reportOn) \
    RestoreGlobalLocale(OldLocale, _T(__FUNCTION__), reportOn)

////////////////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////////////////

void TestMbsToWcs(BOOL turnOn);

////////////////////////////////////////////////////////////////////////////////
// ���Ժ���
////////////////////////////////////////////////////////////////////////////////

void TestString(BOOL turnOn)
{
    SIMP_OFF_DO(turnOn, return);

    TestMbsToWcs(TRUE);
}

////////////////////////////////////////////////////////////////////////////////
// ���Ը���
////////////////////////////////////////////////////////////////////////////////

void ReportCurrentLocale(const _TCHAR* callFunc, BOOL turnOn)
{
    SIMP_OFF_DO(turnOn, return);

    const _TCHAR* loc = _tsetlocale(LC_ALL, 0);
    _ASSERTE(loc != 0);

    if (callFunc == NULL)
        MY_RPT(_CRT_WARN, "current locale: %s", loc);
    else
        MY_RPT(_CRT_WARN, "%s: current locale: %s", callFunc, loc);
}

void SetGlobalLocale(const _TCHAR* loc, __out _TCHAR* oldLoc, size_t bufSize, const _TCHAR* callFunc, BOOL reportOn)
{
    ReportCurrentLocale(callFunc, reportOn);
    _tcscpy_s(oldLoc, bufSize, _tsetlocale(LC_ALL, NULL));
    _tsetlocale(LC_ALL, loc);
    ReportCurrentLocale(callFunc, reportOn);
}

void RestoreGlobalLocale(_TCHAR* loc, const _TCHAR* callFunc, BOOL reportOn)
{
    _tsetlocale(LC_ALL, loc);
    ReportCurrentLocale(callFunc, reportOn);
}

////////////////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////////////////

// ���� sim_mbstowcs(), sim_alloc_mbstowcs()
void TestMbsToWcs(BOOL turnOn)
{
    SIMP_OFF_DO(turnOn, return);

    char mbstr[] = "abc���ַ�123";
    const size_t WCSTR_BUF_SIZE = 5;
    wchar_t wcstrBuf[WCSTR_BUF_SIZE] = {0};
    wchar_t* wcstr = NULL;
    errno_t err = 0;

    SET_GLOBAL_LOCALE(_T(""), FALSE);   // ����ȫ�� locale

    // sim_mbstowcs() �Ĳ��� loc = 0 ʱ, ��ȫ�� locale (setlocale()) �½���ת��
    MY_CALL_RPT(_CRT_WARN, err, Simp::MbsToWcs, (mbstr, wcstrBuf, _countof(wcstrBuf)));
    MY_CALL_RPT(_CRT_WARN, err, Simp::MbsToWcsNew, (mbstr, wcstr));

    free(wcstr);

    RESTORE_GLOBAL_LOCALE(FALSE);       // �ָ� locale
}
