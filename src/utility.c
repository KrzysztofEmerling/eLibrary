#include "utility.h"

#include <ctype.h>
#include <regex.h>

static const char* error_messages[] = {
    "Błąd przy sprawdzaniu emaila.", 
    "Niepoprawny format emaila.",

    "Nazwisko musi zaczynać się od litery.",

    "Imie musi zaczynać się od litery.",

    "Hasła nie pasują do siebie.",
    "Minimalna długość hasła to 6 znaków."
    };
    
bool validate_email(const char* email, const char** error_message)
{
    regex_t regex;
    int reti = regcomp(&regex, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$", REG_EXTENDED);
    if (reti)
    {
        *error_message = error_messages[0];
        return false;
    }
    reti = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);
    if (reti) 
    {
        *error_messages = error_messages[1];
        return false;
    }

    return true;
}
bool validate_last_name(const char* name, const char** error_message)
{
    if (!isalpha(name[0]) || !isalnum(name[0])) 
    {
        *error_message = error_messages[2];
        return false;
    }

    return true;
}
bool validate_first_name(const char* name, const char** error_message)
{
    if (!isalpha(name[0]) || !isalnum(name[0])) 
    {
        *error_message = error_messages[3];
        return false;
    }

    return true;
}
bool validate_password(const char* password, const char* confirm_password, const char** error_message)
{
    if (strcmp(password, confirm_password) != 0)
    {
        *error_message = error_messages[4];
        return false;
    }
    if(strlen(password) < 6)
    {
        *error_message = error_messages[5];
        return false;
    }

    return true;
}
