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

/* ==========================================================  */
/* CODE EXTRACTED FROM TIDY */

/* `bool` is a reserved word in some but not all C++ compilers depending on age.
age. Work around is to avoid bool by introducing a new enum called `Bool`.
*/

/* We could use the C99 definition where supported
typedef _Bool Bool;
#define no (_Bool)0
#define yes (_Bool)1
*/
typedef enum
{
    no,
    yes
} Bool;


typedef unsigned int uint;

typedef char tmbchar;       /* single, possibly partial character */
#ifndef TMBSTR_DEFINED
typedef tmbchar* tmbstr;    /* pointer to buffer of possibly partial chars */
typedef const tmbchar* ctmbstr; /* Ditto, but const */
#  define NULLSTR (tmbstr)""
#  define TMBSTR_DEFINED
#endif

                                /**
                                *  These enumerations are used within instances of `languageDefinition`
                                *  structures to provide additional metadata, and are localizable
                                *  therein.
                                */
typedef enum {

    /* Specifies the language code for a particular language. */
    TIDY_LANGUAGE = 400,

    /* Marker for the last key in the structure. */
    TIDY_MESSAGE_TYPE_LAST

} tidyLanguage;


/**
*  Describes a record for a localization string.
*  - key must correspond with one of Tidy's enums (see `tidyMessageTypes`
*    below)
*  - pluralForm corresponds to gettext plural forms case (not singularity).
*    Most entries should be case 0, representing the single case.:
*    https://www.gnu.org/software/gettext/manual/html_node/Plural-forms.html
*/
typedef struct languageDictionaryEntry {
    uint key;
    uint pluralForm;
    ctmbstr value;
} languageDictionaryEntry;


/**
*  For now we'll just use an array to hold all of the dictionary
*  entries. In the future we can convert this to a hash structure
*  which will make looking up strings faster.
*/
typedef languageDictionaryEntry const languageDictionary[600];


/**
*  Finally, a complete language definition. The item `pluralForm`
*  is a function pointer that will provide the correct plural
*  form given the value `n`. The actual function is present in
*  each language header and is language dependent.
*/
typedef struct languageDefinition {
    uint(*whichPluralForm)(uint n);
    languageDictionary messages;
} languageDefinition;

////////////////////////////////////////////////////
/**
*  This structure type provides universal access to all of Tidy's strings.
*  Note limit of 8, to be changed as more added...
*/
typedef struct {
    Bool manually_set;
    languageDefinition *currentLanguage;
    languageDefinition *fallbackLanguage;
    languageDefinition *languages[8];
} tidyLanguagesType;

//////////////////////
/**
*  This language-specific function returns the correct pluralForm
*  to use given n items, and is used as a member of each language
*  definition.
*/
static uint whichPluralForm_en(uint n) {
    /* Plural-Forms: nplurals=2; */
    return n != 1;
}


/**
*  This structure specifies all of the strings needed by Tidy for a
*  single language. Static definition in a header file makes it
*  easy to include and exclude languages without tinkering with
*  the build system.
*
*  This file serves as the master template for generating `gettext`
*  PO and POT files using our `potool.rb` script. Certain comments
*  entered below will be included in the PO/POT files as developer
*  comments. To enable this, use only standard C-style comments that
*  begin immediately after the opening brace without whitespace.
*/

static languageDefinition language_en = { whichPluralForm_en,{
    /***************************************
    ** This MUST be present and first.
    ** Specify the code for this language.
    ***************************************/
    {/* Specify the ll or ll_cc language code here. */
        TIDY_LANGUAGE,                0,   "en"
    },


    {/* This MUST be present and last. */
        TIDY_MESSAGE_TYPE_LAST,      0,   NULL
    }
} };

//////////////////////
///////////////////////////////
/**
*  This language-specific function returns the correct pluralForm
*  to use given n items, and is used as a member of each language
*  definition.
*/
static uint whichPluralForm_en_gb(uint n) {
    /* Plural-Forms: nplurals=2; */
    return n != 1;
}


