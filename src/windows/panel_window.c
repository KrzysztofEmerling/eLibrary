#include "panel_window.h"

#include "window_menager.h"


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


void on_change_email_button_clicked(GtkButton *button)
{

}

void on_change_passward_button_clicked(GtkButton *button)
{
    
}

void on_delete_button_clicked(GtkButton *button)
{
    
}


GtkWidget* create_borrowed_books_page()
{
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    
    for (int i = 0; i < 3; i++) 
    {
        GtkWidget *item_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        GtkWidget *book_label = NULL;
        GtkWidget *return_button = NULL;

        if(current_user.borrowed_books_ids[i] >= 0)
        {
            Book_node *wsk = app_data.Books;
            while(wsk != NULL)
            {
                if(wsk->book_info.id == current_user.borrowed_books_ids[i]) break;
                wsk = wsk->next;
            }
            if(wsk == NULL) continue;

            char* formatted_text = g_strdup_printf("Książka %d:\nTytuł: %s\nAutor: %s\nGatunek: %s\n", 
                                                   i + 1, wsk->book_info.title, wsk->book_info.author, wsk->book_info.genre);
            book_label = gtk_label_new(formatted_text);
            return_button = gtk_button_new_with_label("Oddaj");
        }
        else
        {
            book_label = gtk_label_new("Możesz jeszcze wypożyczyć książkę");
            return_button = gtk_button_new_with_label("Oddaj");
            gtk_widget_set_sensitive(return_button, FALSE);
        }
        
        gtk_box_append(GTK_BOX(item_box), book_label);
        gtk_box_append(GTK_BOX(item_box), return_button);
        gtk_box_append(GTK_BOX(item_box), gtk_label_new("\n"));
        
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
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    
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

    
    //g_signal_connect(change_email_button, "clicked", G_CALLBACK(on_change_email), app_data.entries[0]);
    
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
    gtk_box_append(GTK_BOX(box), app_data.entries[5]);
    
    //g_signal_connect(change_password_button, "clicked", G_CALLBACK(on_change_password), (gpointer)(GtkEntry *[]){old_password_entry, new_password_entry1, new_password_entry2});
    
    // Sekcja usuwania konta
    GtkWidget *delete_label = gtk_label_new("Usuń konto:");
    GtkWidget *delete_button = gtk_button_new_with_label("Usuń konto");
    app_data.entries[6] = gtk_label_new("");

    
    gtk_box_append(GTK_BOX(box), delete_label);
    gtk_box_append(GTK_BOX(box), delete_button);
    gtk_box_append(GTK_BOX(box), app_data.entries[6]);

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


