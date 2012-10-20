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

using Simp::StdioFile;

////////////////////////////////////////////////////////////////////////////////
// ȫ����
////////////////////////////////////////////////////////////////////////////////

enum ACCESS_MODE {
    AC_EXIST    = 0,
    AC_WRITE    = 2,
    AC_READ     = 4
};

const int FILE_ENCODING_NOSPEC = 0;
const _TCHAR* FILE_ENCODING[] = {_T("[NOSPEC]"), _T("ANSI"), _T("UTF-8"), _T("UTF-16LE"), _T("UNICODE")};

////////////////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////////////////

void ReadText_01(BOOL turnOn);
void ReadLineText_01(BOOL turnOn);

////////////////////////////////////////////////////////////////////////////////
// ���Ժ���
////////////////////////////////////////////////////////////////////////////////

void TestReadLine(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    ReadLineText_01(TRUE);
}

void TestStdio(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    ReadText_01(TRUE);
}

////////////////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////////////////

// ���� Simp::ReadLine
void ReadLineText_01(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    _TCHAR fname[BUF_SIZE];

    // ȡ���ļ���
    while (TRUE) {
        _tprintf(_T("File name: "));
        _getts_s(fname, _countof(fname));
        if (_taccess_s(fname, AC_READ) == 0)
            break;
        _tperror(fname);
    }

    // TEST:
    _tprintf(_T("File name: %s\n"), fname);

    StdioFile file;
    file.Open(fname, _T("r"));

    // ʹ�� Simp::ReadLine ��ȡ�ļ���
    Simp::ReadLine<_TCHAR> readLine;
    readLine.SetBufSize(8);

    int i = 0;
    // Simp::ReadLine ���� CRT gets, ���Ժ���β \n
    while (readLine.Read(file.Stream()) == 0)
        _tprintf(_T("%04d|%s"), i++, readLine.Line());

    if (file.Eof())
        _putts(_T("\n----- Read File End -----"));
    else if (file.Error()) {
#pragma warning(push)
#pragma warning(disable: 4996)
        _ftprintf(stderr, _T("\nRead file error: %s (%d)\n"), _tcserror(errno), errno);
#pragma warning(pop)
    }

    PRINT_FUNC_END;
}

// ANSI, UTF-8, UTF-16 (UCS-2) �����ļ���ȡ
void ReadText_01(BOOL turnOn) {
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    int i;
    int e;
    _TCHAR fname[BUF_SIZE];
    _TCHAR mode[BUF_SIZE];

    // ȡ���ļ���
    while (TRUE) {
        _tprintf(_T("File name: "));
        _getts_s(fname, _countof(fname));
        if (_taccess_s(fname, AC_READ) == 0)
            break;
        _tperror(fname);
    }

    // ȡ���ļ�����
    while (TRUE) {
        _tprintf(_T("Ecoding:"));
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
    _tprintf(_T("File name: %s\n"), fname);
    _tprintf(_T("Encoding: %s\n"), FILE_ENCODING[e]);

    // VC 2005 ���Խ��:
    // ccs=ANSI ��������, ���� Assert. Ҫ��д ANSI �����ļ�, �벻Ҫָ���κ� css= ����
    // ccs=UTF-8 ��������д UTF-8 (NO BOM) �����ļ�
    // ccs=UTF-16LE ��������д UTF-16LE (NO BOM) �� Unicode (NO BOM) �����ļ�
    // ccs=UNICODE ��д Unicode (NO BOM) �����ļ�ʱ����

    if (e == FILE_ENCODING_NOSPEC)
        _stprintf_s(mode, _countof(mode), _T("r"));
    else
        _stprintf_s(mode, _countof(mode), _T("r, ccs=%s"), FILE_ENCODING[e]);
    StdioFile file;
    file.Open(fname, mode);

    // ��ȡ�ļ���
    _TCHAR* line;
    _TCHAR  buf[BUF_SIZE];
    for (i = 0; TRUE; i++) {
        line = file.Gets(buf, _countof(buf));   // gets ����β \n
        if (line == NULL)
            break;
        _tprintf(_T("%04d|%s"), i + 1, line);
    }

    if (file.Eof())
        _putts(_T("\n----- Read File End -----"));
    else if (file.Error()) {
#pragma warning(push)
#pragma warning(disable: 4996)
        _ftprintf(stderr, _T("\nRead file error: %s (%d)\n"), _tcserror(errno), errno);
#pragma warning(pop)
    }

    PRINT_FUNC_END;
}