/**
*  This structure specifies all of the strings needed by Tidy for a
*  single language. Static definition in a header file makes it
*  easy to include and exclude languages without tinkering with
*  the build system.
*/
static languageDefinition language_en_gb = { whichPluralForm_en_gb,{
    /***************************************
    ** This MUST be present and first.
    ** Specify the code for this language.
    ***************************************/
    {/* Specify the ll or ll_cc language code here. */
        TIDY_LANGUAGE,                    0, "en_gb"
    },

    {/* This MUST be present and last. */
        TIDY_MESSAGE_TYPE_LAST,           0, NULL
    }

} };

///////////////////////////////
///////////////////////////////////////////////
static uint whichPluralForm_es(uint n) {
    /* Plural-Forms: nplurals=2; */
    return n != 1;
}


/**
*  This structure specifies all of the strings needed by Tidy for a
*  single language. Static definition in a header file makes it
*  easy to include and exclude languages without tinkering with
*  the build system.
*/
static languageDefinition language_es = { whichPluralForm_es,{
    /***************************************
    ** This MUST be present and first.
    ** Specify the code for this language.
    ***************************************/
    {/* Specify the ll or ll_cc language code here. */
        TIDY_LANGUAGE,          0, "es"
    },
    {/* This MUST be present and last. */
        TIDY_MESSAGE_TYPE_LAST, 0, NULL
    }

} };

static languageDefinition language_es_mx = { whichPluralForm_es,{
    /***************************************
    ** This MUST be present and first.
    ** Specify the code for this language.
    ***************************************/
    {/* Specify the ll or ll_cc language code here. */
        TIDY_LANGUAGE,          0, "es_mx"
    },
    {/* This MUST be present and last. */
        TIDY_MESSAGE_TYPE_LAST, 0, NULL
    }

} };

static languageDefinition language_pt_br = { whichPluralForm_es,{
    /***************************************
    ** This MUST be present and first.
    ** Specify the code for this language.
    ***************************************/
    {/* Specify the ll or ll_cc language code here. */
        TIDY_LANGUAGE,          0, "pt_br"
    },
    {/* This MUST be present and last. */
        TIDY_MESSAGE_TYPE_LAST, 0, NULL
    }

} };

///////////////////////////////////////////////
////////////////////////////////////////////////////////
static uint whichPluralForm_zh_cn(uint n) {
    /* Plural-Forms: nplurals=1; */
    return 0;
}


/**
*  This structure specifies all of the strings needed by Tidy for a
*  single language. Static definition in a header file makes it
*  easy to include and exclude languages without tinkering with
*  the build system.
*/
static languageDefinition language_zh_cn = { whichPluralForm_zh_cn,{
    /***************************************
    ** This MUST be present and first.
    ** Specify the code for this language.
    ***************************************/
    {/* Specify the ll or ll_cc language code here. */
        TIDY_LANGUAGE,          0, "zh_cn"
    },

    {/* This MUST be present and last. */
        TIDY_MESSAGE_TYPE_LAST, 0, NULL
    }

} };


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
static uint whichPluralForm_fr(uint n) {
    /* Plural-Forms: nplurals=2; */
    return (n > 1);
}


/**
*  This structure specifies all of the strings needed by Tidy for a
*  single language. Static definition in a header file makes it
*  easy to include and exclude languages without tinkering with
*  the build system.
*/
static languageDefinition language_fr = { whichPluralForm_fr,{
    /***************************************
    ** This MUST be present and first.
    ** Specify the code for this language.
    ***************************************/
    {/* Specify the ll or ll_cc language code here. */
        TIDY_LANGUAGE,                                 0, "fr"
    },
    {/* This MUST be present and last. */
        TIDY_MESSAGE_TYPE_LAST,                        0, NULL
    }

} };


////////////////////////////////////////////////////////////////////
/**
*  This single structure contains all localizations. Note that we preset
*  `.currentLanguage` to language_en, which is Tidy's default language.
*/
static tidyLanguagesType tidyLanguages = {
    no,           /* library language was NOT manually set */
    &language_en, /* current language */
    &language_en, /* first fallback language */
    {
        /* Required localization! */
        &language_en,
// #if SUPPORT_LOCALIZATIONS
        /* These additional languages are installed. */
        &language_en_gb,
        &language_es,
        &language_es_mx,
        &language_pt_br,
        &language_zh_cn,
        &language_fr,
// #endif
        NULL /* This array MUST be null terminated. */
    }
};




