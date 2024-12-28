#include "window_menager.h"

static GtkWidget* current_widget = NULL;

void on_register_clicked(GtkButton *button, App_data *app_data) {
    change_window(app_data, REGISTER);
}

void on_login_clicked(GtkButton *button, App_data *app_data) {
    
    change_window(app_data, LOGIN);
}

void on_login__submit_clicked() {//TODO: dodac logowanie
}
void on_register_submit_clicked(GtkButton *button, App_data *app_data) {//TODO: dodac rejestrowanie
}

GtkWidget* load_login_screen(App_data* app_data) {
    GtkWidget *login_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *email_entry = gtk_entry_new();
    GtkWidget *password_entry = gtk_entry_new();
    GtkWidget *login_button = gtk_button_new_with_label("Zaloguj się");
    GtkWidget *register_button = gtk_button_new_with_label("Nie masz konta? Zarejestruj się");
    GtkWidget *error_label = gtk_label_new("");
    
    // Ustawienia pól
    gtk_entry_set_placeholder_text(GTK_ENTRY(email_entry), "E-mail");
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Hasło");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); // Ukrycie hasła

    // Ustawienie stylu dla etykiety błędu
    gtk_widget_set_sensitive(error_label, FALSE); // Domyślnie nieaktywny

    // Połączenie sygnałów
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_register_submit_clicked), app_data);
    g_signal_connect(register_button, "clicked", G_CALLBACK(on_register_clicked), app_data);

    // Dodanie widżetów do kontenera
    gtk_box_append(GTK_BOX(login_box), email_entry);
    gtk_box_append(GTK_BOX(login_box), password_entry);
    gtk_box_append(GTK_BOX(login_box), error_label);
    gtk_box_append(GTK_BOX(login_box), login_button);
    gtk_box_append(GTK_BOX(login_box), register_button);

    return login_box;
}

GtkWidget* load_registration_screen(App_data* app_data) {
    GtkWidget *register_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *first_name_entry = gtk_entry_new();
    GtkWidget *last_name_entry = gtk_entry_new();
    GtkWidget *email_entry = gtk_entry_new();
    GtkWidget *password_entry = gtk_entry_new();
    GtkWidget *repeat_password_entry = gtk_entry_new();
    GtkWidget *register_button = gtk_button_new_with_label("Zarejestruj");
    GtkWidget *login_button = gtk_button_new_with_label("Powrót do logowania");
    GtkWidget *error_label = gtk_label_new("");

    // Ustawienia pól
    gtk_entry_set_placeholder_text(GTK_ENTRY(first_name_entry), "Imię");
    gtk_entry_set_placeholder_text(GTK_ENTRY(last_name_entry), "Nazwisko");
    gtk_entry_set_placeholder_text(GTK_ENTRY(email_entry), "E-mail");
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Hasło");
    gtk_entry_set_placeholder_text(GTK_ENTRY(repeat_password_entry), "Powtórz hasło");
    
    // Ustawienie stylu dla etykiety błędu
    gtk_widget_set_sensitive(error_label, FALSE); // Domyślnie nieaktywny

    // Połączenie sygnałów
    g_signal_connect(register_button, "clicked", G_CALLBACK(on_register_submit_clicked), app_data);
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), app_data);

    // Dodanie widżetów do kontenera
    gtk_box_append(GTK_BOX(register_box), first_name_entry);
    gtk_box_append(GTK_BOX(register_box), last_name_entry);
    gtk_box_append(GTK_BOX(register_box), email_entry);
    gtk_box_append(GTK_BOX(register_box), password_entry);
    gtk_box_append(GTK_BOX(register_box), repeat_password_entry);
    gtk_box_append(GTK_BOX(register_box), error_label);
    gtk_box_append(GTK_BOX(register_box), register_button);
    gtk_box_append(GTK_BOX(register_box), login_button);

    return register_box;
}

void change_window(App_data* app_data, Window_type wt) {
    GtkWidget *new_widget = NULL;

    // Usuwamy poprzedni widget
    if (current_widget != NULL) {
        gtk_widget_unparent(current_widget);
    }

    // Wybieramy, który ekran wyświetlić
    switch(wt)
    {
        case LOGIN:
        new_widget = load_login_screen(app_data);
        break;
        case REGISTER:
        new_widget = load_registration_screen(app_data);
        break;
    }

    gtk_window_set_child(GTK_WINDOW(app_data->window), new_widget);
    current_widget = new_widget;
}
