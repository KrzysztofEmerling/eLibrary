#include "panel_window.h"
#include "window_menager.h"
void on_logout_clicked()
{
    app_data.current_user_id = -1;
    change_window(LOGIN);
}

GtkWidget* load_panel_screen()
{
    GtkWidget *panel_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    User_node* wsk = app_data.Users;
    while(wsk != NULL)
    {
        if(wsk->user_info.id == app_data.current_user_id) break;
        wsk = wsk->next;
    }

    const char *formatted_text = g_strdup_printf("Witaj %s, %s!", wsk->user_info.first_name, wsk->user_info.last_name);
    GtkWidget *label1 = gtk_label_new(formatted_text );
    GtkWidget *logout_button = gtk_button_new_with_label("Logout");

    g_signal_connect(logout_button, "clicked", G_CALLBACK(on_logout_clicked), NULL);

    gtk_box_append(GTK_BOX(panel_box), label1);
    gtk_box_append(GTK_BOX(panel_box), logout_button);

    return panel_box;
}