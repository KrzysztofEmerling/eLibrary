#include <gtk/gtk.h>

#ifndef PANEL_WINDOW
#define PANEL_WINDOW



void on_logout_clicked();
void update_current_panel(GtkWidget *new_panel);
void on_borrowed_books_clicked();
void on_search_books_clicked();
void on_profile_management_clicked();

void on_change_email_button_clicked(GtkButton *button);
void on_change_passward_button_clicked(GtkButton *button);
void on_delete_button_clicked(GtkButton *button);

GtkWidget* create_borrowed_books_page();
GtkWidget* create_search_books_page();
GtkWidget* create_profile_management_page();

GtkWidget* load_panel_screen();
GtkWidget* load_deletion_info_screen();


#endif