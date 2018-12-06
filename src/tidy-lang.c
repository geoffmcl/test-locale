/*\
 * tidy-lang.c
 *
 * Copyright (c) 2018 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#include <stdlib.h> 
#include <stdio.h>
#include <string.h> // for strdup(), ...
#include <locale.h>
#include <malloc.h>
// other includes



static const char *module = "tidy-lang";

//static const char *usr_input = 0;
static int getenvcnt = 0;

void give_help( char *name )
{
    printf("%s: usage: [options]\n", module);
    printf("Options:\n");
    printf(" --help  (-h or -?) = This help and exit(0)\n");
    // TODO: More help
}

typedef struct tagLCCATS {
    int cat;
    const char *name;
}LCCATS, *PLCCATS;

static LCCATS lc_cats[] = {
    { LC_ALL, "LC_ALL"},
    { LC_COLLATE, "LC_COLLATE"},
    { LC_CTYPE, "LC_CTYPE"},
#ifdef LC_MESSAGES
    { LC_MESSAGES, "LC_MESSAGES"},
#endif
    { LC_MONETARY, "LC_MONETARY"},
    { LC_NUMERIC, "LC_NUMERIC"},
    { LC_TIME, "LC_TIME"},

    /* must be last */
    { 0, 0 }
};

static const char *get_cat_name(int cat)
{
    PLCCATS pc = lc_cats;
    while (pc->name) {
        if (pc->cat == cat)
            return pc->name;
        pc++;
    }
    return "UNKNOWN";
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
            printf("%s: Unknown bear argument '%s'. Try -? for help...\n", module, arg);
            return 1;
        }
    }
    return 0;
}

void show_env(void)
{
    char *msg = getenv("LC_MESSAGES");
    char *lng = getenv("LANGUAGE");
    char *lang = getenv("LANG");
    getenvcnt++;
    printf("%s:%d: getenv LC_MESSAGES=%s, LANGUAGE=%s, LANG=%s\n", module, getenvcnt,
        msg ? msg : "<null>",
        lng ? lng : "<null>",
        lang ? lang : "<null>");
}


int show_tidy_lang(void)
{
    int iret = 0;
    char buf[128];
    char *cp, *sl_null, *lc_all, *org_null;
    int category = LC_ALL;
    const char *pc = get_cat_name(category);
    char *ph = buf;

    sprintf(ph, "setlocale(%s,", pc);

    show_env();

    org_null = setlocale(category, NULL);
    cp = org_null;
    sl_null = 0;
    if (cp) {
        size_t len = strlen(cp);
        if (len) {
            char *copy = (char *)malloc(len + 1);
            len++;
            if (!copy) {
                printf("%s: memory FAILED %d - aborting...\n", module, (int)len);
                return 1;
            }
            strcpy(copy, cp);   /* copy the org string, for safety */
            sl_null = copy;
            cp = copy;
        }
        printf("%s NULL) = %s\n", ph, cp);
    }
    else {
        printf("%s NULL) FAILED!\n", ph);
        iret |= 1;
    }

    show_env();
    /* get the 'default' user lang */
    lc_all = setlocale(category, "");
    cp = lc_all;
    if (cp) {
        printf("%s \"\") = %s\n", ph, cp);
    }
    else {
        printf("%s \"\") FAILED!\n", ph);
        iret |= 2;
    }
    show_env();
    cp = setlocale(category, NULL);
    if (cp) {
        printf("%s NULL) = %s\n", ph, cp);
    }
    else {
        printf("%s NULL) FAILED!\n", ph);
        iret |= 4;
    }
    show_env();
    /* restore original, if we got one... */
    cp = setlocale(category, sl_null);
    if (cp) {
        printf("%s %s) = %s\n", ph, 
            sl_null ? sl_null : "<null>",
            cp);
    }
    else {
        printf("%s %s) FAILED!\n", ph,
            sl_null ? sl_null : "<null>" );
        iret |= 8;
    }

    /* what is org_null pointing to, if anything ... */
    if (org_null) {
        printf("org.%s NULL) = %s\n", ph, org_null);
    }

    if (sl_null) {
        free(sl_null);
    }

    show_env();
    return iret;
}

// main() OS entry
int main( int argc, char **argv )
{
    int iret = 0;
    iret = parse_args(argc,argv);
    if (iret) {
        if (iret == 2)
            iret = 0;
        return iret;
    }

    iret = show_tidy_lang(); // TODO: actions of app

    return iret;
}


// eof = tidy-lang.c
