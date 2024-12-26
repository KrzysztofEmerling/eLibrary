#include <gtk/gtk.h>

#include "eLibrary.h"

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
    User_node* Users_data = NULL;
    User user;

    user.id = 0;
    user.is_admin = true;
    strcpy(user.email, "a.admin@library.com");
    strcpy(user.last_name, "Emerling");
    strcpy(user.first_name, "Krzysztof");
    hash_password("Admin123!", user.password_hash);

    user.borrowed_books_ids[0] = -1;
    user.borrowed_books_ids[1] = -1;
    user.borrowed_books_ids[2] = -1;

    add_user(&Users_data, user);

    Book_node* Books_data = NULL;
    import_books_data(&Books_data, "books.txt");

    GtkApplication *app = gtk_application_new("com.simple.library.app.gtk4", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    save_books_info(Books_data);
    save_users_info(Users_data);

    return status;
}
