#include "panel_window.h"

#include "window_menager.h"
#include <regex.h>
#include <ctype.h>

static GtkWidget *current_panel;
static GtkWidget *panel_box; 
static User current_user;

void on_logout_clicked()
{
    app_data.current_user_id = -1;
    change_window(LOGIN);
}

void update_current_panel(GtkWidget *new_panel)
{
    if (current_panel != NULL) {
        gtk_widget_unparent(current_panel);
    }
    current_panel = new_panel; 
    gtk_box_append(GTK_BOX(panel_box), current_panel); 
    gtk_widget_set_visible(current_panel, TRUE); 
}

void on_borrowed_books_clicked()
{
    update_current_panel(create_borrowed_books_page());
}

void on_search_books_clicked()
{
    update_current_panel(create_search_books_page());
}

void on_profile_management_clicked()
{
    update_current_panel(create_profile_management_page());
}


void on_return_button_clicked(GtkButton *button, Book *book)
{
    int index = 0;
    while( index < 2)
    {
        if(current_user.borrowed_books_ids[index] == book->id) break;     
        ++index;
    }
    current_user.borrowed_books_ids[index] = -1;
    book->available_copies += 1;
     
    User_node *wsk = app_data.Users;
    while(wsk != NULL)
    {
       if(wsk->user_info.id == current_user.id) break;
       wsk = wsk->next;
    }
    wsk->user_info.borrowed_books_ids[index] = -1;

    gtk_label_set_text(GTK_LABEL(app_data.entries[0 + 2 * index]), "Możesz wypożyczyć książkę");
    gtk_widget_set_sensitive(app_data.entries[1 + 2 * index], FALSE);
}

void on_change_email_button_clicked(GtkButton *button)
{
    gtk_label_set_text(GTK_LABEL(app_data.entries[1]), "");
    gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "");
    gtk_label_set_text(GTK_LABEL(app_data.entries[6]), "");

    const char *temp =  gtk_editable_get_text(GTK_EDITABLE(app_data.entries[0]));
    char email[128];
    strncpy(email, temp, sizeof(email) - 1);
    email[sizeof(email) - 1] = '\0'; 

    for (int i = 0; email[i]; i++) 
    {
        email[i] = tolower(email[i]);
    }

    regex_t regex;
    int reti = regcomp(&regex, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$", REG_EXTENDED);
    if (reti)
    {
        gtk_label_set_text(GTK_LABEL(app_data.entries[1]), "Błąd przy sprawdzaniu emaila.");
        return;
    }
    reti = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);
    if (reti) 
    {
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[0]), "");
        gtk_label_set_text(GTK_LABEL(app_data.entries[1]), "Niepoprawny format emaila.");
        
    }
    else
    {
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[0]), "");
        gtk_label_set_text(GTK_LABEL(app_data.entries[1]), "Email został zmieniony.");

        User_node* wsk = app_data.Users;

        while (wsk != NULL) 
        {
            if (wsk->user_info.id == current_user.id) 
            {
                strcpy(wsk->user_info.email, email);
                return; 
            }
            wsk = wsk->next;
        }
    }
}

void on_change_passward_button_clicked(GtkButton *button)
{
    gtk_label_set_text(GTK_LABEL(app_data.entries[1]), "");
    gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "");
    gtk_label_set_text(GTK_LABEL(app_data.entries[6]), "");

    const char* old_pass = gtk_editable_get_text(GTK_EDITABLE(app_data.entries[2]));
    uint8_t old_hash[32];
    hash_password(old_pass, old_hash);

    const char* new_pass1 = gtk_editable_get_text(GTK_EDITABLE(app_data.entries[3]));
    const char* new_pass2 = gtk_editable_get_text(GTK_EDITABLE(app_data.entries[4]));

    User_node* wsk = app_data.Users;
    while (wsk != NULL) 
    {
        if (wsk->user_info.id == current_user.id) break;
        wsk = wsk->next;
    }

    // Sprawdzenie hasła
    if(!secure_compare_hashes(wsk->user_info.password_hash, old_hash))
    {
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "Stare hasło nie jest poprawne.");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[2]), "");     
        return;
    }
    else if (strcmp(new_pass1 , new_pass2) != 0)
    {
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "Hasła nie pasują do siebie.");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[3]), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[4]), "");
        return;
    }
    else if(strlen(new_pass1 ) < 6)
    {
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "Minimalna długość hasła to 6 znaków.");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[3]), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[4]), "");  
        return;
    }

    uint8_t new_hash[32];
    hash_password(new_pass1, new_hash);
    for(int i = 0; i < 32; i++)
    {
        wsk->user_info.password_hash[i] = new_hash[i];
    }

}

