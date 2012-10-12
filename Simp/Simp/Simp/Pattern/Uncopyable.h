////////////////////////////////////////////////////////////////////////////////
// ��    �� : Uncopyable.h
// ����˵�� :
//   ��ֹ��������
//
// ��    �� : 1.0
// ��    �� : Breaker Zhao <breaker.zy_AT_gmail>
// ��    �� : 2011-10
// ��Ȩ��� : Microsoft Public License <http://www.microsoft.com/en-us/openness/licenses.aspx#MPL>
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Simp/Defs.h>

SIMP_NS_BEGIN

////////////////////////////////////////////////////////////////////////////////
// ��ֹ�������� Uncopyable
////////////////////////////////////////////////////////////////////////////////

class Uncopyable {
protected:
    Uncopyable() {}
    ~Uncopyable() {}
private:
    // ��ֹ����
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};

SIMP_NS_END
