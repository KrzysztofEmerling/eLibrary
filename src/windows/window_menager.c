#include "window_menager.h"

static GtkWidget* current_widget = NULL;

void on_register_clicked(GtkButton *button, App_data *app_data) {
    change_window(app_data, REGISTER);
}

void on_login_clicked(GtkButton *button, App_data *app_data) {
    change_window(app_data, LOGIN);
}

void on_login_submit_clicked(GtkButton *button, App_data *app_data) {
    const char *email = gtk_editable_get_text(GTK_EDITABLE(app_data->entries[0]));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(app_data->entries[1]));

    uint8_t hash[32];
    hash_password(password, hash);

    User_node* wsk = app_data->Users;
    while (wsk->next != NULL && !strcmp(wsk->user_info.email, email))
        wsk = wsk->next;
    
    if (strcmp(wsk->user_info.email, email) == 0 && secure_compare_hashes(wsk->user_info.password_hash, hash)) {
        g_print("poprawne logowanie");
    } else {
        gtk_label_set_text(GTK_LABEL(app_data->entries[2]), "*Błędny e-mail lub hasło.");
        gtk_editable_set_text(GTK_EDITABLE(app_data->entries[0]), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->entries[1]), "");
    }
}

void on_register_submit_clicked(GtkButton *button, App_data *app_data) {
    const char *first_name = gtk_editable_get_text(GTK_EDITABLE(app_data->entries[0]));
    const char *last_name = gtk_editable_get_text(GTK_EDITABLE(app_data->entries[1]));
    const char *email = gtk_editable_get_text(GTK_EDITABLE(app_data->entries[2]));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(app_data->entries[3]));
    const char *confirm_password = gtk_editable_get_text(GTK_EDITABLE(app_data->entries[4]));

    if (strcmp(password, confirm_password) != 0) {
        gtk_label_set_text(GTK_LABEL(app_data->entries[5]), "Hasła nie pasują do siebie.");
        gtk_editable_set_text(GTK_EDITABLE(app_data->entries[3]), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->entries[4]), "");
        return;
    }

    User user;
    user.id = users_counter;
    strcpy(user.first_name, first_name);
    strcpy(user.last_name, last_name);
    strcpy(user.email, email);
    user.is_admin = false;
    memset(user.borrowed_books_ids, -1, sizeof(user.borrowed_books_ids));

    hash_password(password, user.password_hash);

    add_user(&(app_data->Users), user);
    g_print("Dodano użytkownika");
}

GtkWidget* load_login_screen(App_data* app_data) {
    GtkWidget *login_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    app_data->entries[0] = gtk_entry_new();
    app_data->entries[1] = gtk_entry_new();
    GtkWidget *login_button = gtk_button_new_with_label("Zaloguj się");
    GtkWidget *register_button = gtk_button_new_with_label("Nie masz konta? Zarejestruj się");
    app_data->entries[2] = gtk_label_new("");

    // Ukrycie hasła
    gtk_entry_set_visibility(GTK_ENTRY(app_data->entries[1]), FALSE);

    // Ustawienie stylu dla etykiety błędu
    gtk_widget_set_sensitive(app_data->entries[2], FALSE);

    // Połączenie sygnałów
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_submit_clicked), app_data);
    g_signal_connect(register_button, "clicked", G_CALLBACK(on_register_clicked), app_data);

    // Dodanie widżetów do kontenera
    gtk_box_append(GTK_BOX(login_box), app_data->entries[0]);
    gtk_box_append(GTK_BOX(login_box), app_data->entries[1]);
    gtk_box_append(GTK_BOX(login_box), app_data->entries[2]);
    gtk_box_append(GTK_BOX(login_box), login_button);
    gtk_box_append(GTK_BOX(login_box), register_button);

    return login_box;
}

GtkWidget* load_registration_screen(App_data* app_data) {
    GtkWidget *register_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    app_data->entries[0] = gtk_entry_new();
    app_data->entries[1] = gtk_entry_new();
    app_data->entries[2] = gtk_entry_new();
    app_data->entries[3] = gtk_entry_new();
    app_data->entries[4] = gtk_entry_new();
    GtkWidget *register_button = gtk_button_new_with_label("Zarejestruj");
    GtkWidget *login_button = gtk_button_new_with_label("Powrót do logowania");
    app_data->entries[5] = gtk_label_new("");

    // Ukrycie hasła
    gtk_entry_set_visibility(GTK_ENTRY(app_data->entries[3]), FALSE);
    gtk_entry_set_visibility(GTK_ENTRY(app_data->entries[4]), FALSE);

    // Ustawienie stylu dla etykiety błędu
    gtk_widget_set_sensitive(app_data->entries[5], FALSE);

    // Połączenie sygnałów
    g_signal_connect(register_button, "clicked", G_CALLBACK(on_register_submit_clicked), app_data);
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), app_data);

    // Dodanie widżetów do kontenera
    gtk_box_append(GTK_BOX(register_box), app_data->entries[0]);
    gtk_box_append(GTK_BOX(register_box), app_data->entries[1]);
    gtk_box_append(GTK_BOX(register_box), app_data->entries[2]);
    gtk_box_append(GTK_BOX(register_box), app_data->entries[3]);
    gtk_box_append(GTK_BOX(register_box), app_data->entries[4]);
    gtk_box_append(GTK_BOX(register_box), app_data->entries[5]);
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
    switch (wt) {
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
