////////////////////////////////////////////////////////////////////////////////
// ��    �� : Mutex.h
// ����˵�� :
//   Windows �Ļ�����
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
// Windows �������� Mutex
////////////////////////////////////////////////////////////////////////////////

class Mutex : private Uncopyable {
public:
    Mutex() : m_Mutex(NULL) {}

    // ��ʼ��������
    enum INIT_STAT {
        INIT_SUCCESS    = 0,
        INIT_EXISTED    = 1,
        INIT_FAILED     = -1
    };

    INIT_STAT Init() {
        m_Mutex = CreateMutex(NULL, TRUE, NULL);
        if (m_Mutex == NULL)
            return INIT_FAILED;
        return INIT_SUCCESS;
    }

    INIT_STAT Init(const _TCHAR* mutexName, BOOL initOwn = TRUE, LPSECURITY_ATTRIBUTES secAttr = NULL) {
        m_Mutex = CreateMutex(secAttr, initOwn, mutexName);
        if (m_Mutex == NULL)
            return INIT_FAILED;
        if (GetLastError() == ERROR_ALREADY_EXISTS)
            return INIT_EXISTED;
        return INIT_SUCCESS;
    }

    ~Mutex() {
        Unlock();
        CloseHandle(m_Mutex);
    }

public:
    // Lock ����������
    enum LOCK_STAT {
        LOCK_SUCCESS    = WAIT_OBJECT_0,
        LOCK_ABANDONED  = WAIT_ABANDONED,
        LOCK_TIMEOUT    = WAIT_TIMEOUT
    };

    // timeout �� millisec Ϊ��λ, Windows ���������Լ��ı�����ֹ������
    LOCK_STAT Lock(DWORD timeout) {
        return (LOCK_STAT) WaitForSingleObject(m_Mutex, timeout);
    }

    BOOL Lock() {
        return (Lock(INFINITE) == LOCK_SUCCESS);
    }

    BOOL Unlock() {
        return ReleaseMutex(m_Mutex);
    }

private:
    HANDLE m_Mutex;
};

SIMP_NS_END
