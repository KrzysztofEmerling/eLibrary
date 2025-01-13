#include <gtk/gtk.h>
#include "../eLibrary.h"

#ifndef ADMIN_PANEL_H
#define ADMIN_PANEL_H

GtkWidget* load_admin_panel();

void on_tab_switched(GtkNotebook *notebook, GtkWidget *page, guint page_num, gpointer user_data);
GtkWidget* create_admin_panel();

GtkWidget* create_add_employee_page();
GtkWidget* create_admin_sidebar();
void on_add_employee_clicked(GtkButton *button);
void on_add_employee_page_clicked(GtkButton *button);

GtkWidget* create_book_import_panel();
void on_drop(GtkDropTarget *target, const GValue *value, double x, double y, gpointer user_data);
GtkWidget* create_employee_management_panel();
void on_delete_admin_clicked(GtkButton *button, gpointer user_data);
void on_degrade_admin_clicked(GtkButton *button, User* user);


#endif // ADMIN_PANEL_H