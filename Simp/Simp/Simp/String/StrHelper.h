////////////////////////////////////////////////////////////////////////////////
// ��    �� : StrHelper.h
// ����˵�� :
//   �ַ������ò���
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
// ���Ͷ���
////////////////////////////////////////////////////////////////////////////////

typedef std::basic_string<_TCHAR> tstring;

////////////////////////////////////////////////////////////////////////////////
// �ַ������� CharTraits
////////////////////////////////////////////////////////////////////////////////

template <typename CharT>
struct CharTraits {
    typedef CharT   CharType;
    typedef int     IntType;
    static const IntType XEOF = -1;
};

// char �ػ�
template <>
struct CharTraits<char> {
    typedef char    CharType;
    typedef int     IntType;
    static const IntType XEOF = EOF;
};

// wchar_t �ػ�
template <>
struct CharTraits<wchar_t> {
    typedef wchar_t CharType;
    typedef wint_t  IntType;
    static const IntType XEOF = WEOF;
};

////////////////////////////////////////////////////////////////////////////////
// char/wchar_t ���ַ�/�ַ�����������ͳһ��ʾ
// ��;:
//   �ڽ��� CharT ��ģ���б����ַ�/�ַ�����������д��ͳһ
////////////////////////////////////////////////////////////////////////////////

// ��һ�㻯����
template <typename CharT>
const CharT* Str(const char* str, const wchar_t* wstr);
template <typename CharT>
const CharT Char(const char ch, const wchar_t wch);

// char �ػ�
template <>
inline const char* Str(const char* str, const wchar_t* wstr) {
    return str;
}
template <>
inline const char Char(const char ch, const wchar_t wch) {
    return ch;
}

// wchar_t �ػ�
template <>
inline const wchar_t* Str(const char* str, const wchar_t* wstr) {
    return wstr;
}
template <>
inline const wchar_t Char(const char ch, const wchar_t wch) {
    return wch;
}

#define SIMP_STR(type, str) Simp::Str<type>(str, SIMP_WIDE(str))
#define SIMP_CHAR(type, ch) Simp::Char<type>(ch, SIMP_WIDE(ch))

////////////////////////////////////////////////////////////////////////////////
// �ַ�����ʽ������
////////////////////////////////////////////////////////////////////////////////

// ת�� CRT vsprintf �ĺ���ģ��, ��һ�㻯����
template <typename CharT>
int vsprintf_t(CharT* buf, size_t charSize, const CharT* fmt, va_list args);

// vsprintf_t �� char �ػ�
template <>
inline
int vsprintf_t(char* buf, size_t charSize, const char* fmt, va_list args) {
    return vsprintf_s(buf, charSize, fmt, args);
}

// vsprintf_t �� wchar_t �ػ�
template <>
inline
int vsprintf_t(wchar_t* buf, size_t charSize, const wchar_t* fmt, va_list args) {
    return vswprintf_s(buf, charSize, fmt, args);
}

// ��ʽ������ string
template <size_t Size, typename CharT>
inline
std::basic_string<CharT> MakeString(const CharT* fmt, ...) {
    CharT buf[Size];
    va_list args;
    va_start(args, fmt);
    vsprintf_t(buf, Size, fmt, args);
    va_end(args);
    return std::basic_string<CharT>(buf);
}

////////////////////////////////////////////////////////////////////////////////
// ��;:
//   ������ʱ�����������ʱ���ٻ���, ��ʽ��������ʱ C �ַ���
// ����:
//   _putts(StringMaker<BUF_SIZE, _TCHAR>()(_T("%s %d"), _T("foo"), 42));
////////////////////////////////////////////////////////////////////////////////

#define SIMP_MAKESTRA(size, fmt, ...)   Simp::StringMaker<size, char>()(fmt, __VA_ARGS__)
#define SIMP_MAKESTRW(size, fmt, ...)   Simp::StringMaker<size, wchar_t>()(fmt, __VA_ARGS__)
#define SIMP_MAKESTR(size, fmt, ...)    Simp::StringMaker<size, _TCHAR>()(fmt, __VA_ARGS__)

