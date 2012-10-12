////////////////////////////////////////////////////////////////////////////////
// ��    �� : Path.h
// ����˵�� :
//   ·������
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
// ·������
////////////////////////////////////////////////////////////////////////////////

// �õ�ģ�����ڵ�Ŀ¼ȫ·��, �� '\' ��β
inline
BOOL GetModuleDir(HMODULE module, __out _TCHAR* dir, size_t sizeDir, _TCHAR dirDelim = _T('\\')) {
    if (dir == NULL || sizeDir == 0)
        return FALSE;

    DWORD len = GetModuleFileName(module, dir, (DWORD) sizeDir);

    // =0 ����
    // =size_dir, ������̫С·�����ض�
    if (len == 0 || len == sizeDir)
        return FALSE;

    // ȥ�� path �� '\' ֮��Ĳ���
    _TCHAR* bslash = _tcsrchr(dir, dirDelim);

    // ·����Ӧ���� '\'
    if (bslash == NULL)
        return FALSE;

    // ��� '\' ���ǻ����������һ��λ��
    if (bslash != dir + sizeDir - 1)
        *(bslash + 1) = _T('\0');

    return TRUE;
}

SIMP_NS_END
