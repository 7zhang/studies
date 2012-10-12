////////////////////////////////////////////////////////////////////////////////
// FILE         : TestPeterson.cpp
// DESCRIPTION  :
//   Example of Peterson's algorithm for synchronization in concurrent threads.
//
// AUTHOR       : Breaker Zhao <breaker.zy_AT_gmail>
// DATE         : 2011-10
// LICENSE      : Microsoft Public License <http://www.microsoft.com/en-us/openness/licenses.aspx#MPL>
//
// DISCLAIMER   :
//   THIS CODE IS FOR DEMONSTRATIVE PURPOSES ONLY AND SHOULD NOT BE USED IN THE FINAL PRODUCT.
//   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
//   EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
//   WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TestPeterson.h"

////////////////////////////////////////////////////////////////////////////////
// ���ֿռ�
////////////////////////////////////////////////////////////////////////////////

using Simp::LogBaseT;
namespace LOG_LVL = Simp::LOG_LVL;

namespace S = std;
namespace SI = Simp;

using Simp::tcin;
using Simp::tcout;
using Simp::tcerr;

////////////////////////////////////////////////////////////////////////////////
// ȫ����
////////////////////////////////////////////////////////////////////////////////

// ԭ���Ĺ���Ŀ¼
_TCHAR* OldWorkDir;

// ��־����
LogBaseT<char>*     LogA;
LogBaseT<wchar_t>*  LogW;
LogBaseT<_TCHAR>*   Log;

////////////////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////////////////

void TestPeterson_01(BOOL turnOn);
void TestAddThreadNoLock(BOOL turnOn);
void TestAddThreadPeterson2(BOOL turnOn);
void TestAddThreadPetersonN(BOOL turnOn);

////////////////////////////////////////////////////////////////////////////////
// ��ʼ��������
////////////////////////////////////////////////////////////////////////////////

void Init();
void Uninit();

////////////////////////////////////////////////////////////////////////////////
// �������
////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
    Init();

    TestPeterson_01(TRUE);

    Uninit();
    _putts(_T("Press any key to Exit."));
    _gettch();
    return 0;
}