void on_delete_button_clicked(GtkButton *button)
{
    gtk_label_set_text(GTK_LABEL(app_data.entries[1]), "");
    gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "");
    gtk_label_set_text(GTK_LABEL(app_data.entries[6]), "");

    if(current_user.borrowed_books_ids[0] == -1 && current_user.borrowed_books_ids[1] == -1 && current_user.borrowed_books_ids[2] == -1)
    {
        delete_user(&(app_data.Users) ,current_user.id);
        change_window(DELETION_INFO);
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(app_data.entries[6]), "Nie można usunąć konta.\nOddaj wszystkie wyporzyczone książki.");
    }
}


GtkWidget* create_borrowed_books_page()
{
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    for (int i = 0; i < 3; i++) 
    {
        GtkWidget *item_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        Book_node *wsk;
        bool is_empty = true;

        if(current_user.borrowed_books_ids[i] >= 0)
        {
            wsk = app_data.Books;
            while(wsk != NULL)
            {
                if(wsk->book_info.id == current_user.borrowed_books_ids[i]) break;
                wsk = wsk->next;
            }

            if(wsk == NULL) continue;

            is_empty = false;
            char* formatted_text = g_strdup_printf("Książka %d:\nTytuł: %s\nAutor: %s\nGatunek: %s\n", 
                                                   i + 1, wsk->book_info.title, wsk->book_info.author, wsk->book_info.genre);
            app_data.entries[0 + 2 * i] = gtk_label_new(formatted_text);
            app_data.entries[1 + 2 * i] = gtk_button_new_with_label("Oddaj");

            g_free(formatted_text);

        }
        else
        {
            app_data.entries[0 + 2 * i] = gtk_label_new("Możesz wypożyczyć książkę");
            app_data.entries[1 + 2 * i] = gtk_button_new_with_label("Oddaj");
            gtk_widget_set_sensitive(app_data.entries[1 + 2 * i], FALSE);
        }
        
        gtk_box_append(GTK_BOX(item_box), app_data.entries[0 + 2 * i]);
        gtk_box_append(GTK_BOX(item_box), app_data.entries[1 + 2 * i]);
        gtk_box_append(GTK_BOX(item_box), gtk_label_new("\n"));
        if(!is_empty) g_signal_connect(app_data.entries[1 + 2 * i], "clicked", G_CALLBACK(on_return_button_clicked), &(wsk->book_info));
        
        gtk_box_append(GTK_BOX(main_box), item_box);
    }

    return main_box;
}

