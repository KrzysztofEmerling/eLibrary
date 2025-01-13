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

void split(const char* str, char* tocken1, char* tocken2, char* tocken3, char* tocken4, char* tocken5, char* tocken6, char* tocken7, const char delimiter)
{
    const char* current = str;
    int delim_counter = 0;
    while(*current != '\0')
    {
        if(*current++ == delimiter) ++delim_counter;
    }
    if(delim_counter == 7)
    {
        current = str;
        while(*current != delimiter)
        {
            *tocken1++ = *current++;
        }
        *tocken1 = '\0';
        ++current;

        while(*current != delimiter)
        {
            *tocken2++ = *current++;
        }
        *tocken2 = '\0';
        ++current;

        while(*current != delimiter)
        {
            *tocken3++ = *current++;
        }
        *tocken3 = '\0';
        ++current;

        while(*current != delimiter)
        {
            *tocken4++ = *current++;
        }
        *tocken4 = '\0';
        ++current;

        while(*current != delimiter)
        {
            *tocken5++ = *current++;
        }
        *tocken5 = '\0';
        ++current;

        while(*current != delimiter)
        {
            *tocken6++ = *current++;
        }
        *tocken6 = '\0';
        ++current;

        while(*current != '\0')
        {
            *tocken7++ = *current++;
        }
        *tocken7 = '\0';
    }
}