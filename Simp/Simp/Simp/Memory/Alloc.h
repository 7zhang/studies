////////////////////////////////////////////////////////////////////////////////
// ��    �� : Alloc.h
// ����˵�� :
//   �ڴ����
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
// �ڴ���亯��
////////////////////////////////////////////////////////////////////////////////

// Simp ��ͨ���ڴ���亯��, Ĭ��ת�ӵ� CRT �� malloc/free
// Simp �ڲ���ʹ�����з��亯��, �����û������ж��� SIMP_OVERRIDE_ALLOC �Ը������з��亯��

#ifndef SIMP_OVERRIDE_ALLOC

inline
void* Malloc(size_t size) {
    return malloc(size);
}

inline
void* Realloc(void* memblock, size_t size) {
    return realloc(memblock, size);
}

inline
void* Calloc(size_t num, size_t size) {
    return calloc(num, size);
}

inline
void Free(void* memblock) {
    free(memblock);
}

#endif  // SIMP_OVERRIDE_ALLOC NOT defined

////////////////////////////////////////////////////////////////////////////////
// �ڴ�������� Mallocator
////////////////////////////////////////////////////////////////////////////////

// ���� Malloc �ķ�������
template <typename _Type, BOOL ThrowExcept = FALSE>
class Mallocator {
public:
    typedef _Type Type;

    Mallocator() {}

    // count �� Type ����, ������ byte ��
    Type* Alloc(size_t count) {
        Type* p = (Type*) Malloc(count * sizeof(Type));
        if (ThrowExcept && p == NULL)
            throw std::bad_alloc(__FUNCTION__ " failed");
        return p;
    }

    Type* Realloc(Type* p, size_t count) {
        Type* q = (Type*) Simp::Realloc(p, count * sizeof(Type));
        if (ThrowExcept && q == NULL && count != 0)
            throw std::bad_alloc(__FUNCTION__ " failed");
        return q;
    }

    void Free(Type* p) {
        Simp::Free(p);
    }

    // �� p ������ val ֵ, ����ʽ new
    void Construct(Type* p, const Type& val) {
        new(p) Type(val);
    }

    // ���� Type ����, �����ͷŶ�����Ĵ洢
    void Destroy(Type* p) {
        p->~Type();
    }
};

SIMP_NS_END
