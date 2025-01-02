#include <gtk/gtk.h>
#include "../eLibrary.h"

#ifndef WINDOW_MENAGER
#define WINDOW_MENAGER

typedef enum {
    LOGIN,
    REGISTER,
    INFO
} Window_type;

typedef struct app_data
{
    GtkWindow* window;
    GtkWidget* entries[10];

    Book_node* Books;
    User_node* Users;
}App_data;
extern App_data app_data;

void change_window(Window_type wt);

GtkWidget* load_login_screen();

GtkWidget* load_register_screen();
#endif