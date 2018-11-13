/*\
 * show-std-locale.cxx
 *
 * Copyright (c) 2018 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
 * from : https://stackoverflow.com/questions/32931458/getting-the-system-language-in-c-or-c
\*/

#include <stdio.h>
#include <iostream>
#include <locale>
#include <iomanip>
#include <codecvt>
#include <string>
// other includes

//static const char *module = "show-std-locale";

// main() OS entry
int main( int argc, char **argv )
{
    int iret = 0;
    std::string n = std::locale().name();
    std::string n2 = std::locale("").name();

    std::cout << "The default locale is " << n << '\n';
    if (n2.size()) {
        std::cout << "The user's locale is " << n2 << '\n';
    }
    else {
        std::cout << "std::locale(\"\").name() FAILED\n";
    }
        //  << "A nameless locale is " << loc.name() << '\n';
    return iret;
}


// eof = show-std-locale.cxx
