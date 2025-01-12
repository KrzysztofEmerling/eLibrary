#include "eLibrary.h"

#include <stdbool.h>


#ifndef UTILITY_H
#define UTILITY_H

bool validate_email(const char* email,const char** error_message);
bool validate_last_name(const char* name,const char** error_message);
bool validate_first_name(const char* name,const char** error_message);
bool validate_password(const char* password, const char* confirm_password,const char** error_message);




#endif 