template <size_t Size, typename CharT>
struct StringMaker {
    const _TCHAR* operator()(const CharT* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vsprintf_t(Str, Size, fmt, args);
        va_end(args);
        return Str;
    }

    CharT Str[Size];
};

////////////////////////////////////////////////////////////////////////////////
// CRT errno ������Ϣ
////////////////////////////////////////////////////////////////////////////////

// ת�� CRT strerror �ĺ���ģ��, ��һ�㻯����
template <typename CharT>
errno_t strerror_t(__out CharT* buf, size_t size, int errnum);

// char �ػ�
template <>
inline
errno_t strerror_t(__out char* buf, size_t size, int errnum) {
    return strerror_s(buf, size, errnum);
}

// wchar_t �ػ�
template <>
inline
errno_t strerror_t(__out wchar_t* buf, size_t size, int errnum) {
    return _wcserror_s(buf, size, errnum);
}

////////////////////////////////////////////////////////////////////////////////
// ��ģ�� CrtStrErr
// ��;:
//   ������ʱ�����������ʱ���ٻ���, �ڻ�� CRT errno ������Ϣʱ (strerror), ���ÿ��Ƿ�������ٻ�����
// ����:
//   _tprintf(_T("error %d: %s\n"), errno, CrtStrErr<BUF_SIZE, _TCHAR>()(errno));
////////////////////////////////////////////////////////////////////////////////

#define SIMP_CRTSTRERRA(size, errnum)   Simp::CrtStrErr<size, char>()(errnum)
#define SIMP_CRTSTRERRW(size, errnum)   Simp::CrtStrErr<size, wchar_t>()(errnum)
#define SIMP_CRTSTRERR(size, errnum)    Simp::CrtStrErr<size, _TCHAR>()(errnum)

template <size_t Size, typename CharT>
struct CrtStrErr {

    CharT* operator()(int errnum) {
        strerror_t(Msg, Size, errnum);
        return Msg;
    }

    CharT Msg[Size];
};

////////////////////////////////////////////////////////////////////////////////
// �ַ�����������
////////////////////////////////////////////////////////////////////////////////


// ��Ŀ�껺������С�����ͳ���Դ�ַ����ضϹ���
// CRT �� _tcsncpy_s ���� count = _TRUNCATE ((size_t) -1) ʱ������Դ�ַ����ض�
// �� count != _TRUNCATE ʱ��_tcsncpy_s �� _tcscpy_s ���ƣ����� Assert
// �Ľ��� StrNCpy ��ֻҪԴ�ַ������� count ����Ŀ�껺���� bufSize �Ĵ�С�ͻ�ضϣ�Ҳ����ʹ�� count �����ֵ _TRUNCATE
// Դ�ַ��� src �Ŀ����ַ��������� \0 �� count �ĸ������ȴﵽ����
// VC 2005 ʵ��Ч��: StrNCpy ���� CRT �� _tcsncpy_s
// ģ�����:
//   AlertTruncated: Ϊ TRUE ʱͨ������ֵ STRUNCATE ����Դ�ַ��ضϴ���
template <BOOL AlertTruncated, typename CharT>
inline
errno_t StrNCpyImpl(__out CharT* dst, size_t bufSize, const CharT* src, size_t count) {
    if (dst == NULL || bufSize == 0 || src == NULL)
        return EINVAL;

    CharT* p = dst;
    size_t i = 0;
    for (; i < count && i < bufSize - 1 && *src != 0; ++i)
        *p++ = *src++;
    *p = 0;

    // ֻ�����ΪĿ�껺����̫С���µĽضϣ��������ΪԴ�ַ��� count �� \0 �ȴﵽ����Ľض�
    if (AlertTruncated && i < count && *src != 0)
        return STRUNCATE;

    return 0;
}

template <typename CharT>
inline
CharT* StrNCpy(__out CharT* dst, size_t bufSize, const CharT* src, size_t count) {
    if (StrNCpyImpl<FALSE>(dst, bufSize, src, count) != 0)
        return NULL;
    return dst;
}

