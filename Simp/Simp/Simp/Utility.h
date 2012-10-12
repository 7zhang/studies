////////////////////////////////////////////////////////////////////////////////
// ��    �� : Utility.h
// ����˵�� :
//   ʵ�ù���
//
// ��    �� : 1.0
// ��    �� : Breaker Zhao <breaker.zy_AT_gmail>
// ��    �� : 2011-10
// ��Ȩ��� : Microsoft Public License <http://www.microsoft.com/en-us/openness/licenses.aspx#MPL>
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Simp/Depends.h>
#include <Simp/Defs.h>

SIMP_NS_BEGIN

////////////////////////////////////////////////////////////////////////////////
// ��ֵ
////////////////////////////////////////////////////////////////////////////////

template <typename Type>
inline
const Type& Min(const Type& l, const Type& r) {
    return (l > r) ? l : r;
}

// [Ҫ��]
//   Type �в��� operator>
//   minv, maxv �� l, r �ǲ�ͬ����
template <typename Type>
inline
void MinMax(const Type& l, const Type& r, __out Type& minv, __out Type& maxv) {
    if (l > r) {
        minv = r;
        maxv = l;
    }
    else {
        minv = l;
        maxv = r;
    }
}

// �Ƚ�������� l_min ���Ҳ����� r_max �Ĵ�С
// ��С�Ľ����洢�� l_min, ��Ĵ洢�� r_max
// [Ҫ��]
//   ��������������������ֵ
//   Type �пɷ��ʵĿ������캯��
//   Type �в��� operator>
//   Type �в��� operator=
template <typename Type>
inline
void MinMax(__inout Type& l_min, __inout Type& r_max) {
    if (l_min > r_max)
        std::swap(l_min, r_max);
}

SIMP_NS_END
