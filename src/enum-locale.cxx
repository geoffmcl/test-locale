/*\
 * enum-locale.cxx
 *
 * Copyright (c) 2018 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#include <stdio.h>
#include <string.h> // for strdup(), ...
#ifdef _WIN32
#include <Windows.h>
#endif

// other includes

static const char *module = "enum-locale";

static const char *usr_input = 0;

void give_help( char *name )
{
    printf("%s: usage: [options] usr_input\n", module);
    printf("Options:\n");
    printf(" --help  (-h or -?) = This help and exit(0)\n");
    // TODO: More help
}

int parse_args( int argc, char **argv )
{
    int i,i2,c;
    char *arg, *sarg;
    for (i = 1; i < argc; i++) {
        arg = argv[i];
        i2 = i + 1;
        if (*arg == '-') {
            sarg = &arg[1];
            while (*sarg == '-')
                sarg++;
            c = *sarg;
            switch (c) {
            case 'h':
            case '?':
                give_help(argv[0]);
                return 2;
                break;
            // TODO: Other arguments
            default:
                printf("%s: Unknown argument '%s'. Try -? for help...\n", module, arg);
                return 1;
            }
        } else {
            // bear argument
            if (usr_input) {
                printf("%s: Already have input '%s'! What is this '%s'?\n", module, usr_input, arg );
                return 1;
            }
            usr_input = strdup(arg);
        }
    }
    if (!usr_input) {
        printf("%s: No user input found in command!\n", module);
        return 1;
    }
    return 0;
}

#ifdef _WIN32
size_t cb_cnt = 0;
size_t max_wrap = 10;
size_t cur_wrap = 0;

BOOL CALLBACK EnumLocalesProcEx(LPWSTR lpLocaleString, DWORD  dwFlags, LPARAM lParam)
{
    BOOL bRet = TRUE;

    cb_cnt++;
    wprintf(L"%zu '%s' ", cb_cnt, lpLocaleString);
    cur_wrap++;
    if (cur_wrap >= max_wrap) {
        cur_wrap = 0;
        wprintf(L"\n");
    }
    return bRet;
}

int do_enum()
{
    DWORD dwFlag = LOCALE_ALL;
    BOOL res = EnumSystemLocalesEx(
        EnumLocalesProcEx,  // LOCALE_ENUMPROCEX lpLocaleEnumProcEx,
        dwFlag,             // DWORD             dwFlags,
        0,                  // LPARAM            lParam,
        0);                 // LPVOID            lpReserved
    if (cur_wrap) {
        wprintf(L"\n");
    }
    if (res) {
        printf("%s: Callback %d times...\n", module, (int)cb_cnt);
    }
    else {
        DWORD err = GetLastError();
        printf("%s: Enum failed... Err=%u...\n", module, err);

    }

    return res ? 0 : 1;
}
#endif

// main() OS entry
#ifdef _WIN32
int main( int argc, char **argv )
{
    int iret = 0;
    //iret = parse_args(argc,argv);
    if (iret) {
        if (iret == 2)
            iret = 0;
        return iret;
    }

    iret = do_enum();   // TODO: actions of app
    printf("%s: exit(%d)\n", module, iret);
    return iret;
}

#else
int main( int argc, char **argv )
{
    printf("Coded for _WIN32 only...\n"); 
    return 0;
}
#endif

// eof = enum-locale.cxx