///////////////////////////////////////////////////

/**
*  The function getNextWindowsLanguage() returns pointers to this type;
*  it gives LibTidy implementors the ability to determine how Windows
*  locale names are mapped to POSIX language codes.
*/
typedef struct tidyLocaleMapItemImpl {
    ctmbstr winName;
    ctmbstr POSIXName;
} tidyLocaleMapItemImpl;

/**
*  This structure maps old-fashioned Windows strings
*  to proper POSIX names (modern Windows already uses
*  POSIX names).
*/
static const tidyLocaleMapItemImpl localeMappings[] = {
    { "america",                "en_us" },
    { "american english",       "en_us" },
    { "american-english",       "en_us" },
    { "american",               "en_us" },
    { "aus",                    "en_au" },
    { "australia",              "en_au" },
    { "australian",             "en_au" },
    { "austria",                "de_at" },
    { "aut",                    "de_at" },
    { "bel",                    "nl_be" },
    { "belgian",                "nl_be" },
    { "belgium",                "nl_be" },
    { "bra",                    "pt-br" },
    { "brazil",                 "pt-br" },
    { "britain",                "en_gb" },
    { "can",                    "en_ca" },
    { "canada",                 "en_ca" },
    { "canadian",               "en_ca" },
    { "che",                    "de_ch" },
    { "china",                  "zh_cn" },
    { "chinese-simplified",     "zh" },
    { "chinese-traditional",    "zh_tw" },
    { "chinese",                "zh" },
    { "chn",                    "zh_cn" },
    { "chs",                    "zh" },
    { "cht",                    "zh_tw" },
    { "csy",                    "cs" },
    { "cze",                    "cs_cz" },
    { "czech",                  "cs_cz" },
    { "dan",                    "da" },
    { "danish",                 "da" },
    { "dea",                    "de_at" },
    { "denmark",                "da_dk" },
    { "des",                    "de_ch" },
    { "deu",                    "de" },
    { "dnk",                    "da_dk" },
    { "dutch-belgian",          "nl_be" },
    { "dutch",                  "nl" },
    { "ell",                    "el" },
    { "ena",                    "en_au" },
    { "enc",                    "en_ca" },
    { "eng",                    "eb_gb" },
    { "england",                "en_gb" },
    { "english-american",       "en_us" },
    { "english-aus",            "en_au" },
    { "english-can",            "en_ca" },
    { "english-nz",             "en_nz" },
    { "english-uk",             "eb_gb" },
    { "english-us",             "en_us" },
    { "english-usa",            "en_us" },
    { "english",                "en" },
    { "enu",                    "en_us" },
    { "enz",                    "en_nz" },
    { "esm",                    "es-mx" },
    { "esn",                    "es" },
    { "esp",                    "es" },
    { "fin",                    "fi" },
    { "finland",                "fi_fi" },
    { "finnish",                "fi" },
    { "fra",                    "fr" },
    { "france",                 "fr_fr" },
    { "frb",                    "fr_be" },
    { "frc",                    "fr_ca" },
    { "french-belgian",         "fr_be" },
    { "french-canadian",        "fr_ca" },
    { "french-swiss",           "fr_ch" },
    { "french",                 "fr" },
    { "frs",                    "fr_ch" },
    { "gbr",                    "en_gb" },
    { "german-austrian",        "de_at" },
    { "german-swiss",           "de_ch" },
    { "german",                 "de" },
    { "germany",                "de_de" },
    { "grc",                    "el_gr" },
    { "great britain",          "en_gb" },
    { "greece",                 "el_gr" },
    { "greek",                  "el" },
    { "hkg",                    "zh_hk" },
    { "holland",                "nl_nl" },
    { "hong kong",              "zh_hk" },
    { "hong-kong",              "zh_hk" },
    { "hun",                    "hu" },
    { "hungarian",              "hu" },
    { "hungary",                "hu_hu" },
    { "iceland",                "is_is" },
    { "icelandic",              "is" },
    { "ireland",                "en_ie" },
    { "irl",                    "en_ie" },
    { "isl",                    "is" },
    { "ita",                    "it" },
    { "ita",                    "it_it" },
    { "italian-swiss",          "it_ch" },
    { "italian",                "it" },
    { "italy",                  "it_it" },
    { "its",                    "it_ch" },
    { "japan",                  "ja_jp" },
    { "japanese",               "ja" },
    { "jpn",                    "ja" },
    { "kor",                    "ko" },
    { "korea",                  "ko_kr" },
    { "korean",                 "ko" },
    { "mex",                    "es-mx" },
    { "mexico",                 "es-mx" },
    { "netherlands",            "nl_nl" },
    { "new zealand",            "en_nz" },
    { "new-zealand",            "en_nz" },
    { "nlb",                    "nl_be" },
    { "nld",                    "nl" },
    { "non",                    "nn" },
    { "nor",                    "nb" },
    { "norway",                 "no" },
    { "norwegian-bokmal",       "nb" },
    { "norwegian-nynorsk",      "nn" },
    { "norwegian",              "no" },
    { "nz",                     "en_nz" },
    { "nzl",                    "en_nz" },
    { "plk",                    "pl" },
    { "pol",                    "pl-pl" },
    { "poland",                 "pl-pl" },
    { "polish",                 "pl" },
    { "portugal",               "pt-pt" },
    { "portuguese-brazil",      "pt-br" },
    { "portuguese",             "pt" },
    { "pr china",               "zh_cn" },
    { "pr-china",               "zh_cn" },
    { "prt",                    "pt-pt" },
    { "ptb",                    "pt-br" },
    { "ptg",                    "pt" },
    { "rus",                    "ru" },
    { "russia",                 "ru-ru" },
    { "russian",                "ru" },
    { "sgp",                    "zh_sg" },
    { "singapore",              "zh_sg" },
    { "sky",                    "sk" },
    { "slovak",                 "sk" },
    { "spain",                  "es-es" },
    { "spanish-mexican",        "es-mx" },
    { "spanish-modern",         "es" },
    { "spanish",                "es" },
    { "sve",                    "sv" },
    { "svk",                    "sk-sk" },
    { "swe",                    "sv-se" },
    { "sweden",                 "sv-se" },
    { "swedish",                "sv" },
    { "swiss",                  "de_ch" },
    { "switzerland",            "de_ch" },
    { "taiwan",                 "zh_tw" },
    { "trk",                    "tr" },
    { "tur",                    "tr-tr" },
    { "turkey",                 "tr-tr" },
    { "turkish",                "tr" },
    { "twn",                    "zh_tw" },
    { "uk",                     "en_gb" },
    { "united kingdom",         "en_gb" },
    { "united states",          "en_us" },
    { "united-kingdom",         "en_gb" },
    { "united-states",          "en_us" },
    { "us",                     "en_us" },
    { "usa",                    "en_us" },

    /* MUST be last. */
    { NULL,                     NULL }
};

