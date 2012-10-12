////////////////////////////////////////////////////////////////////////////////
// ��    �� : MemHelper.h
// ����˵�� :
//   �ڴ泣�ò���
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
// ����
////////////////////////////////////////////////////////////////////////////////

template <typename Type>
inline void ZeroObject(__out Type* p) {
    ZeroMemory(p, sizeof(Type));
}

template <typename Type>
inline void ZeroArray(__out Type* p, size_t num) {
    ZeroMemory(p, num * sizeof(Type));
}

////////////////////////////////////////////////////////////////////////////////
// ɾ���� deleter
////////////////////////////////////////////////////////////////////////////////

// ��׼ delete �ͷ��ڴ�
template <BOOL Check = FALSE, BOOL Zeroed = FALSE>
struct StdDel {
    template <typename Type>
    void operator()(Type*& p) {
        if (!Check || p != NULL) {
            delete p;
            if (Zeroed)
                p = NULL;
        }
    }
};

const StdDel<FALSE, FALSE>  STDDEL;
const StdDel<FALSE, TRUE>   STDDEL_Z;
const StdDel<TRUE, FALSE>   STDDEL_C;
const StdDel<TRUE, TRUE>    STDDEL_CZ;

// ��׼ delete[] �ͷ��ڴ�
template <BOOL Check = FALSE, BOOL Zeroed = FALSE>
struct StdDelArr {
    template <typename Type>
    void operator()(Type*& p) {
        if (!Check || p != NULL) {
            delete[] p;
            if (Zeroed)
                p = NULL;
        }
    }
};

const StdDelArr<FALSE, FALSE>   STDDELARR;
const StdDelArr<FALSE, TRUE>    STDDELARR_Z;
const StdDelArr<TRUE, FALSE>    STDDELARR_C;
const StdDelArr<TRUE, TRUE>     STDDELARR_CZ;

// �� Free �ͷ��ڴ�
template <BOOL Check = FALSE, BOOL Zeroed = FALSE>
struct SimpFree {
    template <typename Type>
    void operator()(Type*& p) {
        if (!Check || p != NULL) {
            Free(p);
            if (Zeroed)
                p = NULL;
        }
    }
};

const SimpFree<FALSE, FALSE> STDFREE;
const SimpFree<FALSE, TRUE>  STDFREE_Z;
const SimpFree<TRUE, FALSE>  STDFREE_C;
const SimpFree<TRUE, TRUE>   STDFREE_CZ;

// �ͷ� COM ����
template <BOOL Check = FALSE, BOOL Zeroed = FALSE>
struct COMRel {
    template <typename Type>
    void operator()(Type*& p) {
        if (!Check || p != NULL) {
            p->Release();
            if (Zeroed)
                p = NULL;
        }
    }
};

const COMRel<FALSE, FALSE>  COMREL;
const COMRel<FALSE, TRUE>   COMREL_Z;
const COMRel<TRUE, FALSE>   COMREL_C;
const COMRel<TRUE, TRUE>    COMREL_CZ;

// �ر� Windows ���
template <BOOL Check = FALSE, BOOL Zeroed = FALSE>
struct HClose {
    template <typename Type>
    void operator()(Type& h) {
        if (!Check || h != NULL) {
            ::CloseHandle(h);
            if (Zeroed)
                h = NULL;
        }
    }
};

const HClose<FALSE, FALSE>  HCLOSE;
const HClose<FALSE, TRUE>   HCLOSE_Z;
const HClose<TRUE, FALSE>   HCLOSE_C;
const HClose<TRUE, TRUE>    HCLOSE_CZ;

// �ر� FILE ָ��
template <BOOL Check = FALSE, BOOL Zeroed = FALSE>
struct FClose {
    void operator()(FILE*& fp) {
        if (!Check || fp != NULL) {
            ::fclose(fp);
            if (Zeroed)
                fp = NULL;
        }
    }
};

const FClose<FALSE, FALSE>  FCLOSE;
const FClose<FALSE, TRUE>   FCLOSE_Z;
const FClose<TRUE, FALSE>   FCLOSE_C;
const FClose<TRUE, TRUE>    FCLOSE_CZ;

// ɾ��������
// ʹ��ʾ��: Delete(obj, STDDEL_CZ)
template <typename Type, typename Deleter>
inline void Delete(Type& obj, Deleter del) {
    del(obj);
}

SIMP_NS_END
