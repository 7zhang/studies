// TestThread.cpp
//

#include "StdAfx.h"
#include "SimpTest.h"
#include "TestThread.h"

////////////////////////////////////////////////////////////////////////////////
// ���ֿռ�
////////////////////////////////////////////////////////////////////////////////

namespace LOG_LVL = Simp::LOG_LVL;

using Simp::tstring;
using Simp::tcin;
using Simp::tcout;
using Simp::tcerr;
using Simp::ToTchar;

using Simp::ThreadPool;

////////////////////////////////////////////////////////////////////////////////
// ȫ�ֱ���
////////////////////////////////////////////////////////////////////////////////

static int SharedVal;

////////////////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////////////////

void TestThreadPool_01(BOOL turnOn);
void TestSelfLock(BOOL turnOn);

////////////////////////////////////////////////////////////////////////////////
// ���Ժ���
////////////////////////////////////////////////////////////////////////////////

void TestThread(BOOL turnOn)
{
    SIMP_OFF_DO(turnOn, return);
    TestThreadPool_01(TRUE);
    TestSelfLock(FALSE);
}

////////////////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////////////////

namespace My {
//typedef Simp::NoLock LockType;
//typedef Simp::Mutex LockType;
//typedef Simp::CriticalSection LockType;
typedef Simp::SpinLock LockType;

// ������
inline LockType& GetLock()
{
    static Simp::SpinLock::LOCKTYPE lockSlot;
    static Simp::SpinLock lock(&lockSlot, FALSE);
    return lock;
}

}   // namespace My

// �߳����� add_num() �Ĳ���
struct AddNum {
    int* val;
    int count;
    SIMP_DEBUG_ONLY(int magic);
    SIMP_DEBUG_ONLY(static const int MAGIC = 3141592);
};

// ѭ������������ +n �� 1 ���߳�
DWORD WINAPI add_num(LPVOID param)
{
    _ASSERTE(param != 0);
    _ASSERTE(((AddNum*) param)->val != 0);
    _ASSERTE(((AddNum*) param)->magic == AddNum::MAGIC);

    int* val = ((AddNum*) param)->val;
    int count = ((AddNum*) param)->count;

    for (int i = 0; i < count; i++) {
        My::GetLock().Lock();
        (*val)++;
        My::GetLock().Unlock();
    }

    return 0;
}

void TestThreadPool_01(BOOL turnOn)
{
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    const int THREAD_NUM = MAXIMUM_WAIT_OBJECTS;
    const int ADD_NUM_MIN = 10000;
    const int ADD_NUM_MAX = 30000;

    int i;
    int sum = 0;
    AddNum* addnum[THREAD_NUM];

    srand((unsigned) time(NULL));
    SharedVal = 0;

    // ��ʼ�� ThreadPool �е��̲߳���

    for (i = 0; i < THREAD_NUM; i++) {
        addnum[i] = new AddNum;
        SIMP_DEBUG_ONLY(addnum[i]->magic = AddNum::MAGIC);
        addnum[i]->val = &SharedVal;
        addnum[i]->count = (int) (((double) rand() / (double) RAND_MAX) * (ADD_NUM_MAX - ADD_NUM_MIN) + ADD_NUM_MIN);
        sum += addnum[i]->count;
    }
    ThreadPool threadPool(add_num, (void**) addnum, THREAD_NUM);
    _tprintf(_T("sum: %d\n"), sum);

    // ���� ThreadPool �е��߳�
    if (!threadPool.StartThread()) {
        MY_RPT(_CRT_ERROR, "ThreadPool::Run failed");
        goto _FREE;
    }

    // �ȴ������߳̽���
    _ASSERTE(THREAD_NUM <= MAXIMUM_WAIT_OBJECTS);
    if (threadPool.WaitAllThread(INFINITE) == WAIT_FAILED)
        Log->Log(LOG_LVL::LEVEL_ERROR, FMT("ThreadPool::WaitAllThread failed: %d"), GetLastError());

    _tprintf(_T("shared val: %d\n"), SharedVal);

_FREE:
    for (i = 0; i < THREAD_NUM; i++)
        delete addnum[i];

    PRINT_FUNC_END;
}

void TestSelfLock(BOOL turnOn)
{
    SIMP_OFF_DO(turnOn, return);
    PRINT_FUNC_BEGIN;

    BOOL ret;
    ret = My::GetLock().Lock();
    ret = My::GetLock().Lock();

    PRINT_FUNC_END;
}
