////////////////////////////////////////////////////////////////////////////////
// ��    �� : CSec.h
// ����˵�� :
//   Windows ���ٽ�������
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
// Windows �ٽ��������� CriticalSection
////////////////////////////////////////////////////////////////////////////////

class CriticalSection : private Uncopyable {
public:
    CriticalSection() {
        ZeroMemory(&m_CSec, sizeof(m_CSec));
    }

    BOOL Init(DWORD spinCnt = 0) {
        if (!InitializeCriticalSectionAndSpinCount(&m_CSec, spinCnt))
            return FALSE;
        return Lock();
    }

    BOOL Init(DWORD spinCnt, BOOL initOwn = TRUE) {
        if (!InitializeCriticalSectionAndSpinCount(&m_CSec, spinCnt))
            return FALSE;
        if (initOwn)
            return Lock();
        return TRUE;
    }

    ~CriticalSection() {
        Unlock();
        DeleteCriticalSection(&m_CSec);
    }

public:
    // Windows �ٽ������Լ��ı���, ��ֹ������
    // CAUTION: ����ٽ����������⵽����, ��������� EXCEPTION_POSSIBLE_DEADLOCK �� SEH �쳣
    BOOL Lock() {
        EnterCriticalSection(&m_CSec);
        return TRUE;
    }

    BOOL Unlock() {
        LeaveCriticalSection(&m_CSec);
        return TRUE;
    }

    BOOL TryLock() {
        return TryEnterCriticalSection(&m_CSec);
    }

    DWORD SetSpinCount(DWORD spin_count) {
        return SetCriticalSectionSpinCount(&m_CSec, spin_count);
    }

private:
    CRITICAL_SECTION m_CSec;
};

SIMP_NS_END