template <typename CharT>
inline
errno_t StrNCpyAlert(__out CharT* dst, size_t bufSize, const CharT* src, size_t count) {
    return StrNCpyImpl<TRUE>(dst, bufSize, src, count);
}

////////////////////////////////////////////////////////////////////////////////
// ȥ���ַ������߿հ��ַ�
////////////////////////////////////////////////////////////////////////////////

template <BOOL InPlace, BOOL AlertTruncated, typename CharT>
inline
errno_t TrimLeftImpl(const CharT* src, __out CharT* dst, size_t dstSize) {
    if (InPlace)
        _ASSERTE(src == dst && dstSize != 0);

    if (src == NULL || dst == NULL || dstSize == 0)
        return EINVAL;

    while (_istspace(*src))
        src++;

    if (InPlace) {
#pragma warning(push)
#pragma warning(disable: 4996)
        _tcscpy(dst, src);
#pragma warning(pop)
    }
    else
        return StrNCpyImpl<AlertTruncated>(dst, dstSize, src, _TRUNCATE);

    return 0;
}

template <typename CharT>
inline
CharT* TrimLeft(const CharT* src, __out CharT* dst, size_t dstSize) {
    return (TrimLeftImpl<FALSE, FALSE>(src, dst, dstSize) != 0 ? NULL : dst);
}

template <typename CharT>
inline
errno_t TrimLeftAlert(const CharT* src, __out CharT* dst, size_t dstSize) {
    return TrimLeftImpl<FALSE, TRUE>(src, dst, dstSize);
}

template <typename CharT>
inline
CharT* TrimLeft(__inout CharT* src) {
    return (TrimLeftImpl<TRUE, FALSE>(src, src, _TRUNCATE) != 0 ? NULL : src);
}

template <typename CharT>
inline
errno_t TrimLeftAlert(__inout CharT* src) {
    return TrimLeftImpl<TRUE, TRUE>(src, src, _TRUNCATE);
}

// ��һ��Ԥ��д����㷨���������ж� space �ַ��� for ѭ���У����жϱ���Ŀ�껺���������ַ�
// ���Ŀ��� lastNonSpace ֮��λ����Ϊ \0 �ַ����Խض�ֱ����β������ space �ַ�����
// ���ַ������ŵ���ֻ��һ��ɨ��Դ�ַ������������٣�����ȱ���ǣ��ں�׺ space �ַ��ܶ������£�
// 1. �и������Ч�ĸ����ַ�����
//    �������㷨��Ȼ����ɨ��Դ�ַ�������ֻ���Ʊ�Ҫ���ַ���д�����٣�
// 2. ��ҪԤ���㹻���Ŀ�껺�������Դ��Ԥ��д�뵫��������� \0 �ض϶����� space �ַ�����
//    �������㷨ֻҪĿ�껺����������ǡ�� TrimRight ֮����ַ�������
template <BOOL InPlace, BOOL AlertTruncated, typename CharT>
inline
errno_t TrimRightImpl(const CharT* src, __out CharT* dst, size_t dstSize) {
    if (InPlace)
        _ASSERTE(src == dst && dstSize != 0);

    if (src == NULL || dst == NULL || dstSize == 0)
        return EINVAL;

    // ��¼���һ���� space �ַ�λ��
    const CharT* lastNonSpace = NULL;
    for (const CharT* p = src; *p != 0; p++) {
        if (!_istspace(*p))
            lastNonSpace = p;
    }
    if (lastNonSpace == NULL)
        *dst = 0;
    else {
        _ASSERTE(*lastNonSpace != 0);
        if (InPlace)
            *(const_cast<CharT*>(lastNonSpace) + 1) = 0;
        else
            return StrNCpyImpl<AlertTruncated>(dst, dstSize, src, lastNonSpace - src + 1);
    }

    return 0;
}

