#include <gtk/gtk.h>
#include <stdio.h>  

#include "eLibrary.h"
#include "windows/window_menager.h"


void create_example_data()
{
    User user;
    user.id = users_counter;
    strcpy(user.first_name, "Krzysztof");
    strcpy(user.last_name, "Emerling");
    strcpy(user.email, "a.admin@library.com");
    user.is_admin = true;
    user.borrowed_books_ids[0] = -1;
    user.borrowed_books_ids[1] = -1;
    user.borrowed_books_ids[2] = -1;
    hash_password("Admin123!", user.password_hash);
    add_user(&(app_data.Users), user);
    save_users_info(app_data.Users);

    //import_books_data(&(app_data.Books), "books.txt");
    save_books_info(app_data.Books);
}

static void on_activate(GtkApplication *app)
{

    app_data.window = GTK_WINDOW(gtk_application_window_new(app));
    gtk_window_set_title(app_data.window, "E-Library");
    gtk_window_set_decorated(app_data.window, TRUE);
    
    int width = 2560 * 0.5;
    int height = 1080 * 0.8;
    gtk_window_set_default_size(app_data.window, width, height);

    change_window(LOGIN);
    gtk_window_present(app_data.window);

}

int main(int argc, char *argv[]) {
    
    app_data.Users = NULL;  
    app_data.Books = NULL;  
    app_data.current_user_id = -1;

    create_example_data();
    
    // load_users_info(&app_data.Users); 
    // load_books_info(&app_data.Books); 

    
    GtkApplication *app = gtk_application_new("com.simple.library.app.gtk4", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);


    g_object_unref(app);
    save_books_info(app_data.Books);  
    save_users_info(app_data.Users);  

    //print_users(app_data.Users);
    //print_books(app_data.Books);

    free(app_data.Users);
    free(app_data.Books);

    return status;
}
