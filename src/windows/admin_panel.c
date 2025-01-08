
#include "admin_panel.h"

#include "panel_window.h"
#include "window_menager.h"

#include <regex.h>
#include <ctype.h>

void on_add_employee_page_clicked(GtkButton *button)
{
    GtkWidget *add_employee_page = create_add_employee_page();
    update_current_panel(add_employee_page);
}

GtkWidget* create_add_employee_page()
{
    GtkWidget *add_employee_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    
    GtkWidget *first_name_entry = gtk_entry_new();
    GtkWidget *last_name_entry = gtk_entry_new();
    GtkWidget *email_entry = gtk_entry_new();
    GtkWidget *password_entry = gtk_entry_new();
    GtkWidget *confirm_password_entry = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(first_name_entry), "Imię");
    gtk_entry_set_placeholder_text(GTK_ENTRY(last_name_entry), "Nazwisko");
    gtk_entry_set_placeholder_text(GTK_ENTRY(email_entry), "Email");
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Hasło");
    gtk_entry_set_placeholder_text(GTK_ENTRY(confirm_password_entry), "Powtórz hasło");

    GtkWidget *add_button = gtk_button_new_with_label("Dodaj pracownika");
    GtkWidget *error_label = gtk_label_new("");

    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_entry_set_visibility(GTK_ENTRY(confirm_password_entry), FALSE);
    gtk_widget_set_sensitive(error_label, FALSE);

    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_employee_clicked), NULL);

    gtk_box_append(GTK_BOX(add_employee_box), first_name_entry);
    gtk_box_append(GTK_BOX(add_employee_box), last_name_entry);
    gtk_box_append(GTK_BOX(add_employee_box), email_entry);
    gtk_box_append(GTK_BOX(add_employee_box), password_entry);
    gtk_box_append(GTK_BOX(add_employee_box), confirm_password_entry);
    gtk_box_append(GTK_BOX(add_employee_box), error_label);
    gtk_box_append(GTK_BOX(add_employee_box), add_button);

    // Zapisz referencje do widgetów, aby móc je później wykorzystać
    app_data.entries[0] = first_name_entry;
    app_data.entries[1] = last_name_entry;
    app_data.entries[2] = email_entry;
    app_data.entries[3] = password_entry;
    app_data.entries[4] = confirm_password_entry;
    app_data.entries[5] = error_label;

    return add_employee_box;
}

void on_add_employee_clicked(GtkButton *button)
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
    user.is_admin = true;  // Ustawienie uprawnień admina
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
    
    // Wyświetl informację o pomyślnym dodaniu pracownika
    GtkWidget *success_label = gtk_label_new("Pracownik został pomyślnie dodany!");
    gtk_box_append(GTK_BOX(current_panel), success_label);
}

GtkWidget* load_admin_panel()
{
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *notebook = gtk_notebook_new();
    GtkWidget *admin_label = gtk_label_new("Admin Panel");
    GtkWidget *admin_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *user_label = gtk_label_new("User Tab");
    GtkWidget *user_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *content_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);


    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), admin_content, admin_label);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), user_content, user_label);
    gtk_box_append(GTK_BOX(main_box), notebook);
    gtk_box_append(GTK_BOX(main_box), content_container);
    g_signal_connect(notebook, "switch-page", G_CALLBACK(on_tab_switched), content_container);

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);

    return main_box;
}

void on_tab_switched(GtkNotebook *notebook, GtkWidget *page, guint page_num, gpointer user_data)
{
    GtkWidget *content_container = GTK_WIDGET(user_data);

    // Usunięcie poprzedniej zawartości
    GtkWidget *child;
    while ((child = gtk_widget_get_first_child(content_container)) != NULL) {
        gtk_box_remove(GTK_BOX(content_container), child);
    }

    // Dodanie nowej zawartości
    if (page_num == 0) {
        // Zakładka Admin Panel
        GtkWidget *admin_panel = create_admin_panel();
        gtk_box_append(GTK_BOX(content_container), admin_panel);
    } else if (page_num == 1) {
        // Zakładka User Tab
        GtkWidget *user_panel = load_panel_screen();
        gtk_box_append(GTK_BOX(content_container), user_panel);
    }
}

GtkWidget* create_admin_sidebar()
{
    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    
    const char *formatted_text = g_strdup_printf("Witaj\n%s, %s!", current_user.first_name, current_user.last_name);
    GtkWidget *label1 = gtk_label_new(formatted_text);
    GtkWidget *label2 = gtk_label_new("\n");

    GtkWidget *add_employee_button = gtk_button_new_with_label("Dodaj pracownika");
    GtkWidget *manage_books_button = gtk_button_new_with_label("Zarządzaj książkami");

    GtkWidget *label3 = gtk_label_new("\n");
    GtkWidget *logout_button = gtk_button_new_with_label("Logout");

    g_signal_connect(add_employee_button, "clicked", G_CALLBACK(on_add_employee_page_clicked), NULL);
    //g_signal_connect(manage_books_button, "clicked", G_CALLBACK(on_manage_books_clicked), NULL);
    g_signal_connect(logout_button, "clicked", G_CALLBACK(on_logout_clicked), NULL);

    gtk_box_append(GTK_BOX(sidebar), label1);
    gtk_box_append(GTK_BOX(sidebar), label2);
    gtk_box_append(GTK_BOX(sidebar), add_employee_button);
    gtk_box_append(GTK_BOX(sidebar), manage_books_button);
    gtk_box_append(GTK_BOX(sidebar), label3);
    gtk_box_append(GTK_BOX(sidebar), logout_button);

    return sidebar;
}
GtkWidget* create_admin_panel()
{
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    // Dodanie sidebaru
    GtkWidget *sidebar = create_admin_sidebar();

    // Dodanie głównego obszaru zawartości
    GtkWidget *content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *welcome_label = gtk_label_new("Witaj w panelu admina!");
    
    gtk_box_append(GTK_BOX(main_box), sidebar);
    gtk_box_append(GTK_BOX(content_area), welcome_label);

    gtk_box_append(GTK_BOX(main_box), content_area);
    return main_box;
}