template <typename CharT>
inline
CharT* TrimRight(const CharT* src, __out CharT* dst, size_t dstSize) {
    return (TrimRightImpl<FALSE, FALSE>(src, dst, dstSize) != 0 ? NULL : dst);
}

template <typename CharT>
inline
errno_t TrimRightAlert(const CharT* src, __out CharT* dst, size_t dstSize) {
    return TrimRightImpl<FALSE, TRUE>(src, dst, dstSize);
}

template <typename CharT>
inline
CharT* TrimRight(__inout CharT* src) {
    return (TrimRightImpl<TRUE, FALSE>(src, src, _TRUNCATE) != 0 ? NULL : src);
}

template <typename CharT>
inline
errno_t TrimRightAlert(__inout CharT* src) {
    return TrimRightImpl<TRUE, TRUE>(src, src, _TRUNCATE);
}

template <BOOL InPlace, BOOL AlertTruncated, typename CharT>
inline
errno_t TrimImpl(const CharT* src, __out CharT* dst, size_t dstSize) {
    if (InPlace)
        _ASSERTE(src == dst && dstSize != 0);

    if (src == NULL || dst == NULL || dstSize == 0)
        return EINVAL;

    while (_istspace(*src))
        src++;

    if (InPlace && src == dst)
        return TrimRightImpl<TRUE, AlertTruncated>(src, dst, dstSize);
    return TrimRightImpl<FALSE, AlertTruncated>(src, dst, dstSize);
}

template <typename CharT>
inline
CharT* Trim(const CharT* src, __out CharT* dst, size_t dstSize) {
    return (TrimImpl<FALSE, FALSE>(src, dst, dstSize) != 0 ? NULL : dst);
}

template <typename CharT>
inline
errno_t TrimAlert(const CharT* src, __out CharT* dst, size_t dstSize) {
    return TrimImpl<FALSE, TRUE>(src, dst, dstSize);
}

template <typename CharT>
inline
CharT* Trim(__inout CharT* src) {
    return (TrimImpl<TRUE, FALSE>(src, src, _TRUNCATE) != 0 ? NULL : src);
}

template <typename CharT>
inline
errno_t TrimAlert(__inout CharT* src) {
    return TrimImpl<TRUE, TRUE>(src, src, _TRUNCATE);
}

////////////////////////////////////////////////////////////////////////////////
// ת���ַ���
////////////////////////////////////////////////////////////////////////////////

// n = 6E, \n = 0A
// t = 74, \t = 09
// v = 76, \v = 0B
// b = 62, \b = 08
// r = 72, \r = 0D
// f = 66, \f = 0C
// a = 61, \a = 07
// \ = 5C, \\ = 5C
// ? = 3F, \? = 3F
// ' = 27, \' = 27
// " = 2

template <typename CharT>
inline
CharT EscCharMap(CharT ch) {
    //  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   A,   B,   C,   D,   E,   F
    static char charMap[256] = {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 0
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 1
        0,   0,'\"',   0,   0,   0,   0,'\'',   0,   0,   0,   0,   0,   0,   0,   0,   // 2
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,'\?',   // 3
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 4
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,'\\',   0,   0,   0,   // 5
        0,'\a','\b',   0,   0,   0,'\f',   0,   0,   0,   0,   0,   0,   0,'\n',   0,   // 6
        0,   0,'\r',   0,'\t',   0,'\v',   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 7
        0
    };
    return (CharT) charMap[(unsigned char) ch];
}

template <typename CharT>
inline
int EscHex(const CharT*& src) {
    _ASSERTE(src != NULL);

    char ch = (char) *(++src);
    int val = 0;
    if (ch >= '0' && ch <= '9')
        val = ch - '0';
    else if (ch >= 'A' && ch <= 'F')
        val = 10 + ch - 'A';
    else if (ch >= 'a' && ch <= 'f')
        val = 10 + ch - 'a';
    else
        return -1;

    ch = (char) *(++src);
    if (ch >= '0' && ch <= '9')
        val = (val << 4) + (ch - '0');
    else if (ch >= 'A' && ch <= 'F')
        val = (val << 4) + (10 + ch - 'A');
    else if (ch >= 'a' && ch <= 'f')
        val = (val << 4) + (10 + ch - 'a');
    else
        --src;
    return val;
}

