#include <gtk/gtk.h>
#include "../eLibrary.h"

#ifndef WINDOW_MENAGER
#define WINDOW_MENAGER

typedef enum {
    LOGIN,
    REGISTER
} Window_type;

typedef struct app_data
{
    GtkWindow* window;

    Book_node* Books;
    User_node* Users;

    User Current_user;

}App_data;

void change_window(App_data* app_data, Window_type wt);

//void on_login_clicked(GtkButton *button, App_data* app_data);
GtkWidget* load_login_screen(App_data* app_data);
//void on_register_clicked(GtkButton *button, App_data* app_data);
//bool is_valid_user(App_data* app_data, const gchar* email, const gchar* password);

GtkWidget* load_register_screen(App_data* app_data);
#endif