
#include "window_menager.h"
#include "login_window.h"

#include <regex.h>
#include <ctype.h>



void on_register_clicked(GtkButton *button) 
{
    change_window(REGISTER);
}

void on_login_clicked(GtkButton *button) 
{
    change_window(LOGIN);
}

void on_login_submit_clicked(GtkButton *button)
{
    const char *email = gtk_editable_get_text(GTK_EDITABLE(app_data.entries[0]));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(app_data.entries[1]));

    uint8_t hash[32];
    hash_password(password, hash);

    User_node* wsk = app_data.Users;
    while (wsk != NULL)
    {
        if(strcmp(wsk->user_info.email, email) == 0) break;
        wsk = wsk->next;
    }
    
    if ( wsk != NULL && secure_compare_hashes(wsk->user_info.password_hash, hash)) 
    {
        app_data.current_user_id = wsk->user_info.id;
        if(wsk->user_info.is_admin) change_window(ADMIN_PANEL);
        else change_window(NORMAL_PANEL);
    } 
    else 
    {
        gtk_label_set_text(GTK_LABEL(app_data.entries[2]), "*Błędny e-mail lub hasło.");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[0]), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[1]), "");
    }
}

void on_register_submit_clicked(GtkButton *button)
{
    const char *first_name = gtk_editable_get_text(GTK_EDITABLE(app_data.entries[0]));
    const char *last_name = gtk_editable_get_text(GTK_EDITABLE(app_data.entries[1]));
    const char *email = gtk_editable_get_text(GTK_EDITABLE(app_data.entries[2]));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(app_data.entries[3]));
    const char *confirm_password = gtk_editable_get_text(GTK_EDITABLE(app_data.entries[4]));

    User user;
    user.id = users_counter;
    strcpy(user.first_name, first_name);
    strcpy(user.last_name, last_name);
    strcpy(user.email, email);
    user.is_admin = false;
    memset(user.borrowed_books_ids, -1, sizeof(user.borrowed_books_ids));
    hash_password(password, user.password_hash);

    for (int i = 0; user.email[i]; i++) 
    {
        user.email[i] = tolower(user.email[i]);
    }

    // Walidacja imienia
    if (!isalpha(first_name[0]) || !isalnum(first_name[0])) 
    {
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[0]), "");
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "Imię musi zaczynać się od litery.");
        return;
    }
    user.first_name[0] = toupper(user.first_name[0]); 

    // Walidacja nazwiska
    if (!isalpha(last_name[0]) || !isalnum(last_name[0])) 
    {
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[1]), "");
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "Nazwisko musi zaczynać się od litery.");
        return;
    }
    user.last_name[0] = toupper(user.last_name[0]); 

    // Walidacja emaila
    regex_t regex;
    int reti = regcomp(&regex, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$", REG_EXTENDED);
    if (reti)
    {
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "Błąd przy sprawdzaniu emaila.");
        return;
    }
    reti = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);
    if (reti) 
    {
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[2]), "");
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "Niepoprawny format emaila.");
        return;
    }

    // Sprawdzenie hasła
    if (strcmp(password, confirm_password) != 0)
    {
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "Hasła nie pasują do siebie.");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[3]), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[4]), "");
        return;
    }
    if(strlen(password) < 6)
    {
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "Minimalna długość hasła to 6 znaków.");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[3]), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[4]), "");  
        return;
    }

    add_user(&(app_data.Users), user);
    change_window(REGISTRATION_INFO);
}

GtkWidget* load_login_screen()
{
    GtkWidget *login_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    app_data.entries[0] = gtk_entry_new();
    app_data.entries[1] = gtk_entry_new();
    
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data.entries[0]), "Nazwa użytkownika");
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data.entries[1]), "Hasło");

    GtkWidget *login_button = gtk_button_new_with_label("Zaloguj się");
    GtkWidget *register_button = gtk_button_new_with_label("Nie masz konta? Zarejestruj się");
    app_data.entries[2] = gtk_label_new("");

    gtk_entry_set_visibility(GTK_ENTRY(app_data.entries[1]), FALSE);
    gtk_widget_set_sensitive(app_data.entries[2], FALSE);

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_submit_clicked), NULL);
    g_signal_connect(register_button, "clicked", G_CALLBACK(on_register_clicked), NULL);

    gtk_box_append(GTK_BOX(login_box), app_data.entries[0]);
    gtk_box_append(GTK_BOX(login_box), app_data.entries[1]);
    gtk_box_append(GTK_BOX(login_box), app_data.entries[2]);
    gtk_box_append(GTK_BOX(login_box), login_button);
    gtk_box_append(GTK_BOX(login_box), register_button);

    return login_box;
}

GtkWidget* load_registration_screen()
{
    GtkWidget *register_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    app_data.entries[0] = gtk_entry_new();
    app_data.entries[1] = gtk_entry_new();
    app_data.entries[2] = gtk_entry_new();
    app_data.entries[3] = gtk_entry_new();
    app_data.entries[4] = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data.entries[0]), "Imię");
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data.entries[1]), "Nazwisko");
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data.entries[2]), "Email");
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data.entries[3]), "Hasło");
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data.entries[4]), "Powtórz hasło");

    GtkWidget *register_button = gtk_button_new_with_label("Zarejestruj");
    GtkWidget *login_button = gtk_button_new_with_label("Powrót do logowania");
    
    app_data.entries[5] = gtk_label_new("");

    gtk_entry_set_visibility(GTK_ENTRY(app_data.entries[3]), FALSE);
    gtk_entry_set_visibility(GTK_ENTRY(app_data.entries[4]), FALSE);
    gtk_widget_set_sensitive(app_data.entries[5], FALSE);

    g_signal_connect(register_button, "clicked", G_CALLBACK(on_register_submit_clicked), NULL);
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), NULL);

    gtk_box_append(GTK_BOX(register_box), app_data.entries[0]);
    gtk_box_append(GTK_BOX(register_box), app_data.entries[1]);
    gtk_box_append(GTK_BOX(register_box), app_data.entries[2]);
    gtk_box_append(GTK_BOX(register_box), app_data.entries[3]);
    gtk_box_append(GTK_BOX(register_box), app_data.entries[4]);
    gtk_box_append(GTK_BOX(register_box), app_data.entries[5]);
    gtk_box_append(GTK_BOX(register_box), register_button);
    gtk_box_append(GTK_BOX(register_box), login_button);

    return register_box;
}

GtkWidget* load_registration_info_screen()
{
    GtkWidget *register_info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *label1 = gtk_label_new("Sukces!");
    GtkWidget *label2 = gtk_label_new("Konto zostało założone");
    GtkWidget *login_button = gtk_button_new_with_label("Powrót do logowania");

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), NULL);

    gtk_box_append(GTK_BOX(register_info_box), label1);
    gtk_box_append(GTK_BOX(register_info_box), label2);
    gtk_box_append(GTK_BOX(register_info_box), login_button);
    return register_info_box;
}

