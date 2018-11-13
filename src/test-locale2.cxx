/*\
 * test-locale2.cxx
 *
 * Copyright (c) 2018 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
 * from : https://en.cppreference.com/w/c/locale/setlocale
\*/

#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <wchar.h>
#ifdef _WIN32
#include <process.h>  
#include <windows.h>  
#endif // _WIN32

#ifdef _WIN32
int thread_main();
//static const char *module = "test-locale2";
//static const char *en_US = "en_US.UTF-8";
static const char *en_US = "en-US"; // en_US";
//static const char *de_DE = "de_DE.utf8";
static const char *de_DE = "de-DE";
//static const char *ja_JP = "ja_JP.utf8";
static const char *ja_JP = "ja-JP";
// static const char *ja_JP = "fr-FR";
#else
////// UNIX //////
static const char *en_US = "en_US.utf-8";
static const char *de_DE = "fr_FR.utf-8"; // "de_DE.utf-8";
static const char *ja_JP = "fr_FR.utf-8";
// static const char *ja_JP = "fr_FR";

#endif
void show_time()
{
    time_t t = time(NULL);
    printf("\nShow Number 3.14 and date-time string...\n");
#ifdef _WIN32
    wchar_t str[100];
    wcsftime(str, 100, L"%A %c", localtime(&t));
    wprintf(L"Number: %.2f\nDate: %Ls\n", 3.14, str);
#else
    char str[100];
    str[0] = 0;
    strftime(str, 100, "%A %c", localtime(&t));
    printf("Number: %.2f\nDate: %s\n", 3.14, str);
#endif
    printf("\n");
}

// main() OS entry
int main(void)
{
    int iret = 0;
    char * cp;
    
    show_time();
    
    cp = setlocale(LC_ALL, NULL);
    if (cp) {
        printf("setlocale(LC_ALL, NULL) = %s\n", cp);
    }
    else {
        printf("setlocale(LC_ALL, NULL) FAILED!\n");
        iret |= 16;
    }
    cp = setlocale(LC_ALL, "");
    if (cp) {
        printf("setlocale(LC_ALL, \"\") = %s\n", cp);
    }
    else {
        printf("setlocale(LC_ALL, \"8\") FAILED!\n");
        iret |= 8;
    }

    // the C locale will be UTF-8 enabled English;
    // decimal dot will be German
    // date and time formatting will be Japanese
    cp = setlocale(LC_ALL, en_US);
    if (cp) {
        printf("setlocale(LC_ALL, \"%s\") = %s\n", en_US, cp);
    }
    else {
        printf("setlocale(LC_ALL, \"%s\") FAILED!\n", en_US);
        iret |= 1;
    }
    cp = setlocale(LC_NUMERIC, de_DE);
    if (cp) {
        printf("setlocale(LC_NUMERIC, \"%s\") = %s\n", de_DE, cp);
    }
    else {
        printf("setlocale(LC_NUMERIC, \"%s\") FAILED!\n", de_DE);
        iret |= 2;
    }
    cp = setlocale(LC_TIME, ja_JP);
    if (cp) {
        printf("setlocale(LC_TIME, \"%s\") = %s\n", ja_JP, cp);
    }
    else {
        printf("setlocale(LC_TIME, \"%s\") FAILED!\n", ja_JP);
        iret |= 4;
    }

    show_time();

#ifdef _WIN32
    iret |= thread_main();
#endif
    return iret;
}


#ifdef _WIN32
////////////////////////////////////////////////

// from : https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=EN-US&k=k%28LOCALE%2Fsetlocale%29%3Bk%28setlocale%29%3Bk%28DevLang-C%2B%2B%29%3Bk%28TargetOS-Windows%29&rd=true&f=255&MSPPError=-2147217396

#define BUFF_SIZE 100  

// Retrieve the date in the current  
// locale's format.  
int get_date(unsigned char* str)
{
    __time64_t ltime;
    struct tm  thetime;

    // Retrieve the current time  
    _time64(&ltime);
    _gmtime64_s(&thetime, &ltime);

    // Format the current time structure into a string  
    // "%#x" is the long date representation in the  
    // current locale  
    if (!strftime((char *)str, BUFF_SIZE, "%#x",
        (const struct tm *)&thetime))
    {
        printf("strftime failed!\n");
        return -1;
    }
    size_t len = strlen((char *)str);
    while (len > 0) {
        len--;
        if (str[len] > ' ')
            break;
        str[len] = 0;
    }
    return 0;
}

// This thread sets its locale to the argument  
// and prints the date.  
// (unsigned(__stdcall *start_address)(void *))
// Error:	argument of type "uintptr_t (__stdcall *)(void *pArguments)" is incompatible with 
// parameter of type "_beginthreadex_proc_type"	

uintptr_t __stdcall SecondThreadFunc(void* pArguments)
{
    unsigned char str[BUFF_SIZE];
    char * locale = (char *)pArguments;
    char * cp;

    // Set the thread locale  
    cp = setlocale(LC_ALL, locale);
    printf("thread: locale is now set to %s.\n",
        cp ? cp : "FAILED");

    // Retrieve the date string from the helper function  
    if (get_date(str) == 0)
    {
        printf("thread: The date in %s locale is: '%s'\n", locale, str);
        printf("thread: A floating point number %.2f\n", 3.14);
    }

    _endthreadex(0);
    return 0;
}

//const char *loc_thread = "de-DE";
const char *loc_thread = "fr-FR";
// The main thread sets the locale to English   
// and then spawns a second thread (above) and prints the date.  
int thread_main()
{
    HANDLE          hThread;
    unsigned        threadID;
    unsigned char   str[BUFF_SIZE];
    char *          cp;

    printf("\nLocale per thread test...\n");
    // Configure per-thread locale to cause all subsequently created   
    // threads to have their own locale.  
    _configthreadlocale(_ENABLE_PER_THREAD_LOCALE);

    cp = setlocale(LC_ALL, "en-US");
    // Set the locale of the main thread to US English.  
    printf("main: thread locale is now set to %s.\n",
        cp ? cp : "FAILED");

    // Create the second thread with a German locale.  
    // Our thread function takes an argument of the locale to use.  
    hThread = (HANDLE)_beginthreadex(NULL, 0,  (_beginthreadex_proc_type)&SecondThreadFunc,
        (void *)loc_thread, 0, &threadID);

    if (get_date(str) == 0)
    {
        // Retrieve the date string from the helper function  
        printf("main: The date in en-US locale is: '%s'\n", str);
        printf("main: A floating point number %.2f\n", 3.14);
    }

    if (hThread) {

        // Wait for the created thread to finish.  
        WaitForSingleObject(hThread, INFINITE);

        // Destroy the thread object.  
        CloseHandle(hThread);
    }
    else {
        printf("_beginthreadex FAILED\n");
    }

    return 0;
}

////////////////////////////////////////////////
#endif //#ifdef _WIN32


// eof = test-locale2.cxx
