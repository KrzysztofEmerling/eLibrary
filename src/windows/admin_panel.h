#include <gtk/gtk.h>
#include "../eLibrary.h"

#ifndef ADMIN_PANEL_H
#define ADMIN_PANEL_H

GtkWidget* load_admin_panel();

void on_tab_switched(GtkNotebook *notebook, GtkWidget *page, guint page_num, gpointer user_data);
GtkWidget* create_admin_panel();

GtkWidget* create_add_employee_page();
void on_add_employee_clicked(GtkButton *button);
void on_add_employee_page_clicked(GtkButton *button);
void on_add_employee_page_clicked(GtkButton *button);


#endif // ADMIN_PANEL_H