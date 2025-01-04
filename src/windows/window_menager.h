#include <gtk/gtk.h>
#include "../eLibrary.h"

#ifndef WINDOW_MENAGER
#define WINDOW_MENAGER

typedef enum 
{
    LOGIN,
    REGISTER,
    REGISTRATION_INFO,
    ADMIN_PANEL,
    NORMAL_PANEL
} Window_type;

typedef struct app_data
{
    GtkWindow* window;
    GtkWidget* entries[10];

    Book_node* Books;
    User_node* Users;

    int current_user_id;
}App_data;

extern App_data app_data;

void change_window(Window_type wt);
#endif