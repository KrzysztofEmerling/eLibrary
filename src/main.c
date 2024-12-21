#include <gtk/gtk.h>

static void on_button_clicked(GtkButton *button, gpointer user_data)
 {
    g_print("Hello, World!\n");
}

static void on_activate(GtkApplication *app, gpointer user_data) 
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK 4 Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GtkWidget *button = gtk_button_new_with_label("Click Me!");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    gtk_window_set_child(GTK_WINDOW(window), button);


    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[])
{
    GtkApplication *app = gtk_application_new("com.simple.library.app.gtk4", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
