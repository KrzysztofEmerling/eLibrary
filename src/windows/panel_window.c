#include "panel_window.h"

#include "window_menager.h"
#include "../utility.h"

#include <ctype.h>


static GtkWidget *panel_box; 

GtkWidget *books_list = NULL;


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


void on_return_button_clicked(GtkButton *button, Book_node *book_wsk)
{
    int index = 0;
    while( index < 2)
    {
        if(current_user.borrowed_books_ids[index] == book_wsk->book_info.id) break;     
        ++index;
    }
    current_user.borrowed_books_ids[index] = -1;
    book_wsk->book_info.available_copies += 1;
     
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

    const char* error_msg;
    if (!validate_email(email, &error_msg), error_msg) 
    {
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[0]), "");
        gtk_label_set_text(GTK_LABEL(app_data.entries[1]), error_msg);   
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

    const char* error_msg;
    if(!secure_compare_hashes(wsk->user_info.password_hash, old_hash))
    {
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "Stare hasło nie jest poprawne.");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[2]), "");     
        return;
    }
    else if (!validate_password(new_pass1, new_pass2, &error_msg))
    {
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), error_msg);
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

    gtk_editable_set_text(GTK_EDITABLE(app_data.entries[2]), "");     
    gtk_editable_set_text(GTK_EDITABLE(app_data.entries[3]), "");
    gtk_editable_set_text(GTK_EDITABLE(app_data.entries[4]), "");
    gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "Hasło zostało zmienione.");
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

void on_borrow_book_button_clicked(GtkButton *button, Book_node *book_wsk)
{

    bool first_slot_free = current_user.borrowed_books_ids[0] == -1;
    bool secend_slot_free = current_user.borrowed_books_ids[1] == -1;
    bool third_slot_free = current_user.borrowed_books_ids[2] == -1;
    if(book_wsk->book_info.available_copies < 1) 
    {
        update_current_panel(create_borrow_book_info_subpage(false, "Brak dostępnych egzęplarzy"));
    }
    else if(((!first_slot_free)  && book_wsk->book_info.id == current_user.borrowed_books_ids[0]) ||
            ((!secend_slot_free) && book_wsk->book_info.id == current_user.borrowed_books_ids[1]) ||
            ((!third_slot_free)  && book_wsk->book_info.id == current_user.borrowed_books_ids[2])  )
    {
        update_current_panel(create_borrow_book_info_subpage(false, "Wyporzyczyłeś już jeden egzęplarz tej książki"));
    }
    else if(first_slot_free || secend_slot_free || third_slot_free)
    {
        int index = 0;
        while(current_user.borrowed_books_ids[index] != -1) { ++index; }
        current_user.borrowed_books_ids[index] = book_wsk->book_info.id;

        User_node *wsk = app_data.Users;
        while(wsk != NULL)
        {
            if(wsk->user_info.id == current_user.id) break;
            wsk = wsk->next;
        }
        wsk->user_info.borrowed_books_ids[index] = current_user.borrowed_books_ids[index];

        book_wsk->book_info.available_copies -= 1;

        update_current_panel(create_borrow_book_info_subpage(true, ""));
    }
    else update_current_panel(create_borrow_book_info_subpage(false, "Możesz mieć maksymalnie 3 wypożyczone książki na raz!"));
}

void on_buy_book_button_clicked(GtkButton *button, Book_node *book_wsk)
{
    if(book_wsk->book_info.available_copies > 0)
    {
        update_current_panel(create_purchase_subpage(true, "", book_wsk));
    }
    else update_current_panel(create_purchase_subpage(false, "Aktualnie nie ma wolnych kopi książki", book_wsk));
    
}

void on_confirmed_buy(GtkButton *button, Book_node *book_wsk)
{
    book_wsk->book_info.available_copies -= 1;
    book_wsk->book_info.total_copies -= 1;

    if(book_wsk->book_info.total_copies < 1)
    {
        delete_book(&(app_data.Books), book_wsk->book_info.id);
    }

    update_current_panel(create_search_books_page());
}

