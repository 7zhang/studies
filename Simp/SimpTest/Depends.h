// Depends.h
//

#pragma once

////////////////////////////////////////////////////////////////////////////////
// C ��׼��������ʱ�� (CRT)
////////////////////////////////////////////////////////////////////////////////

// #define _CRT_SECURE_NO_WARNINGS     // �رղ���ȫ�� CRT ��������
#define _USE_MATH_DEFINES           // ʹ�� math.h �е� M_PI �ȳ���
#define _CRTDBG_MAP_ALLOC           // ʹ�� Debug ��� malloc �� strdup

#include <stdlib.h>
#include <errno.h>
#include <crtdbg.h>
#include <time.h>

#include <stdio.h>
#include <conio.h>

#include <limits.h>
#include <float.h>
#include <math.h>

#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <mbstring.h>
#include <wchar.h>
#include <tchar.h>

#include <sal.h>
#include <direct.h>
#include <typeinfo.h>
#include <sys/types.h>
#include <sys/stat.h>

////////////////////////////////////////////////////////////////////////////////
// C++ ��׼��
////////////////////////////////////////////////////////////////////////////////

#include <ios>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <iomanip>

#include <exception>
#include <stdexcept>

#include <locale>
#include <utility>
#include <numeric>
#include <typeinfo>

#include <memory>
#include <new>

#include <string>
#include <vector>
#include <list>
#include <deque>
#include <bitset>
#include <map>
#include <set>
#include <queue>
#include <stack>

#include <iterator>
#include <algorithm>
#include <functional>

////////////////////////////////////////////////////////////////////////////////
// Windows API
////////////////////////////////////////////////////////////////////////////////

// �ų� Windows ͷ�ļ��в����õĹ���
#define WIN32_LEAN_AND_MEAN

// ��ʹ�� Windows ͷ�ļ��ж���� min, max ��, ����Ӱ�� std ���ֿռ�� min, max
#define NOMINMAX

#include <Windows.h>

////////////////////////////////////////////////////////////////////////////////
// Simp
////////////////////////////////////////////////////////////////////////////////

#include <Simp/Defs.h>
#include <Simp/Memory.h>
#include <Simp/Debug.h>
#include <Simp/Utility.h>
#include <Simp/String.h>
#include <Simp/Stream.h>
#include <Simp/Thread.h>
#include <Simp/Log.h>
#include <Simp/File.h>
#include <Simp/Path.h>
