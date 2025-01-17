#include "window_menager.h"

#include "login_window.h"
#include "panel_window.h"
#include "admin_panel.h"

static GtkWidget* current_widget = NULL;
App_data app_data;

void change_window(Window_type wt)
{
    GtkWidget *new_widget = NULL;

    if (current_widget != NULL) {
        gtk_widget_unparent(current_widget);
        current_widget = NULL;
    }


    switch (wt)
    {
        case LOGIN:
            new_widget = load_login_screen();
            break;
        case REGISTER:
            new_widget = load_registration_screen();
            break;
        case REGISTRATION_INFO:
            new_widget = load_registration_info_screen();
            break;

        case NORMAL_PANEL:
            new_widget = load_panel_screen();
            break;
        case DELETION_INFO:
            new_widget = load_deletion_info_screen();
            break;

        case ADMIN_PANEL:
            new_widget = load_admin_panel();
            // new_widget = load_panel_screen();
    }

    gtk_window_set_child(GTK_WINDOW(app_data.window), new_widget);
    current_widget = new_widget;
}