#define ISUPPER(a) ((a >= 'A') && (a <= 'Z'))

/* Transform ASCII chars in string to lower case */
tmbstr TY_tmbstrtolower(tmbstr s)
{
    tmbstr cp;
    for (cp = s; *cp; ++cp) {
        //*cp = (tmbchar)TY_(ToLower)(*cp);
        if (ISUPPER(*cp)) {
            *cp += 'a' - 'A';
        }
    }
    return s;
}


/**
*  Retrieves the POSIX name for a string. Result is a static char so please
*  don't try to free it. If the name looks like a cc_ll identifier, we will
*  return it if there's no other match.
*  @note this routine uses default allocator, see tidySetMallocCall.
*/
tmbstr TY_tidyNormalizedLocaleName(ctmbstr locale)
{
    uint i;
    uint len;
    static char result[6] = "xx_yy";
    // TidyAllocator * allocator = &TY_(g_default_allocator);

    //tmbstr search = TY_(tmbstrdup)(allocator, locale);
    //search = TY_(tmbstrtolower)(search);
    tmbstr search = strdup( locale );
    if (!search) {
        return result;
    }
    search = TY_tmbstrtolower(search);

    /* See if our string matches a Windows name. */
    for (i = 0; localeMappings[i].winName; ++i)
    {
        if (strcmp(localeMappings[i].winName, search) == 0)
        {
            //TidyFree(allocator, search);
            free(search);
            // search = TY_(tmbstrdup)(allocator, localeMappings[i].POSIXName);
            search = strdup( localeMappings[i].POSIXName );
            if (!search) {
                return result;
            }
            break;
        }
    }

    /* We're going to be stupid about this and trust the user, and
    return just the first two characters if they exist and the
    4th and 5th if they exist. The worst that can happen is a
    junk language that doesn't exist and won't be set. */

    len = (uint)strlen(search);
    len = (len <= 5 ? len : 5);

    for (i = 0; i < len; i++)
    {
        if (i == 2)
        {
            /* Either terminate the string or ensure there's an underscore */
            if (len == 5) {
                result[i] = '_';
            }
            else {
                result[i] = '\0';
                break;      /* no need to copy after null */
            }
        }
        else
        {
            //result[i] = tolower(search[i]);
            result[i] = search[i];
        }
    }

    //TidyFree(allocator, search);
    free(search);
    return result;
}

