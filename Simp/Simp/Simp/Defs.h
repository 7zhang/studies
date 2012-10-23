////////////////////////////////////////////////////////////////////////////////
// ��    �� : Defs.h
// ����˵�� :
//   �������ͺ͹���
//
// ��    �� : 1.0
// ��    �� : Breaker Zhao <breaker.zy_AT_gmail>
// ��    �� : 2011-10
// ��Ȩ��� : Microsoft Public License <http://www.microsoft.com/en-us/openness/licenses.aspx#MPL>
////////////////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Simp ������ֿռ�
////////////////////////////////////////////////////////////////////////////////

#define SIMP_NS_BEGIN   namespace Simp {
#define SIMP_NS_END     }

SIMP_NS_BEGIN

////////////////////////////////////////////////////////////////////////////////
// Simp ��İ汾
////////////////////////////////////////////////////////////////////////////////

#define SIMP_VER        0x01000001
#define SIMP_VER_STR    "1.0.0.1"

////////////////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////////////////

// ע��ûʹ�ù����������
#define SIMP_NOT_USED(x)        (x)

// �ַ�����������
#define _SIMP_STRINGIZE(x)      #x
#define SIMP_STRINGIZE(x)       _SIMP_STRINGIZE(x)
#define SIMP_STRINGIZET(x)      _T(SIMP_STRINGIZE(x))

// ƴ��
#define _SIMP_CONCAT(x, y)      x##y
#define SIMP_CONCAT(x, y)       _SIMP_CONCAT(x, y)

// ���ַ���������
#define _SIMP_WIDE(str)         L##str
#define SIMP_WIDE(str)          _SIMP_WIDE(str)

// C/C++ ����Լ��
#ifdef __cplusplus
#define SIMP_EXTERN_C           extern "C"
#define SIMP_EXTERN_C_BEGIN     extern "C" {
#define SIMP_EXTERN_C_END       }
#define SIMP_EXTERN_CPP         extern "C++"
#define SIMP_EXTERN_CPP_BEGIN   extern "C++" {
#define SIMP_EXTERN_CPP_END     }
#else
// __cplusplus NOT defined
#define SIMP_EXTERN_C           extern
#define SIMP_EXTERN_C_BEGIN
#define SIMP_EXTERN_C_END
#endif

// ֻ����һ��ȫ�ִ洢, �Ա��� .h �ж���ȫ�ֱ���
#define SIMP_GLOBAL __declspec(selectany)

// ��� TRUE ��ȥ��
#define SIMP_TRUE_DO(x, exp)    if (x) { exp; }
#define SIMP_ON_DO(x, exp)      SIMP_TRUE_DO((x), exp)
// ��� FALSE ��ȥ��
#define SIMP_FALSE_DO(x, exp)   if (!(x)) { exp; }
#define SIMP_OFF_DO(x, exp)     SIMP_FALSE_DO((x), exp)

// ����ĳ�º� goto
#define SIMP_GOTO(exp, label)   { exp; goto label; }

// ����ֵ
// 32bit ����ֵ������ x ��Чȡֵ 0 ~ 31
// x86 GCC �� VC ʵ���� SIMP_MASK(0) = SIMP_MASK(32) = 1
#define SIMP_MASK(x)            (1 << (x))

// Simp ģ����
const char      MODULE_NAMEA[]  = "Simp";
const wchar_t   MODULE_NAMEW[]  = L"Simp";
const _TCHAR    MODULE_NAME[]   = _T("Simp");

////////////////////////////////////////////////////////////////////////////////
// ��̬����
////////////////////////////////////////////////////////////////////////////////

// ��� expr = FALSE, VC ����������� error C2027
template <BOOL expr> struct StaticAssert;
template <> struct StaticAssert<TRUE> {};
template <size_t Size> struct StaticAssertTest {};

#define SIMP_STATIC_ASSERT(x) \
    typedef StaticAssertTest<sizeof(StaticAssert<BOOL(x)>)> StaticAssertType##__LINE__

////////////////////////////////////////////////////////////////////////////////
// �ڴ���亯��
////////////////////////////////////////////////////////////////////////////////

// Simp ��ͨ���ڴ���亯��, �� memory/alloc.h �ж���

extern void* Malloc(size_t size);
extern void* Realloc(void* memblock, size_t size);
extern void* Calloc(size_t num, size_t size);
extern void Free(void* memblock);

SIMP_NS_END
