// SimpTest.cpp
//

#include "StdAfx.h"
#include "SimpTest.h"
#include "TestString.h"
#include "TestMemory.h"
#include "TestDebug.h"
#include "TestThread.h"
#include "TestLog.h"
#include "TestFile.h"

////////////////////////////////////////////////////////////////////////////////
// ���ֿռ�
////////////////////////////////////////////////////////////////////////////////

using Simp::LogBaseT;
namespace LOG_LVL = Simp::LOG_LVL;

using Simp::tcin;
using Simp::tcout;
using Simp::tcerr;

////////////////////////////////////////////////////////////////////////////////
// ȫ�ֱ���
////////////////////////////////////////////////////////////////////////////////

// ԭ���Ĺ���Ŀ¼
static _TCHAR* OldWorkDir;

// ��־����
LogBaseT<char>*     LogA;
LogBaseT<wchar_t>*  LogW;
LogBaseT<_TCHAR>*   Log;

////////////////////////////////////////////////////////////////////////////////
// ��ʼ����������
////////////////////////////////////////////////////////////////////////////////

void Init();
void Uninit();

////////////////////////////////////////////////////////////////////////////////
// ������ں���
////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[]) {
    Init();

    try {
        TestString(FALSE);
        TestMemory(TRUE);
        TestDbgout(FALSE);
        TestThread(FALSE);
        TestLog(FALSE);
        TestStdio(FALSE);
    }
    catch (std::exception& e) {
        LogA->Log(LOG_LVL::LEVEL_ERROR, FMTA("std::exception: what: %s, type: %s"), e.what(), typeid(e).name());
    }
    catch (...) {
        Log->Log(LOG_LVL::LEVEL_ERROR, FMT("unknown exception"));
    }

    Uninit();
    _putts(_T("Press any key to Exit."));
    _gettch();
    return 0;
}

// ��ʼ�����л���
void Init() {
#define _USE_CRT_LOCALE

#if _MSC_VER > 1400
#define _USE_IOS_LOCALE
#endif

    // CRT ȫ�� locale �� iostream imbue locale ��ͻ
    // VC 2005 ���� (_MSC_VER = 1400)
    // VC 2010 ���� (_MSC_VER = 1600)
#if _MSC_VER <= 1400 && defined(_USE_CRT_LOCALE) && defined(_USE_IOS_LOCALE)
#error cannot use CRT global locale and iostream imbue locale at the same time, when _MSC_VER <= 1400
#endif

#ifdef _USE_CRT_LOCALE
    _tsetlocale(LC_ALL, _T(""));
#endif

#ifdef _USE_IOS_LOCALE
    // ���� C++ iostream ��׼ IO �� locale
    std::locale loc(std::locale(""), std::locale::classic(), std::locale::numeric);
    tcout.imbue(loc);
    tcerr.imbue(loc);
    tcin.imbue(loc);
#endif

    // ���� CRT �������
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);

    // ���õ�ǰ exe �ļ���Ŀ¼Ϊ����Ŀ¼
    _TCHAR modDir[BUF_SIZE];
    OldWorkDir = _tgetcwd(NULL, 0);
    Simp::GetModuleDir(GetModuleHandle(NULL), modDir, BUF_SIZE);
    _tchdir(modDir);

    // ������־����
    LogA = Simp::DebugOutLogT<char>(LOG_LVL::LEVEL_DEBUG, BUF_SIZE);
    LogW = Simp::DebugOutLogT<wchar_t>(LOG_LVL::LEVEL_DEBUG, BUF_SIZE);
#ifdef _UNICODE
    Log = LogW;
#else
    Log = LogA;
#endif
}

// �������л���
void Uninit() {
    // �ָ�ԭ���Ĺ���Ŀ¼
    _tchdir(OldWorkDir);
    free(OldWorkDir);

    // �ֹ��ͷ���־����
    LogA->Destroy();
    LogW->Destroy();

    // ��̬������ Uninit ֮������, ��Ҫ�ֹ��ͷź���̬�洢�ľ�̬����, �����������
    _CrtDumpMemoryLeaks();
}
