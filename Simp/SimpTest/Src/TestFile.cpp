// TestFile.cpp
//

#include "StdAfx.h"
#include "SimpTest.h"
#include "TestFile.h"

////////////////////////////////////////////////////////////////////////////////
// ���ֿռ�
////////////////////////////////////////////////////////////////////////////////

using Simp::tstring;
using Simp::tcin;
using Simp::tcout;
using Simp::tcerr;
using Simp::ToTchar;

using Simp::StdioFile;

////////////////////////////////////////////////////////////////////////////////
// ȫ�ֱ���
////////////////////////////////////////////////////////////////////////////////

enum ACCESS_MODE {
    AC_EXIST    = 0,
    AC_WRITE    = 2,
    AC_READ     = 4
};

const _TCHAR* FILE_ENCODING[] = {_T("ANSI"), _T("UTF-8"), _T("UTF-16LE"), _T("UNICODE")};

////////////////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////////////////

void TestReadText(BOOL turnOn);

////////////////////////////////////////////////////////////////////////////////
// ���Ժ���
////////////////////////////////////////////////////////////////////////////////

void TestStdio(BOOL turnOn)
{
    SIMP_OFF_DO(turnOn, return);
    TestReadText(TRUE);
}

////////////////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////////////////

// ANSI, UTF-8, UTF-16 (UCS-2) �����ļ���ȡ
void TestReadText(BOOL turnOn)
{
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    int i;
    int e;
    _TCHAR fname[BUF_SIZE];
    _TCHAR mode[BUF_SIZE];

    // ȡ���ļ���
    while (TRUE) {
        _tprintf(_T("file name: "));
        _getts_s(fname, _countof(fname));
        if (_taccess_s(fname, AC_READ) == 0)
            break;
        _tperror(fname);
    }

    // ȡ���ļ�����
    while (TRUE) {
        _tprintf(_T("ecoding:"));
        for (i = 0; i < _countof(FILE_ENCODING); i++)
            _tprintf(_T(" %d. %s"), i + 1, FILE_ENCODING[i]);
        _tprintf(_T(": "));
        _getts_s(mode, _countof(mode));
#pragma warning(push)
#pragma warning(disable: 4996)
        if (_stscanf(mode, _T("%d"), &e) == 1 && e >= 1 && e <= _countof(FILE_ENCODING))
#pragma warning(pop)
            break;
    }
    e--;

    // TEST:
    _tprintf(_T("file name: %s\n"), fname);
    _tprintf(_T("encoding: %s\n"), FILE_ENCODING[e]);

    _stprintf_s(mode, _countof(mode), _T("r, ccs=%s"), FILE_ENCODING[e]);
    StdioFile file;
    file.Open(fname, mode);

    // ��ȡ�ļ���
    _TCHAR* line;
    _TCHAR  buf[BUF_SIZE];
    for (i = 0; TRUE; i++) {
        line = file.Gets(buf, _countof(buf));   // gets ������β�� \n
        if (line == NULL)
            break;
        _tprintf(_T("%04d|%s"), i + 1, line);
    }

    if (file.Eof())
        _tprintf(_T("\nread file end\n"));
    else if (file.Error()) {
#pragma warning(push)
#pragma warning(disable: 4996)
        _ftprintf(stderr, _T("\nread file error: %s (%d)\n"), _tcserror(errno), errno);
#pragma warning(pop)
    }

    PRINT_FUNC_END;
}
