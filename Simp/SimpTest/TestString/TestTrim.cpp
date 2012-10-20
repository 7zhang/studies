// TestTrim.cpp
//

#include "StdAfx.h"
#include "SimpTest.h"
#include "TestString.h"

////////////////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////////////////

void TestTrimLeft_01(BOOL turnOn);
void TestTrimLeft_02(BOOL turnOn);
void TestTrimRight_01(BOOL turnOn);
void TestTrimRight_02(BOOL turnOn);
void TestTrim_01(BOOL turnOn);
void TestTrim_02(BOOL turnOn);

void TestIsSpace(BOOL turnOn);
void TestCRTStrCpy(BOOL turnOn);

////////////////////////////////////////////////////////////////////////////////
// ���Ժ���
////////////////////////////////////////////////////////////////////////////////

void TestTrim(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);

    TestCRTStrCpy(FALSE);
    TestIsSpace(FALSE);
    TestTrimLeft_01(FALSE);
    TestTrimLeft_02(FALSE);
    TestTrimRight_01(FALSE);
    TestTrimRight_02(FALSE);
    TestTrim_01(FALSE);
    TestTrim_02(TRUE);
}

void TestCRTStrCpy(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    _TCHAR dst[8];
    const _TCHAR* srcs[] = {
        // NULL,
        _T(""),
        _T("1234567"),
        _T("12345678"),
    };

    // ��Դ�ַ�����Ŀ�껺�����ص�ʱ��strcpy_s��strncpy_s ����Ϊ���޶����
    // The behavior of strcpy_s is undefined if the source and destination strings overlap.
    // The behavior of strncpy_s is undefined if the source and destination strings overlap.

#if 0
    // ��������̫Сʱ _tcscpy_s ���� Assert�������ش����룬�������� Parameter Validation Ϊ����ִ��
    for (size_t i = 0; i < _countof(srcs); i++) {
        errno_t err = _tcscpy_s(dst, _countof(dst), srcs[i]);
        if (err != 0)
            _tperror(_T("_tcscpy_s"));

        _tprintf(_T("%d. Dest: [%s]\n"), i, dst);
    }
#endif

    // ��������̫Сʱ _tcsncpy_s ʹ�� _TRUNCATE �ضϳ������������ַ���
    // ������ _TRUNCATE ʱ��_tcsncpy_s �� _tcscpy_s ���ƣ����� Assert
    for (size_t i = 0; i < _countof(srcs); i++) {
        errno_t err = _tcsncpy_s(dst, _countof(dst), srcs[i], _TRUNCATE);
        if (err != 0) {

            // VC2005 ���ԣ�_tcserror ������ʶ�� STRUNCATE ������ (80)���õ���Ӧ�Ĵ���������ֻ�ܵõ� Unknown error
            // err = EINVAL;        // 22
            // err = STRUNCATE;     // 80

            // _tcserror Ϊ�̲߳���ȫ���������� C4996 ���棬ʹ�� _CRT_SECURE_NO_WARNINGS �رվ���
            // _ftprintf(stderr, _T("_tcscpy_s: %s (%d)\n"), _tcserror(err), err);

            _TCHAR msg[100];
            _tcserror_s(msg, _countof(msg), err);
            _ftprintf(stderr, _T("_tcscpy_s: %s (%d)\n"), msg, err);
        }

        _tprintf(_T("%d. Dest: [%s]\n"), i, dst);
    }

    PRINT_FUNC_END;
}

void TestIsSpace(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    _tprintf(_T("Current Locale: %s\n"), _tsetlocale(LC_ALL, NULL));
    _TCHAR ws[] = _T(" \f\n\r\t\v��");

    // NOTE:
    // ���ü������� locale = chs ʱ��_istspace ��ʶ������ȫ�ǿո�Ϊ�հ��ַ� (Unicode=0x3000)
    // _istspace ��ʶ�� \0 Ϊ�հ��ַ�

    size_t sz = _countof(ws);
    for (size_t i = 0; i < sz; i++)
        _tprintf(_T("%d. Character (%04X): %s\n"), i, ws[i], (_istspace(ws[i]) ? _T("white space") : _T("non white space")));

    PRINT_FUNC_END;
}

void TestTrimLeft_01(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    _TCHAR dst[8];
    const _TCHAR* srcs[] = {
        NULL,
        _T(""),
        _T("  \t��"),
        _T("  \t��123   "),
        _T("  \t��1234567"),
        _T("  \t��12345678")
    };

    for (size_t i = 0; i < _countof(srcs); i++) {
        errno_t err = Simp::TrimLeftAlert(srcs[i], dst, _countof(dst));

        if (err == 0)
            _tprintf(_T("%d. Trimed: [%s]\n"), i, dst);
        else if (err == STRUNCATE)
            _tprintf(_T("%d. Trimed: [%s] Truncated\n"), i, dst);
        else
            _tprintf(_T("%d. Trim failed\n"), i);
    }

    PRINT_FUNC_END;
}