template <typename CharT>
inline
int EscUCS2(const CharT*& src) {
    _ASSERTE(src != NULL);

    int val = 0;
    for (int i = 0; i < 4; ++i) {
        char ch = (char) *(++src);
        val <<= 4;
        if (ch >= '0' && ch <= '9')
            val += (ch - '0');
        else if (ch >= 'A' && ch <= 'F')
            val += (10 + ch - 'A');
        else if (ch >= 'a' && ch <= 'f')
            val += (10 + ch - 'a');
        else
            return -1;
    }
    return val;
}

// ����:
//   dstEnd: Ŀ�껺����β�ڱ�
// ����ֵ:
//   STRUNCATE: �����������С����, ���ض�
//   EILSEQ: ��������������, ת��任ʧ��
// ��������:
//   \xa\xdbc => \n \xdb c
//   \xa\xdhc => \n \r h c
//   \u4E2D => ��
template <BOOL InPlace, typename CharT>
inline
errno_t StrEscapeImpl(const CharT* src, __out CharT* dst, CharT* dstEnd) {
    if (InPlace)
        _ASSERTE(src == dst);

    if (src == NULL || dst == NULL || dstEnd == NULL)
        return EINVAL;

    enum ESC_STAT {
        STAT_NORMAL,
        STAT_ESCAPED
    } stat = STAT_NORMAL;

    CharT ch = *src;
    while (ch != 0) {
        switch (stat) {
        case STAT_NORMAL:
            if (ch == _T('\\')) {
                stat = STAT_ESCAPED;
                goto NEXT_CHAR;
            }
            break;

        case STAT_ESCAPED:
            if (ch == _T('x')) {
                int val = EscHex(src);
                if (val == -1)
                    return EILSEQ;
                ch = (CharT) val;
            }
            else if (ch == _T('u')) {
                if (sizeof(CharT) != sizeof(wchar_t))
                    return EILSEQ;
                int val = EscUCS2(src);
                if (val == -1)
                    return EILSEQ;
                ch = (CharT) val;
            }
            else {
                ch = EscCharMap<CharT>(ch);
                if (ch == 0)
                    return EILSEQ;
            }

            stat = STAT_NORMAL;
            break;
        }

        // д��Ŀ���ַ���������
        *dst = ch;
        if (++dst == dstEnd) {
            *(--dst) = 0;
            return STRUNCATE;
        }

NEXT_CHAR:
        // ����Դ�ַ�������һ���ַ�
        ch = *(++src);
    }

    *dst = 0;
    // �뿪ʱӦΪ STAT_NORMAL, ����Դ���д���
    if (stat != STAT_NORMAL)
        return EILSEQ;
    return 0;
}

template <typename CharT>
inline
errno_t StrEscape(const CharT* src, __out CharT* dst, size_t dstSize) {
    return StrEscapeImpl<FALSE>(src, dst, dst + dstSize);
}

template <typename CharT>
inline
errno_t StrEscape(__inout CharT* src) {
    return StrEscapeImpl<TRUE>(src, src, (CharT*) -1);
}

////////////////////////////////////////////////////////////////////////////////
// �ַ�����ģ�� StringT
//
// ֧�ֶ������� "" '' ��Χ���ַ��� (�м京�пո�� \" \' ת���ַ�)
////////////////////////////////////////////////////////////////////////////////

// CAUTION: ��Ҫ�� StringT, basic_string ʹ�ö�̬��, basic_string ����Ϊ��̬�󶨶���� (�� virtual dtor)
template <typename CharT>
class StringT : public std::basic_string<CharT> {
public:
    StringT() : std::basic_string<CharT>() {}
    StringT(const CharT* str) : std::basic_string<CharT>(str) {}

