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

////////////////////////////////////////////////////////////////////////////////
// ȫ����
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

void TestToTchar_01(BOOL turnOn);
void TestToTchar_02(BOOL turnOn);
void TestMbsToWcs(BOOL turnOn);
void TestCrtStrError(BOOL turnOn);

////////////////////////////////////////////////////////////////////////////////
// ���Ժ���
////////////////////////////////////////////////////////////////////////////////

void TestString(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);

    TestToTchar_01(FALSE);
    TestToTchar_02(FALSE);
    TestMbsToWcs(FALSE);
    TestCrtStrError(TRUE);
}

////////////////////////////////////////////////////////////////////////////////
// ���Ը���
////////////////////////////////////////////////////////////////////////////////

void ReportCurrentLocale(const _TCHAR* callFunc, BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);

    const _TCHAR* loc = _tsetlocale(LC_ALL, 0);
    _ASSERTE(loc != 0);

    if (callFunc == NULL)
        MY_RPT(_CRT_WARN, "current locale: %s", loc);
    else
        MY_RPT(_CRT_WARN, "%s: current locale: %s", callFunc, loc);
}

void SetGlobalLocale(const _TCHAR* loc, __out _TCHAR* oldLoc, size_t bufSize, const _TCHAR* callFunc, BOOL reportOn) {
    ReportCurrentLocale(callFunc, reportOn);
    _tcscpy_s(oldLoc, bufSize, _tsetlocale(LC_ALL, NULL));
    _tsetlocale(LC_ALL, loc);
    ReportCurrentLocale(callFunc, reportOn);
}

void RestoreGlobalLocale(_TCHAR* loc, const _TCHAR* callFunc, BOOL reportOn) {
    _tsetlocale(LC_ALL, loc);
    ReportCurrentLocale(callFunc, reportOn);
}

////////////////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////////////////

// ���� sim_mbstowcs(), sim_alloc_mbstowcs()
void TestMbsToWcs(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

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

    PRINT_FUNC_END;
}

void TestToTchar_01(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    _TCHAR msgExcept[BUF_SIZE];
    _TCHAR msgType[BUF_SIZE];

    try {
        throw std::domain_error("test ToTchar");
    }
    catch (std::exception& e) {
        _tprintf(_T("Exception: what: %s, type: %s\n"),
                 Simp::ToTchar(e.what(), msgExcept, _countof(msgExcept)),
                 Simp::ToTchar(typeid(e).name(), msgType, _countof(msgType)));
    }

    PRINT_FUNC_END;
}

void TestToTchar_02(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    _tprintf(_T("ANSI to Tchar: %s\n")
             _T("UCS to Tchar: %s\n"),
             SIMP_TCHAR(BUF_SIZE, "ANSI string."),
             SIMP_TCHAR(BUF_SIZE, L"UCS string."));

    PRINT_FUNC_END;
}

void TestCrtStrError(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    FILE* stream;
    errno_t err = _tfopen_s(&stream, _T("NOExistFile.txt"), _T("r"));
    if (err == 0)
        fclose(stream);
    else
        _tprintf(_T("File open error %d: %s\n"), errno, SIMP_CRTSTRERR(BUF_SIZE, errno));

    PRINT_FUNC_END;
}

