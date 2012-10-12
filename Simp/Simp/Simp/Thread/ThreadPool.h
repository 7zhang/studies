////////////////////////////////////////////////////////////////////////////////
// ��    �� : ThreadPool.h
// ����˵�� :
//   Windows �̳߳�
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
#include <Simp/Memory.h>

SIMP_NS_BEGIN

////////////////////////////////////////////////////////////////////////////////
// Windows �̳߳��� ThreadPool
////////////////////////////////////////////////////////////////////////////////

class ThreadPool : private Uncopyable {
public:
    typedef std::vector<LPVOID> ThreadParamVec;
    typedef std::vector<HANDLE> ThreadHandleVec;

public:
    // ׼�� threadNum ������Ϊ thread_proc ���߳�, �̲߳����� param[0 ~ threadNum-1] ����
    ThreadPool(LPTHREAD_START_ROUTINE threadProc, LPVOID* param, size_t threadNum = 1) {
        _ASSERTE(threadProc != NULL);
        _ASSERTE(param != NULL);
        _ASSERTE(threadNum != 0);

        m_ThreadProc = threadProc;
        for (size_t i = 0; i < threadNum; i++) {
            m_ThreadParamVec.push_back(param[i]);
            m_ThreadHandleVec.push_back(NULL);
        }
    }

    ~ThreadPool() {
        CloseThread();
    }

    void CloseThread() {
        ThreadHandleVec::iterator i = m_ThreadHandleVec.begin();
        for (; i != m_ThreadHandleVec.end(); ++i)
            Delete(*i, HCLOSE_CZ);
    }

    // �������̵߳����в���
    void AddThread(LPVOID* param, size_t threadNum = 1) {
        _ASSERTE(param != NULL);
        _ASSERTE(threadNum != 0);

        for (size_t i = 0; i < threadNum; i++) {
            m_ThreadParamVec.push_back(param[i]);
            m_ThreadHandleVec.push_back(NULL);
        }
    }

    BOOL StartThread(DWORD flag = 0, SIZE_T stackSize = 0, LPSECURITY_ATTRIBUTES secAttr = NULL) {
#ifdef _DEBUG
        SIMP_FALSE_DO(ValidateThreadParam(), return FALSE);
#endif
        size_t threadNum = m_ThreadParamVec.size();
        for (size_t i = 0; i < threadNum; ++i) {
            m_ThreadHandleVec[i] = ::CreateThread(secAttr, stackSize, m_ThreadProc, m_ThreadParamVec[i], flag, NULL);
            if (m_ThreadHandleVec[i] == NULL)
                FALSE;
        }
        return TRUE;
    }

    BOOL ValidateThreadParam() {
        size_t threadNum = m_ThreadParamVec.size();
        return (threadNum == m_ThreadHandleVec.size());
    }

    void ResumeThread() {
#ifdef _DEBUG
        SIMP_FALSE_DO(ValidateThreadParam(), return);
#endif
        size_t threadNum = m_ThreadParamVec.size();
        for (size_t i = 0; i < threadNum; ++i)
            ::ResumeThread(m_ThreadHandleVec[i]);
    }

    ThreadHandleVec& GetThreadHandleVec() {
        return m_ThreadHandleVec;
    }

    DWORD WaitAllThread(DWORD timeOut) {
        DWORD theadNum = (DWORD) m_ThreadHandleVec.size();
        if (theadNum > MAXIMUM_WAIT_OBJECTS)
            return -1;
        return WaitForMultipleObjects(theadNum, &m_ThreadHandleVec[0], TRUE, timeOut);
    }

private:
    LPTHREAD_START_ROUTINE  m_ThreadProc;
    ThreadParamVec          m_ThreadParamVec;
    ThreadHandleVec         m_ThreadHandleVec;
};

SIMP_NS_END