void on_sortbooks_button_clicked(GtkButton *button)
{
    GtkWidget *dropdown = app_data.entries[0];
    GtkStringObject *selected_item = gtk_drop_down_get_selected_item(GTK_DROP_DOWN(dropdown));
    const char *selected_sort_option = gtk_string_object_get_string(selected_item);

    gboolean ascending = gtk_check_button_get_active(GTK_CHECK_BUTTON(app_data.entries[1]));

    // Sortowanie książek na podstawie wybranej opcji i kierunku
    if (g_strcmp0(selected_sort_option, "Tytuł") == 0) 
    {
        sort_books(app_data.Books, SORT_BY_TITLE, ascending);
    } 
    else if (g_strcmp0(selected_sort_option, "Autor") == 0)
    {
        sort_books(app_data.Books, SORT_BY_AUTHOR, ascending);
    } 
    else if(g_strcmp0(selected_sort_option, "Gatunek") == 0)
    {
        sort_books(app_data.Books, SORT_BY_GENRE, ascending);
    } 
    else if (g_strcmp0(selected_sort_option, "Rok wydania") == 0) 
    {
        sort_books(app_data.Books, SORT_BY_YEAR, ascending);
    } 
    else if (g_strcmp0(selected_sort_option, "Cena") == 0) 
    {
        sort_books(app_data.Books, SORT_BY_PRICE, ascending);
    }

    create_book_list_subpage();
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
        if(!is_empty) g_signal_connect(app_data.entries[1 + 2 * i], "clicked", G_CALLBACK(on_return_button_clicked), wsk);
        
        gtk_box_append(GTK_BOX(main_box), item_box);
    }

    return main_box;
}


GtkWidget* create_search_books_page()
{
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    
    // Sortowanie
    GtkWidget *sort_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *sort_label = gtk_label_new("Sortuj według:");
    GtkWidget *sort_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    GtkStringList *sort_options = gtk_string_list_new((const char*[]){"Tytuł", "Autor", "Gatunek", "Rok wydania", "Cena", NULL});
    app_data.entries[0] = gtk_drop_down_new(G_LIST_MODEL(sort_options), NULL);
    app_data.entries[1] = gtk_check_button_new_with_label("⬆");
    app_data.entries[2] = gtk_check_button_new_with_label("⬇");
    gtk_check_button_set_group(GTK_CHECK_BUTTON(app_data.entries[2]), GTK_CHECK_BUTTON(app_data.entries[1]));
    
    // Ustawiamy "ascending" (⬆) jako domyślnie zaznaczony
    gtk_check_button_set_active(GTK_CHECK_BUTTON(app_data.entries[1]), TRUE);

    GtkWidget *sort_button = gtk_button_new_with_label("Sortuj");
    gtk_check_button_set_group(GTK_CHECK_BUTTON(app_data.entries[2]), GTK_CHECK_BUTTON(app_data.entries[1]));

    gtk_box_append(GTK_BOX(sort_hbox), app_data.entries[0]);
    gtk_box_append(GTK_BOX(sort_hbox), app_data.entries[1]);
    gtk_box_append(GTK_BOX(sort_hbox), app_data.entries[2]);
    gtk_box_append(GTK_BOX(sort_hbox), sort_button);

    gtk_box_append(GTK_BOX(sort_box), sort_label);
    gtk_box_append(GTK_BOX(sort_box), sort_hbox);

    gtk_box_append(GTK_BOX(main_box), sort_box);
    gtk_box_append(GTK_BOX(main_box), gtk_label_new("\n"));

    app_data.entries[3] = gtk_label_new("");
    gtk_box_append(GTK_BOX(main_box), gtk_label_new("Books list:"));
    gtk_box_append(GTK_BOX(main_box), app_data.entries[3]);

    // Create a scrolled window to contain the book list
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scrolled_window, -1, 500); 

    books_list = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), books_list);

    create_book_list_subpage();
    gtk_box_append(GTK_BOX(main_box), scrolled_window);

    g_signal_connect(sort_button, "clicked", G_CALLBACK(on_sortbooks_button_clicked), NULL);

    return main_box;
}

void create_book_list_subpage()
{
    // Clear existing content
    GtkWidget *child;
    while ((child = gtk_widget_get_first_child(books_list)) != NULL) {
        gtk_box_remove(GTK_BOX(books_list), child);
    }

    Book_node *current_book = app_data.Books;
    while (current_book != NULL)
    {
        GtkWidget *book_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        const char *formatted_text1 = g_strdup_printf("%s, %s, rok %d", current_book->book_info.title, current_book->book_info.author, current_book->book_info.publication_year);
        const char *formatted_text2 = g_strdup_printf("Gatunek: %s, dostępnych: %d, cena detaliczna: %.2f $.", current_book->book_info.genre, current_book->book_info.available_copies, current_book->book_info.retail_price);
        
        GtkWidget *book_label1 = gtk_label_new(formatted_text1);
        GtkWidget *book_label2 = gtk_label_new(formatted_text2);
        GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        GtkWidget *rent_button = gtk_button_new_with_label("Wypożycz");
        GtkWidget *buy_button = gtk_button_new_with_label("Kup");
        
        gtk_box_append(GTK_BOX(book_box), book_label1);
        gtk_box_append(GTK_BOX(book_box), book_label2);
        gtk_box_append(GTK_BOX(button_box), rent_button);
        gtk_box_append(GTK_BOX(button_box), buy_button);
        gtk_box_append(GTK_BOX(book_box), button_box);
        
        g_signal_connect(buy_button, "clicked", G_CALLBACK(on_buy_book_button_clicked), current_book);
        g_signal_connect(rent_button, "clicked", G_CALLBACK(on_borrow_book_button_clicked), current_book);
        
        gtk_box_append(GTK_BOX(books_list), book_box);
        
        current_book = current_book->next;
    }

    gtk_widget_set_visible(books_list, TRUE);
}