// ��ʼ�����л���
void Init()
{
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
void Uninit()
{
    // �ָ�ԭ���Ĺ���Ŀ¼
    _tchdir(OldWorkDir);
    free(OldWorkDir);

    // �ֹ��ͷ���־����
    LogA->Destroy();
    LogW->Destroy();

    // ��Ϊ��̬������ Uninit ֮������, ������Ҫ�ֹ��ͷź���̬�洢�ľ�̬����, �����������
    _CrtDumpMemoryLeaks();
}

////////////////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////////////////

// shared global variable for self-increment (++SharedVal) within multi-threads
int SharedVal;

void TestPeterson_01(BOOL turnOn)
{
    SIMP_OFF_DO(turnOn, return);
    TestAddThreadNoLock(FALSE);
    TestAddThreadPeterson2(FALSE);
    TestAddThreadPetersonN(TRUE);
}

////////////////////////////////////////////////////////////////////////////////
// ��ͬ����ʽ (����): ������ Data Race
////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI AddThreadNoLock(LPVOID param)
{
    int count = (int) (intptr_t) param;
    for (int i = 0; i < count; ++i)
        ++SharedVal;
    return 0;
}

BOOL StartAddThreadNoLock(int threadNum)
{
    const int ADD_NUM_MIN = 10000;
    const int ADD_NUM_MAX = 30000;

    int i = 0;
    int sum = 0;
    SharedVal = 0;

    // ��ʼ�� ThreadPool �е��߳�
    S::vector<LPVOID> threadParam(threadNum);
    for (i = 0; i < threadNum; ++i) {
        threadParam[i] = (LPVOID) (intptr_t) (int) (((double) rand() / (double) RAND_MAX) * (ADD_NUM_MAX - ADD_NUM_MIN) + ADD_NUM_MIN);
        sum += (int) (intptr_t) threadParam[i];
    }
    _tprintf(_T("sum: %d\n"), sum);

    SI::ThreadPool threadPool(AddThreadNoLock, &threadParam[0], threadNum);

    // ���� ThreadPool �е��߳�
    if (!threadPool.StartThread())
        MY_RPT(_CRT_ERROR, "ThreadPool::StartThread failed");

    // �ȴ������߳̽���
    _ASSERTE(threadNum <= MAXIMUM_WAIT_OBJECTS);
    if (threadPool.WaitAllThread(INFINITE) == WAIT_FAILED)
        Log->Log(LOG_LVL::LEVEL_ERROR, FMT("ThreadPool::WaitAllThread failed: %d"), GetLastError());

    _tprintf(_T("shared val: %d\n"), SharedVal);

    return (SharedVal == sum);
}

void TestAddThreadNoLock(BOOL turnOn)
{
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    srand((unsigned) time(NULL));

    const int MAX_ROUND = 500;
    int i = 0;
    for (; i < MAX_ROUND; ++i) {
        _tprintf(_T("----- Round %d\n"), i);
        if (!StartAddThreadNoLock(MAXIMUM_WAIT_OBJECTS)) {
            _tprintf(_T("hit singularity!\n"));
            Beep(750, 400);
            break;
        }
    }
    if (i == MAX_ROUND)
        _tprintf(_T("pass all round = %d\n"), i);

    PRINT_FUNC_END;
}

////////////////////////////////////////////////////////////////////////////////
// ʹ�� Peterson2 ͬ������
////////////////////////////////////////////////////////////////////////////////

struct Peterson2 {
    void Init(int id, LPVOID param) {
        m_Id = id;
        m_Param = param;
        m_Turn = 1 - id;
    }

    static void Reset() {
        Flag[0] = FALSE;
        Flag[1] = FALSE;
        Turn = 0;
    }

    template <BOOL UseMemoryBarrier>
    void Lock() {
        Flag[m_Id] = TRUE;
        if (UseMemoryBarrier)
            MemoryBarrier();
        Turn = m_Turn;
        while (Flag[m_Turn] && Turn == m_Turn)  // busy wait
            YieldProcessor();
    }

    void Unlock() {
        Flag[m_Id] = FALSE;
    }

    int     m_Id;
    int     m_Turn;
    LPVOID  m_Param;

    static volatile BOOL    Flag[2];
    static volatile int     Turn;
};

volatile BOOL Peterson2::Flag[2];
volatile int Peterson2::Turn;

template <BOOL UseMemoryBarrier>
DWORD WINAPI AddThreadPeterson2(LPVOID param)
{
    _ASSERTE(param != NULL);
    Peterson2* pet = (Peterson2*) param;
    int count = (int) (intptr_t) pet->m_Param;

    for (int i = 0; i < count; ++i) {
        pet->Lock<UseMemoryBarrier>();
        ++SharedVal;    // Peterson2 critical section
        pet->Unlock();
    }
    return 0;
}

template <BOOL UseMemoryBarrier>
BOOL StartAddThreadPeterson2()
{
    const int ADD_NUM_MIN = 10000;
    const int ADD_NUM_MAX = 30000;

    int i = 0;
    int sum = 0;
    SharedVal = 0;
    Peterson2::Reset(); // IMPORTANT

    // ��ʼ�� ThreadPool �е��߳�
    LPVOID threadParam[2];
    Peterson2 pet[2];
    for (i = 0; i < 2; ++i) {
        int cnt = (int) (((double) rand() / (double) RAND_MAX) * (ADD_NUM_MAX - ADD_NUM_MIN) + ADD_NUM_MIN);
        pet[i].Init(i, (LPVOID) (intptr_t) cnt);
        sum += cnt;
        threadParam[i] = &pet[i];
    }
    _tprintf(_T("sum: %d\n"), sum);

    SI::ThreadPool threadPool(AddThreadPeterson2<UseMemoryBarrier>, threadParam, 2);

    // ���� ThreadPool �е��߳�
    if (!threadPool.StartThread())
        MY_RPT(_CRT_ERROR, "ThreadPool::StartThread failed");

    // �ȴ������߳̽���
    if (threadPool.WaitAllThread(INFINITE) == WAIT_FAILED)
        Log->Log(LOG_LVL::LEVEL_ERROR, FMT("ThreadPool::WaitAllThread failed: %d"), GetLastError());

    _tprintf(_T("shared val: %d\n"), SharedVal);

    return (SharedVal == sum);
}

void TestAddThreadPeterson2(BOOL turnOn)
{
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    srand((unsigned) time(NULL));

    const int MAX_ROUND = 500;
    int i = 0;
    for (; i < MAX_ROUND; ++i) {
        _tprintf(_T("----- Round %d\n"), i);
        if (!StartAddThreadPeterson2<TRUE>()) {
            _tprintf(_T("hit singularity!\n"));
            Beep(750, 400);
            break;
        }
    }
    if (i == MAX_ROUND)
        _tprintf(_T("pass all round = %d\n"), i);

    PRINT_FUNC_END;
}

////////////////////////////////////////////////////////////////////////////////
// ʹ�� PetersonN ͬ������
////////////////////////////////////////////////////////////////////////////////

struct PetersonN {
    void Init(int id, LPVOID param) {
        m_Id = id;
        m_Param = param;
    }

    static void Reset(int threadNum) {
        Clean();
        ThreadNum = threadNum;

        Flag = new int[threadNum + 1];
        ZeroMemory((void*) Flag, (threadNum + 1) * sizeof(int));

        Turn = new int[threadNum];
        ZeroMemory((void*) Turn, threadNum * sizeof(int));
    }

    static void Clean() {
        ThreadNum = 0;
        delete[] Flag;
        Flag = NULL;
        delete[] Turn;
        Turn = NULL;
    }

    template <BOOL UseMemoryBarrier>
    void Lock() {
        for (int i = 1; i <= ThreadNum - 1; ++i) {
            Flag[m_Id] = i;
            if (UseMemoryBarrier)
                MemoryBarrier();
            Turn[i] = m_Id;
            for (int j = 1; j <= ThreadNum; ++j) {
                if (j == m_Id)
                    continue;
                while (Flag[j] >= i && Turn[i] == m_Id)
                    YieldProcessor();
            }
        }
    }

    void Unlock() {
        Flag[m_Id] = 0;
    }

    int     m_Id;
    LPVOID  m_Param;

    static volatile int*    Flag;
    static volatile int*    Turn;
    static int              ThreadNum;
};

volatile int* PetersonN::Flag = NULL;
volatile int* PetersonN::Turn = NULL;
int PetersonN::ThreadNum;

template <BOOL UseMemoryBarrier>
DWORD WINAPI AddThreadPetersonN(LPVOID param)
{
    _ASSERTE(param != NULL);
    PetersonN* pet = (PetersonN*) param;
    int count = (int) (intptr_t) pet->m_Param;

    for (int i = 0; i < count; ++i) {
        pet->Lock<UseMemoryBarrier>();
        ++SharedVal;    // PetersonN critical section
        pet->Unlock();
    }
    return 0;
}

template <BOOL UseMemoryBarrier>
BOOL StartAddThreadPetersonN(int threadNum)
{
    const int ADD_NUM_MIN = 10000;
    const int ADD_NUM_MAX = 30000;

    int i = 0;
    int sum = 0;
    SharedVal = 0;
    PetersonN::Reset(threadNum);

    // ��ʼ�� ThreadPool �е��߳�
    S::vector<LPVOID> threadParam(threadNum);
    S::vector<PetersonN> pet(threadNum);
    for (i = 0; i < threadNum; ++i) {
        int cnt = (int) (((double) rand() / (double) RAND_MAX) * (ADD_NUM_MAX - ADD_NUM_MIN) + ADD_NUM_MIN);
        pet[i].Init(i + 1, (LPVOID) (intptr_t) cnt);    // NOTE: first thread id is 1, not 0
        sum += cnt;
        threadParam[i] = &pet[i];
    }
    _tprintf(_T("sum: %d\n"), sum);

    SI::ThreadPool threadPool(AddThreadPetersonN<UseMemoryBarrier>, &threadParam[0], threadNum);

    // ���� ThreadPool �е��߳�
    if (!threadPool.StartThread())
        MY_RPT(_CRT_ERROR, "ThreadPool::StartThread failed");

    // �ȴ������߳̽���
    _ASSERTE(threadNum <= MAXIMUM_WAIT_OBJECTS);
    if (threadPool.WaitAllThread(INFINITE) == WAIT_FAILED)
        Log->Log(LOG_LVL::LEVEL_ERROR, FMT("ThreadPool::WaitAllThread failed: %d"), GetLastError());

    _tprintf(_T("shared val: %d\n"), SharedVal);

    return (SharedVal == sum);
}

void TestAddThreadPetersonN(BOOL turnOn)
{
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    srand((unsigned) time(NULL));

    const int MAX_ROUND = 500;
    const int THREAD_NUM = 4;
    int i = 0;
    for (; i < MAX_ROUND; ++i) {
        _tprintf(_T("----- Round %d\n"), i);
        if (!StartAddThreadPetersonN<FALSE>(THREAD_NUM)) {
            _tprintf(_T("hit singularity!\n"));
            Beep(750, 400);
            break;
        }
    }
    if (i == MAX_ROUND)
        _tprintf(_T("pass all round = %d\n"), i);

    PetersonN::Clean();
    PRINT_FUNC_END;
}