GtkWidget* create_search_books_page()
{
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    
    // Pasek wyszukiwania
    GtkWidget *search_label = gtk_label_new("Wyszukaj książki:");
    GtkWidget *search_entry = gtk_entry_new();
    GtkWidget *search_button = gtk_button_new_with_label("Szukaj");
    GtkWidget *search_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *search_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    gtk_box_append(GTK_BOX(search_hbox), search_entry);
    gtk_box_append(GTK_BOX(search_hbox), search_button);
    
    gtk_box_append(GTK_BOX(search_box), search_label);
    gtk_box_append(GTK_BOX(search_box), search_hbox);

    // Filtr
    GtkWidget *filter_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *filter_label = gtk_label_new("Filtruj zakres:");
    GtkWidget *filter_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *min_entry = gtk_entry_new();
    GtkWidget *max_entry = gtk_entry_new();

    GtkStringList *filter_options = gtk_string_list_new((const char*[]){"Rok wydania", "Dostępne egzęplarze", "Cena", NULL});
    GtkWidget *filter_combo = gtk_drop_down_new(G_LIST_MODEL(filter_options), NULL);

    gtk_box_append(GTK_BOX(filter_hbox), gtk_label_new("Od:"));
    gtk_box_append(GTK_BOX(filter_hbox), min_entry);
    gtk_box_append(GTK_BOX(filter_hbox), gtk_label_new("Do:"));
    gtk_box_append(GTK_BOX(filter_hbox), max_entry);
    gtk_box_append(GTK_BOX(filter_hbox), filter_combo);
    
    gtk_box_append(GTK_BOX(filter_box), filter_label);
    gtk_box_append(GTK_BOX(filter_box), filter_hbox);

    // Sortowanie
    GtkWidget *sort_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *sort_label = gtk_label_new("Sortuj według:");
    GtkWidget *sort_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    GtkStringList *sort_options = gtk_string_list_new((const char*[]){"Tytuł", "Autor", "Gatunek", "Rok wydania", "cena", NULL});
    GtkWidget *sort_combo = gtk_drop_down_new(G_LIST_MODEL(sort_options), NULL);
    GtkWidget *radio_asc = gtk_check_button_new_with_label("⬆");
    GtkWidget *radio_desc = gtk_check_button_new_with_label("⬇");
    GtkWidget *sort_button = gtk_button_new_with_label("Sortuj");
    gtk_check_button_set_group(GTK_CHECK_BUTTON(radio_desc), GTK_CHECK_BUTTON(radio_asc));

    gtk_box_append(GTK_BOX(sort_hbox), sort_combo);
    gtk_box_append(GTK_BOX(sort_hbox), radio_asc);
    gtk_box_append(GTK_BOX(sort_hbox), radio_desc);
    gtk_box_append(GTK_BOX(sort_hbox), sort_button);

    gtk_box_append(GTK_BOX(sort_box), sort_label);
    gtk_box_append(GTK_BOX(sort_box), sort_hbox);

    // Dodanie do głównego boxa
    gtk_box_append(GTK_BOX(main_box), search_box);
    gtk_box_append(GTK_BOX(main_box), filter_box);
    gtk_box_append(GTK_BOX(main_box), sort_box);

    return main_box;
}

GtkWidget* create_profile_management_page() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    
    GtkWidget *profile_label = gtk_label_new("Zarządzanie profilem użytkownika");
    gtk_box_append(GTK_BOX(box), profile_label);

    // Sekcja zmiany e-maila
    GtkWidget *email_label = gtk_label_new("Zmiana e-maila:");
    app_data.entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data.entries[0]), "Wprowadź nowy e-mail");   
    GtkWidget *change_email_button = gtk_button_new_with_label("Zmień e-mail");
    app_data.entries[1] = gtk_label_new("");

    gtk_box_append(GTK_BOX(box), email_label); 
    gtk_box_append(GTK_BOX(box), app_data.entries[0]);
    gtk_box_append(GTK_BOX(box), change_email_button);
    gtk_box_append(GTK_BOX(box), app_data.entries[1]);
    g_signal_connect(change_email_button, "clicked", G_CALLBACK(on_change_email_button_clicked), NULL);
    
    // Sekcja zmiany hasła
    GtkWidget *password_label = gtk_label_new("Zmiana hasła:");
    app_data.entries[2] = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(app_data.entries[2]), FALSE); 
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data.entries[2]), "Wprowadź stare hasło");

    app_data.entries[3] = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(app_data.entries[3]), FALSE); 
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data.entries[3]), "Wprowadź nowe hasło");

    app_data.entries[4] = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(app_data.entries[4]), FALSE); 
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data.entries[4]), "Powtórz nowe hasło");
    GtkWidget *change_password_button = gtk_button_new_with_label("Zmień hasło");
    app_data.entries[5] = gtk_label_new("");
    
    gtk_box_append(GTK_BOX(box), password_label);
    gtk_box_append(GTK_BOX(box), app_data.entries[2]);   
    gtk_box_append(GTK_BOX(box), app_data.entries[3]);  
    gtk_box_append(GTK_BOX(box), app_data.entries[4]);
    gtk_box_append(GTK_BOX(box), change_password_button);
    gtk_box_append(GTK_BOX(box), app_data.entries[5]);
    
    g_signal_connect(change_password_button, "clicked", G_CALLBACK(on_change_passward_button_clicked), NULL);
    
    // Sekcja usuwania konta
    GtkWidget *delete_label = gtk_label_new("Usuń konto:");
    GtkWidget *delete_button = gtk_button_new_with_label("Usuń konto");
    app_data.entries[6] = gtk_label_new("");

    
    gtk_box_append(GTK_BOX(box), delete_label);
    gtk_box_append(GTK_BOX(box), delete_button);
    gtk_box_append(GTK_BOX(box), app_data.entries[6]);

    g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_button_clicked), NULL);
    
    return box;
}

