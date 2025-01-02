#include <gtk/gtk.h>

#ifndef LOGIN_WINDOW
#define LOGIN_WINDOW

void on_register_clicked(GtkButton *button);
void on_login_clicked(GtkButton *button);

void on_login_submit_clicked(GtkButton *button);
void on_register_submit_clicked(GtkButton *button);

GtkWidget* load_login_screen();
GtkWidget* load_registration_screen();
GtkWidget* load_registration_info_screen();

#endif