void TestTrimLeft_02(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    _TCHAR* srcs[] = {
        NULL,
        _tcsdup(_T("")),
        _tcsdup(_T("  \t��")),
        _tcsdup(_T("  \t��123   ")),
        _tcsdup(_T("  \t��1234567")),
        _tcsdup(_T("  \t��12345678"))
    };

    for (size_t i = 0; i < _countof(srcs); i++) {
        errno_t err = Simp::TrimLeftAlert(srcs[i]);

        if (err == 0)
            _tprintf(_T("%d. Trimed: [%s]\n"), i, srcs[i]);
        else if (err == STRUNCATE)
            _tprintf(_T("%d. Trimed: [%s] Truncated\n"), i, srcs[i]);
        else
            _tprintf(_T("%d. Trim failed\n"), i);
    }

    for (size_t i = 0; i < _countof(srcs); i++)
        free(srcs[i]);

    PRINT_FUNC_END;
}

void TestTrimRight_01(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    _TCHAR dst[8];
    const _TCHAR* srcs[] = {
        NULL,
        _T(""),
        _T("  \t��"),
        _T("   123  \t��"),
        _T("1234567  \t��"),
        _T("12345678  \t��")
    };

    for (size_t i = 0; i < _countof(srcs); i++) {
        errno_t err = Simp::TrimRightAlert(srcs[i], dst, _countof(dst));

        if (err == 0)
            _tprintf(_T("%d. Trimed: [%s]\n"), i, dst);
        else if (err == STRUNCATE)
            _tprintf(_T("%d. Trimed: [%s] Truncated\n"), i, dst);
        else
            _tprintf(_T("%d. Trim failed\n"), i);
    }

    PRINT_FUNC_END;
}

void TestTrimRight_02(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    _TCHAR* srcs[] = {
        NULL,
        _tcsdup(_T("")),
        _tcsdup(_T("  \t��")),
        _tcsdup(_T("   123  \t��")),
        _tcsdup(_T("1234567  \t��")),
        _tcsdup(_T("12345678  \t��"))
    };

    for (size_t i = 0; i < _countof(srcs); i++) {
        errno_t err = Simp::TrimRightAlert(srcs[i]);

        if (err == 0)
            _tprintf(_T("%d. Trimed: [%s]\n"), i, srcs[i]);
        else if (err == STRUNCATE)
            _tprintf(_T("%d. Trimed: [%s] Truncated\n"), i, srcs[i]);
        else
            _tprintf(_T("%d. Trim failed\n"), i);
    }

    for (size_t i = 0; i < _countof(srcs); i++)
        free(srcs[i]);

    PRINT_FUNC_END;
}

void TestTrim_01(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    _TCHAR dst[8];
    const _TCHAR* srcs[] = {
        NULL,
        _T(""),
        _T("  \t��"),
        _T("  \t��123   "),
        _T("  \t��1234567"),
        _T("  \t��12345678"),
        _T("   123  \t��"),
        _T("1234567  \t��"),
        _T("12345678  \t��"),
        _T("  \t��12   67  \t��"),
        _T("  \t��1234567  \t��"),
        _T("  \t��12345678  \t��")
    };

    for (size_t i = 0; i < _countof(srcs); i++) {
        errno_t err = Simp::TrimAlert(srcs[i], dst, _countof(dst));

        if (err == 0)
            _tprintf(_T("%d. Trimed: [%s]\n"), i, dst);
        else if (err == STRUNCATE)
            _tprintf(_T("%d. Trimed: [%s] Truncated\n"), i, dst);
        else
            _tprintf(_T("%d. Trim failed\n"), i);
    }

    PRINT_FUNC_END;
}

void TestTrim_02(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    _TCHAR* srcs[] = {
        NULL,
        _tcsdup(_T("")),
        _tcsdup(_T("  \t��")),
        _tcsdup(_T("  \t��123   ")),
        _tcsdup(_T("  \t��1234567")),
        _tcsdup(_T("  \t��12345678")),
        _tcsdup(_T("   123  \t��")),
        _tcsdup(_T("1234567  \t��")),
        _tcsdup(_T("12345678  \t��")),
        _tcsdup(_T("  \t��12   67  \t��")),
        _tcsdup(_T("  \t��1234567  \t��")),
        _tcsdup(_T("  \t��12345678  \t��"))
    };

    for (size_t i = 0; i < _countof(srcs); i++) {
        errno_t err = Simp::TrimAlert(srcs[i]);

        if (err == 0)
            _tprintf(_T("%d. Trimed: [%s]\n"), i, srcs[i]);
        else if (err == STRUNCATE)
            _tprintf(_T("%d. Trimed: [%s] Truncated\n"), i, srcs[i]);
        else
            _tprintf(_T("%d. Trim failed\n"), i);
    }

    for (size_t i = 0; i < _countof(srcs); i++)
        free(srcs[i]);

    PRINT_FUNC_END;
}