GtkWidget* create_purchase_subpage(bool is_sukcess, char *message, Book_node *book_wsk)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); 
    GtkWidget *sherch_menu_button = gtk_button_new_with_label("Powrót do wyszukiwania");

    if(is_sukcess)
    {    
        GtkWidget *label1 = gtk_label_new("Czy chcesz zakupić książkę?");
    
        const char *formatted_text1 = g_strdup_printf("%s, %s, rok %d", book_wsk->book_info.title, book_wsk->book_info.author, book_wsk->book_info.publication_year);
        const char *formatted_text2 = g_strdup_printf("Gatunek: %s, dostępnych: %d, cena detaliczna: %.2f $.", book_wsk->book_info.genre, book_wsk->book_info.available_copies, book_wsk->book_info.retail_price);
        GtkWidget *label2 = gtk_label_new(formatted_text1);
        GtkWidget *label3 = gtk_label_new(formatted_text2);
        
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3); 
        GtkWidget *buy_button = gtk_button_new_with_label("Tak, kupuje.");
        
        gtk_box_append(GTK_BOX(hbox), buy_button);
        gtk_box_append(GTK_BOX(hbox), gtk_label_new(" "));
        gtk_box_append(GTK_BOX(hbox), sherch_menu_button);
        g_signal_connect(buy_button, "clicked", G_CALLBACK(on_confirmed_buy), book_wsk);
        
        gtk_box_append(GTK_BOX(box), label1);
        gtk_box_append(GTK_BOX(box), label2);
        gtk_box_append(GTK_BOX(box), label3);
        gtk_box_append(GTK_BOX(box), gtk_label_new(" "));
        gtk_box_append(GTK_BOX(box), hbox);
    }
    else
    {
        GtkWidget *label1 = gtk_label_new("Nie można zrealizować zakupu");
        GtkWidget *label2 = gtk_label_new(message);

        const char *formatted_text1 = g_strdup_printf("%s, %s, rok %d", book_wsk->book_info.title, book_wsk->book_info.author, book_wsk->book_info.publication_year);
        const char *formatted_text2 = g_strdup_printf("Gatunek: %s, dostępnych: %d, cena detaliczna: %.2f $.", book_wsk->book_info.genre, book_wsk->book_info.available_copies, book_wsk->book_info.retail_price);
        GtkWidget *label3 = gtk_label_new(formatted_text1);
        GtkWidget *label4 = gtk_label_new(formatted_text2);

        gtk_box_append(GTK_BOX(box), label1);
        gtk_box_append(GTK_BOX(box), label2);
        gtk_box_append(GTK_BOX(box), label3);
        gtk_box_append(GTK_BOX(box), label4);
        gtk_box_append(GTK_BOX(box), gtk_label_new(" "));
        gtk_box_append(GTK_BOX(box), sherch_menu_button);

    }
    g_signal_connect(sherch_menu_button, "clicked", G_CALLBACK(on_search_books_clicked), NULL);

    return box;
}

GtkWidget* create_borrow_book_info_subpage(bool is_sukcess, char *message)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); 
    if(is_sukcess)
    {        
        GtkWidget *label1 = gtk_label_new("Sukces!");
        GtkWidget *label2 = gtk_label_new("Książka została dodana do sekcji wypożyczonych książek");   

        gtk_box_append(GTK_BOX(box), label1);
        gtk_box_append(GTK_BOX(box), label2);
    }
    else
    {
        GtkWidget *label1 = gtk_label_new("Nie można dodać książek do sekcji wypożyczonych!");
        GtkWidget *label2 = gtk_label_new(message); 

        gtk_box_append(GTK_BOX(box), label1);
        gtk_box_append(GTK_BOX(box), label2);
    }

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3); 
    GtkWidget *borrowed_books_menu_button = gtk_button_new_with_label("Zobacz wypożyczone książki");
    GtkWidget *sherch_menu_button = gtk_button_new_with_label("Powrót do wyszukiwarki");

    gtk_box_append(GTK_BOX(hbox), borrowed_books_menu_button);
    gtk_box_append(GTK_BOX(hbox), gtk_label_new(" "));
    gtk_box_append(GTK_BOX(hbox), sherch_menu_button);
    g_signal_connect(borrowed_books_menu_button, "clicked", G_CALLBACK(on_borrowed_books_clicked), NULL);
    g_signal_connect(sherch_menu_button, "clicked", G_CALLBACK(on_search_books_clicked), NULL);
    
    gtk_box_append(GTK_BOX(box), hbox);


    return box;
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