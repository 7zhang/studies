////////////////////////////////////////////////////////////////////////////////
// ��    �� : Singleton.h
// ����˵�� :
//   ����ģʽ��
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
// ������ģ�� Singleton
////////////////////////////////////////////////////////////////////////////////

// �򵥵ĵ�����, ʹ�þֲ���̬�洢, ������ǰ�ͷ� (�������ǰ), ��һ�γ�ʼ�����̰߳�ȫ����
//
// [Ҫ��]
//   �û����� Type �пɷ��ʵ�Ĭ�Ϲ��캯��
template <typename _Type>
struct Singleton : private Uncopyable {
    typedef _Type Type;

    static Type& Inst() {
        static Type obj;
        return obj;
    }
};

SIMP_NS_END