GtkWidget* load_panel_screen()
{
    panel_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); 
    
    User_node* wsk = app_data.Users;
    while(wsk != NULL)
    {
        if(wsk->user_info.id == app_data.current_user_id) break;
        wsk = wsk->next;
    }
    current_user.id = app_data.current_user_id;
    strcpy(current_user.first_name, wsk->user_info.first_name);
    strcpy(current_user.last_name, wsk->user_info.last_name);
    strcpy(current_user.email, wsk->user_info.email);

    current_user.is_admin = wsk->user_info.is_admin;
    current_user.borrowed_books_ids[0] = wsk->user_info.borrowed_books_ids[0];
    current_user.borrowed_books_ids[1] = wsk->user_info.borrowed_books_ids[1];
    current_user.borrowed_books_ids[2] = wsk->user_info.borrowed_books_ids[2];

    const char *formatted_text = g_strdup_printf("Witaj\n%s, %s!",current_user.first_name , current_user.last_name);
    
    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *label1 = gtk_label_new(formatted_text);
    GtkWidget *label2 = gtk_label_new("\n");
    
    GtkWidget *borrowed_books_button = gtk_button_new_with_label("Wyporzyczone książki");
    GtkWidget *search_books_button = gtk_button_new_with_label("Szukaj książek");
    GtkWidget *profile_management_button = gtk_button_new_with_label("Profil");
    
    GtkWidget *label3 = gtk_label_new("\n");
    GtkWidget *logout_button = gtk_button_new_with_label("Logout");

    g_signal_connect(borrowed_books_button, "clicked", G_CALLBACK(on_borrowed_books_clicked), NULL);
    g_signal_connect(search_books_button, "clicked", G_CALLBACK(on_search_books_clicked), NULL);
    g_signal_connect(profile_management_button, "clicked", G_CALLBACK(on_profile_management_clicked), NULL);
    g_signal_connect(logout_button, "clicked", G_CALLBACK(on_logout_clicked), NULL);

    gtk_box_append(GTK_BOX(sidebar), label1);
    gtk_box_append(GTK_BOX(sidebar), label2);
    gtk_box_append(GTK_BOX(sidebar), borrowed_books_button);
    gtk_box_append(GTK_BOX(sidebar), search_books_button);
    gtk_box_append(GTK_BOX(sidebar), profile_management_button);
    gtk_box_append(GTK_BOX(sidebar), label3);
    gtk_box_append(GTK_BOX(sidebar), logout_button);

    current_panel = create_borrowed_books_page(); 
    gtk_box_append(GTK_BOX(panel_box), sidebar);
    gtk_box_append(GTK_BOX(panel_box), current_panel);

    return panel_box;
}

GtkWidget* load_deletion_info_screen()
{
    GtkWidget *register_info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *label1 = gtk_label_new("Sukces!");
    GtkWidget *label2 = gtk_label_new("Konto zostało usunięte");
    GtkWidget *login_button = gtk_button_new_with_label("Powrót do logowania");

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_logout_clicked), NULL);

    gtk_box_append(GTK_BOX(register_info_box), label1);
    gtk_box_append(GTK_BOX(register_info_box), label2);
    gtk_box_append(GTK_BOX(register_info_box), login_button);

    return register_info_box;
}
