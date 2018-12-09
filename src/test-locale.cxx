/*\
 * test-locale.cxx
 *
 * Copyright (c) 2017 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
 * source: https://stackoverflow.com/questions/4324542/what-is-the-windows-equivalent-for-en-us-utf-8-locale
 *
\*/

#ifdef _MSC_VER
#include <Windows.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h>  
#endif
#include <fstream>
#include <iostream>
#include <string>
#include <locale>
// #include <codecvt>

static const char *module = "test-locale";
static const char *def_file = "temptext.txt";
static time_t system_time = 0; // time(NULL);

#if 0 // 000000000000000000000000000000000000000000000000000000000
void prepare_file()
{
    // UTF-8 data
    char utf8[] = { '\x7a',                       // latin small letter 'z' U+007a
        '\xe6','\xb0','\xb4',         // CJK ideograph "water"  U+6c34
        '\xf0','\x9d','\x84','\x8b' }; // musical sign segno U+1d10b
    std::ofstream fout(def_file);
    fout.write(utf8, sizeof utf8);
    fout.close();
}
void test_file_utf16()
{
    std::wifstream fin(def_file);
    fin.imbue(std::locale(fin.getloc(), new std::codecvt_utf8_utf16<wchar_t>));
    std::cout << "Read from file using UTF-8/UTF-16 codecvt\n";
    for (wchar_t c; fin >> c; )
        std::cout << std::hex << std::showbase << c << '\n';
}

void test_file_ucs2()
{
    std::wifstream fin(def_file);
    fin.imbue(std::locale(fin.getloc(), new std::codecvt_utf8<wchar_t>));
    std::cout << "Read from file using UTF-8/UCS2 codecvt\n";
    for (wchar_t c; fin >> c; )
        std::cout << std::hex << std::showbase << c << '\n';
}

#endif // #if 0 // 000000000000000000000000000000000000000000000000000000000

void show_current_locale()
{
    std::cout << std::endl;
    char *loc = setlocale(LC_ALL, NULL);
    if (loc)
        std::cout << "Current 'setlocale(LC_ALL, NULL) returned: '" << loc << "'\n";
    else
        std::cout << "Current 'setlocale(LC_ALL, NULL) returned: 'NULL'\n";
    loc = setlocale(LC_ALL, "");
    if (loc)
        std::cout << "Current 'setlocale(LC_ALL, \"\") returned: '" << loc << "'\n";
    else
        std::cout << "Current 'setlocale(LC_ALL, \"\") returned: 'NULL'\n";
    std::cout << std::endl;
}

// from : https://stackoverflow.com/questions/571359/how-do-i-set-the-proper-initial-locale-for-a-c-program-on-windows
void show_locale()
{
#ifdef _WIN32
    _locale_t loc = _get_current_locale(); // Gets a locale object representing the current locale.
#endif 
                                           // At program startup, the global locale is the "C" or "Classic" locale
    std::string s, name = std::locale().name();
    std::cout << "at start std::locale().name() = " << name << std::endl;
    std::locale::global(std::locale(""));
    s = std::locale().name();
    std::cout << "after std::locale::global(std::locale(\"\")) = '" << s << "'" << std::endl;
    std::locale::global(std::locale(name));
    std::cout << "return after std::locale::global(name) = " << std::locale().name() << std::endl;
}

void localf(const char* pLoc)
{
    char time_text[128]; // [81];
    char *pstg = setlocale(LC_ALL, pLoc);
    if (pstg == NULL)
    {
        fprintf(stderr, "Error: Unable to establish locale\n");
        //return;
        pstg = (char *)"FAILED";
    }
    // else
    {
        strftime(time_text, 80, "%x %A %B %d", localtime(&system_time));
        printf("Using '%s' time = [%s] stg = '%s'\n", pLoc, time_text, pstg);
    }
    return;
}

#if 0 // 000000000000000000000000000000000000000000000000000000000
int test_main()
{
    prepare_file();
    test_file_utf16();
    test_file_ucs2();
    return 0;
}
#endif // 00000000000000000000000000000000000000000000000000000000

#ifdef _MSC_VER
/*
typedef struct _CONSOLE_FONT_INFOEX {
ULONG cbSize;
DWORD nFont;
COORD dwFontSize;
UINT  FontFamily;
UINT  FontWeight;
WCHAR FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

BOOL WINAPI SetCurrentConsoleFontEx(
_In_ HANDLE               hConsoleOutput,
_In_ BOOL                 bMaximumWindow,
_In_ PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx
);

*/
void SetLucidaFont()
{
    HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX info;
    memset(&info, 0, sizeof(CONSOLE_FONT_INFOEX));
    info.cbSize = sizeof(CONSOLE_FONT_INFOEX);              // prevents err=87 below
    if (GetCurrentConsoleFontEx(StdOut, FALSE, &info))
    {
        info.FontFamily = FF_DONTCARE;
        info.dwFontSize.X = 0;  // leave X as zero
        info.dwFontSize.Y = 14;
        info.FontWeight = 400;
//#ifdef UNICODE
        //_tcscpy_s(info.FaceName, L"Lucida Console");
        wcscpy_s(info.FaceName, L"Lucida Console");
//#else
//        strcpy(info.FaceName, L"Lucida Console");
//#endif

        if (SetCurrentConsoleFontEx(StdOut, FALSE, &info))
        {
            printf("set console to 'Lucinda' ok\n");
        }
        else {
            printf("set console to 'Lucinda' FAILED\n");
        }

    }
}
#endif

int test_setlocale()
{
    printf("\nSome tests using  setlocale(LC_ALL, pLoc)\n");
#ifdef _MSC_VER
    SetLucidaFont();
#endif

    localf("C");
    localf("french");
    localf("chinese");
    localf("german");
    localf("rubbish");
    localf("C");    // return to DEFAULT

    return 0;
}

// main() OS entry
int main( int argc, char **argv )
{
    int iret = 0;
    system_time = time(NULL);
    show_current_locale();
    show_locale();
    // iret = test_main();
    iret |= test_setlocale();
    show_current_locale();
    return iret;
}


// eof = test-locale.cxx
