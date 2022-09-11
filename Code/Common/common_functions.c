#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "common_functions.h"


// _locale suggest value: zh_CN.UTF-8
void SetConsoleLocale(const char* _locale)
{
    setlocale(LC_ALL, _locale);
}
