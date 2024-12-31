#include <gtk/gtk.h>
#include <stdio.h>  

#include "eLibrary.h"
#include "windows/window_menager.h"


static void on_activate(GtkApplication *app)
{

    app_data.window = GTK_WINDOW(gtk_application_window_new(app));
    gtk_window_set_title(app_data.window, "E-Library");
    gtk_window_set_decorated(app_data.window, TRUE);
    
    g_print("aktywacja");

    int width = 2560 * 0.5;
    int height = 1080 * 0.8;
    gtk_window_set_default_size(app_data.window, width, height);

    // Zmieniamy widok okna na ekran logowania
    change_window(LOGIN);

    // Wyświetlamy okno
    gtk_window_present(app_data.window);
}

int main(int argc, char *argv[]) {
    
    // Przydzielamy pamięć na listy użytkowników i książek
    app_data.Users = NULL;  // Początkowo wskaźnik jest NULL
    app_data.Books = NULL;  // Początkowo wskaźnik jest NULL

    // Ładujemy dane użytkowników i książek
    load_users_info(&app_data.Users); // Przekazujemy wskaźnik do wskaźnika (User_node**)
    load_books_info(&app_data.Books); // Przekazujemy wskaźnik do wskaźnika (Book_node**)

    // Tworzymy aplikację GTK
    GtkApplication *app = gtk_application_new("com.simple.library.app.gtk4", G_APPLICATION_DEFAULT_FLAGS);

    // Rejestrujemy sygnał aktywacji aplikacji
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Uruchamiamy aplikację
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    // Zwolnienie zasobów i zapis danych przed zakończeniem
    g_object_unref(app);
    save_books_info(app_data.Books);  // Zapisujemy książki
    save_users_info(app_data.Users);  // Zapisujemy użytkowników

    // Zwalniamy pamięć po zakończeniu
    free(app_data.Users);
    free(app_data.Books);

    return status;
}
