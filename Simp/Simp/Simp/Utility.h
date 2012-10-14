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
    return (l < r ? l : r);
}

template <typename Type>
inline
const Type& Max(const Type& l, const Type& r) {
    return (l > r ? l : r);
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

// �Ƚ�������� lmin ���Ҳ����� rmax �Ĵ�С
// ��С�Ľ����洢�� lmin, ��Ĵ洢�� rmax
// [Ҫ��]
//   ��������������������ֵ
//   Type �пɷ��ʵĿ������캯��
//   Type �в��� operator>
//   Type �в��� operator=
template <typename Type>
inline
void MinMax(__inout Type& lmin, __inout Type& rmax) {
    if (lmin > rmax)
        std::swap(lmin, rmax);
}

SIMP_NS_END