    // ���� is ��ȡת���ַ���
    // 1. ����������� " �� ' ��ͷ, ��ֱ����ȡ��һ���ɶԵ� " �� ' ����, ��������ʼ�ͽ����� " �� ', �������м��ת�� \" �� \'
    // 2. ��������벻�� " �� ' ��ͷ, ��ֱ����ȡ��һ���հ��ַ�����, �հ��ַ������浽 estr ��
    // 3. �������� EOF ״̬ʱ, ���� 1 �� 2 ���������
    // ��������:
    //   "ab\" cd" => ab\" cd
    //   "ab'cd" => ab'cd
    std::basic_istream<CharT>& Input(std::basic_istream<CharT>& is) {
        std::basic_streambuf<CharT>* isBuf = is.rdbuf();
        const std::locale loc = is.getloc();

        _ASSERT(isBuf != NULL);
        if (isBuf == NULL) {
            is.setstate(std::ios_base::badbit);
            return is;
        }

        // \", \' ת���ַ��任״̬
        enum QUOTE_STAT {
            STAT_INIT,      // ��ʼ״̬
            STAT_QUOTED,    // ���Ű�Χ״̬
            STAT_NOQUOTED,  // �����Ű�Χ״̬
            STAT_ESCAPED,   // ת��״̬. ��Ϊֻ�������Ű�Χ״̬�µ� \", \' ת��, ����ʵ���� STAT_QUOTED ����״̬, ����Ϊ״̬��ϵ��, ����û����״̬�任�ķ���
            STAT_END        // ����״̬
        } stat = STAT_INIT;
        std::basic_streambuf<CharT>::int_type ch;
        std::basic_streambuf<CharT>::int_type closeQuote;

        this->clear();

        // ������������ȡ�ַ�, \" \' ת��
        while (stat != STAT_END) {
            ch = isBuf->sbumpc();

            // �� buffer �Ѷ���
            if (!std::basic_streambuf<CharT>::traits_type::not_eof(ch)) {
                is.setstate(std::ios_base::eofbit);
                break;
            }

            switch (stat) {
            case STAT_INIT:
                // ���� ' �� ", ��Ϊ ���Ű�Χ״̬
                if (ch == '\"' || ch == '\'') {
                    closeQuote = ch;            // ���óɶԵĹر������ַ�
                    stat = STAT_QUOTED;
                    continue;
                }
                else
                    stat = STAT_NOQUOTED;  // ��Ϊ�����Ű�Χ״̬

            case STAT_NOQUOTED:
                // �����Ű�Χ״̬ ������ \ ת��
                // ���� �հ��ַ� ʱ, ��Ϊ����״̬
                if (std::isspace(ch, loc)) {
                    stat = STAT_END;
                    continue;
                }
                break;

            case STAT_QUOTED:
                // ���� \ ʱ, ��Ϊ ת��״̬
                if (ch == '\\')
                    stat = STAT_ESCAPED;
                // ���� �ɶԵĹر������ַ�, ��Ϊ ����״̬
                else if (ch == closeQuote) {
                    stat = STAT_END;
                    continue;
                }
                break;

            case STAT_ESCAPED:
                // ʲô������, ������Ű�Χ״̬
                stat = STAT_QUOTED;
                break;
            }

            this->push_back(std::basic_streambuf<CharT>::traits_type::to_char_type(ch));
        }

        is.width(0);
        return is;
    }


    std::basic_ostream<CharT>& Output(std::basic_ostream<CharT>& os) const {
        os << static_cast<std::basic_string<CharT> >(*this);    // NOTE: ��Ҫת��, ��������ݹ�
        return os;
    }
};

////////////////////////////////////////////////////////////////////////////////
// String �� iostream ��������
////////////////////////////////////////////////////////////////////////////////

template <typename CharT>
inline
std::basic_ostream<CharT>&
operator<<(std::basic_ostream<CharT>& os, const StringT<CharT>& str) {
    return str.Output(os);
}

template <typename CharT>
inline
std::basic_istream<CharT>&
operator>>(std::basic_istream<CharT>& is, __out StringT<CharT>& str) {
    return str.Input(is);
}

SIMP_NS_END
