////////////////////////////////////////////////////////////////////////////////
// ��    �� : LogDbg.h
// ����˵�� :
//   Windows �����������־��ʩ
//
// ��    �� : 1.0
// ��    �� : Breaker Zhao <breaker.zy_AT_gmail>
// ��    �� : 2011-10
// ��Ȩ��� : Microsoft Public License <http://www.microsoft.com/en-us/openness/licenses.aspx#MPL>
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Simp/Depends.h>
#include <Simp/Defs.h>

#include <Simp/Debug/DbgOut.h>
#include <Simp/String/StrHelper.h>
#include <Simp/Log/LogBase.h>

SIMP_NS_BEGIN

////////////////////////////////////////////////////////////////////////////////
// Windows ���������־����ʩ��ģ�� LogDebugOutT
////////////////////////////////////////////////////////////////////////////////

template <typename CharT>
class LogDebugOutT : public LogBaseT<CharT> {
public:
    explicit
    LogDebugOutT(size_t bufSize, LOG_LEVEL baseLevel) : LogBaseT<CharT>(baseLevel), m_Buf(NULL), m_BufSize(0) {
        SetBufSize(bufSize);
    }

    virtual ~LogDebugOutT() {
        Destroy();
    }

    virtual void Destroy() {
        delete[] m_Buf;
        m_Buf = NULL;
        m_BufSize = 0;
    }

public:
    virtual void DoLog(const CharT* format, va_list args) {
        vsprintf_t(m_Buf, m_BufSize, format, args);
        OutputDebug(m_Buf);
    }

public:
    size_t GetBufSize() const {
        return m_BufSize;
    }

    void SetBufSize(size_t bufSize) {
        CharT* oldBuf = m_Buf;
        m_Buf = new CharT[bufSize];
        delete oldBuf;
        m_BufSize = bufSize;
    }

private:
    size_t  m_BufSize;
    CharT*  m_Buf;
};

////////////////////////////////////////////////////////////////////////////////
// LogDebugOutT �ĵ������󴴽�����
////////////////////////////////////////////////////////////////////////////////

template <typename CharT>
inline
LogBaseT<CharT>* DebugOutLogT(LOG_LEVEL baseLevel, size_t sizeBuf) {
    static LogDebugOutT<CharT> log(sizeBuf, baseLevel);
    return &log;
}

inline
LogBaseT<_TCHAR>* DebugOutLog(LOG_LEVEL baseLevel, size_t sizeBuf) {
    return DebugOutLogT<_TCHAR>(baseLevel, sizeBuf);
}

SIMP_NS_END