/**
*  Returns the languageDefinition if the languageCode is installed in Tidy,
*  otherwise return NULL
*/
languageDefinition *TY_tidyTestLanguage(ctmbstr languageCode)
{
    uint i;
    languageDefinition *testLang;
    languageDictionary *testDict;
    ctmbstr testCode;

    printf("  Test langcode '%s' ", languageCode);
    for (i = 0; tidyLanguages.languages[i]; ++i)
    {
        testLang = tidyLanguages.languages[i];
        testDict = &testLang->messages;
        testCode = (*testDict)[0].value;

        if (strcmp(testCode, languageCode) == 0) {
            printf("match found\n");
            return testLang;
        }
    }
    printf("no match!\n");
    return NULL;
}



/**
*  Tells Tidy to use a different language for output.
*  @param  languageCode A Windows or POSIX language code, and must match
*          a TIDY_LANGUAGE for an installed language.
*  @result Indicates that a setting was applied, but not necessarily the
*          specific request, i.e., true indicates a language and/or region
*          was applied. If es_mx is requested but not installed, and es is
*          installed, then es will be selected and this function will return
*          true. However the opposite is not true; if es is requested but
*          not present, Tidy will not try to select from the es_XX variants.
*/

Bool TY_tidySetLanguage(ctmbstr languageCode)
{
    languageDefinition *dict1 = NULL;
    languageDefinition *dict2 = NULL;
    tmbstr wantCode = NULL;
    char lang[3] = "";

    if (!languageCode || !(wantCode = TY_tidyNormalizedLocaleName(languageCode)))
    {
        return no;
    }

    /* We want to use the specified language as the currentLanguage, and set
    fallback language as necessary. We have either a two or five digit code,
    either or both of which might be installed. Let's test both of them:
    */

    dict1 = TY_tidyTestLanguage(wantCode);  /* WANTED language */

    if (strlen(wantCode) > 2)
    {
        strncpy(lang, wantCode, 2);
        lang[2] = '\0';
        dict2 = TY_tidyTestLanguage(lang); /* BACKUP language? */
    }

    if (dict1 && dict2)
    {
        tidyLanguages.currentLanguage = dict1;
        tidyLanguages.fallbackLanguage = dict2;
    }
    if (dict1 && !dict2)
    {
        tidyLanguages.currentLanguage = dict1;
        tidyLanguages.fallbackLanguage = NULL;
    }
    if (!dict1 && dict2)
    {
        tidyLanguages.currentLanguage = dict2;
        tidyLanguages.fallbackLanguage = NULL;
    }
    if (!dict1 && !dict2)
    {
        /* No change. */
    }

    return dict1 || dict2;
}

/**
*  Gets the current language used by Tidy.
*/
ctmbstr TY_tidyGetLanguage(void)
{
    languageDefinition *langDef = tidyLanguages.currentLanguage;
    languageDictionary *langDict = &langDef->messages;
    return (*langDict)[0].value;
}



/* CODE EXTRACTED FROM TIDY */
/* ==========================================================  */

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
    ctmbstr tl;

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

    if (TY_tidySetLanguage(lc_all)) {
        tl = TY_tidyGetLanguage();
        printf("SET tidy lang to '%s'...\n", tl);
    }
    else {
        tl = TY_tidyGetLanguage();
        printf("Default tidy lang is '%s'...\n", tl);
    }

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
