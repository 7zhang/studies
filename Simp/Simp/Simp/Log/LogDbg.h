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
// Windows ���������־����ʩ��ģ�� LogDebugOut
////////////////////////////////////////////////////////////////////////////////

template <typename CharT>
class LogDebugOut : public LogBase<CharT> {
public:
    explicit
    LogDebugOut(size_t bufSize, LOG_LEVEL baseLevel) : LogBase<CharT>(baseLevel), m_Buf(NULL), m_BufSize(0) {
        SetBufSize(bufSize);
    }

    virtual ~LogDebugOut() {
        Destroy();
    }

    virtual void Destroy() {
        delete[] m_Buf;
        m_Buf = NULL;
        m_BufSize = 0;
    }

public:
    virtual void DoLog(const CharT* fmt, va_list args) {
        vsprintf_t(m_Buf, m_BufSize, fmt, args);
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

    // �������󴴽�����
    static LogDebugOut<CharT>* Inst(LOG_LEVEL baseLevel, size_t sizeBuf) {
        static LogDebugOut<CharT> log(sizeBuf, baseLevel);
        return &log;
    }

private:
    size_t  m_BufSize;
    CharT*  m_Buf;
};

SIMP_NS_END
