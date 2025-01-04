#include "window_menager.h"

#include "login_window.h"
#include "panel_window.h"

static GtkWidget* current_widget = NULL;
App_data app_data;

void change_window(Window_type wt)
{
    GtkWidget *new_widget = NULL;

    if (current_widget != NULL) {
        gtk_widget_unparent(current_widget);
    }

    // for (int i = 0; i < 10; i++) 
    // {
    //     if (GTK_IS_WIDGET(app_data.entries[i])) 
    //     {
    //         gtk_widget_unparent(app_data.entries[i]);  
    //         app_data.entries[i] = NULL;              
    //     }
    // }

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
    }

    gtk_window_set_child(GTK_WINDOW(app_data.window), new_widget);
    current_widget = new_widget;
}
