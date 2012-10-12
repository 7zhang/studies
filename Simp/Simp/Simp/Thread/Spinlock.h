////////////////////////////////////////////////////////////////////////////////
// ��    �� : Spinlock.h
// ����˵�� :
//   ������
//
// ��    �� : 1.0
// ��    �� : Breaker Zhao <breaker.zy_AT_gmail>
// ��    �� : 2011-10
// ��Ȩ��� : Microsoft Public License <http://www.microsoft.com/en-us/openness/licenses.aspx#MPL>
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Simp/Depends.h>
#include <Simp/Defs.h>

#include <Simp/Pattern/Uncopyable.h>

SIMP_NS_BEGIN

////////////////////////////////////////////////////////////////////////////////
// �������� SpinLock
////////////////////////////////////////////////////////////////////////////////

class SpinLock : private Uncopyable {
public:
    typedef void (*YieldFunc)();
    typedef long LOCKTYPE;

    enum LOCK_STAT {
        LOCK_UNLOCKED   = 0,    // ������ 0, �Ա� lock �е� _InterlockedExchange ���� 0
        LOCK_LOCKED     = 1
    };

public:
    // ʹ���ڲ� lock_
    SpinLock() : m_Lock(LOCK_UNLOCKED), m_YieldFunc(DefYield) {
        m_LockSlot = &m_Lock;
        Lock();
    }

    // ʹ���û��ṩ�� lck
    SpinLock(LOCKTYPE* lock, BOOL initOwn = TRUE, YieldFunc yieldFunc = DefYield) : m_LockSlot(lock), m_YieldFunc(yieldFunc) {
        _ASSERTE(lock != NULL);
        *lock = LOCK_UNLOCKED;
        if (initOwn)
            Lock();
    }

    ~SpinLock() {
        Unlock();
    }

public:
    BOOL Lock() {
        while (_InterlockedExchange(m_LockSlot, LOCK_LOCKED))
            m_YieldFunc();
        return TRUE;
    }

    BOOL Unlock() {
        _InterlockedExchange(m_LockSlot, LOCK_UNLOCKED);
        return TRUE;
    }

private:
    static void DefYield() {
        Sleep(0);
    }

private:
    LOCKTYPE    m_Lock;      // �ڲ� lock
    LOCKTYPE*   m_LockSlot;
    YieldFunc   m_YieldFunc; // �ó����Ⱥ���
};

SIMP_NS_END
