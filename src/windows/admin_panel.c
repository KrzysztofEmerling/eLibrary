
#include "admin_panel.h"
#include "window_menager.h"
#include "../utility.h"
#include "panel_window.h"
#include <ctype.h>

static GtkWidget *employee_list_box;

void create_admin_list_subpage()
{
    // Clear existing content
    GtkWidget *child;
    while ((child = gtk_widget_get_first_child(employee_list_box)) != NULL) {
        gtk_list_box_remove(GTK_LIST_BOX(employee_list_box), child);
    }

    User_node *user_wsk = app_data.Users;
    while (user_wsk != NULL)
    {
        if (user_wsk->user_info.is_admin)
        {
            GtkWidget *admin_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
            const char *formatted_text = g_strdup_printf("%s %s (%s)", 
                user_wsk->user_info.first_name, 
                user_wsk->user_info.last_name, 
                user_wsk->user_info.email);

            GtkWidget *admin_label = gtk_label_new(formatted_text);

            GtkWidget *delete_button = gtk_button_new_with_label("Usuń");
            g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_admin_clicked), GINT_TO_POINTER(user_wsk->user_info.id));

            if (user_wsk->user_info.id == current_user.id) 
            {
                gtk_widget_set_sensitive(delete_button, FALSE);
            }

            gtk_box_append(GTK_BOX(admin_box), admin_label);
            gtk_box_append(GTK_BOX(admin_box), delete_button);
            gtk_list_box_append(GTK_LIST_BOX(employee_list_box), admin_box);
        }
        user_wsk = user_wsk->next;
    }

    gtk_widget_set_visible(employee_list_box, TRUE);
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
    user.is_admin = true; 
    memset(user.borrowed_books_ids, -1, sizeof(user.borrowed_books_ids));
    hash_password(password, user.password_hash);

    for (int i = 0; user.email[i]; i++) 
    {
        user.email[i] = tolower(user.email[i]);
    }

    const char* error_msg;
    if (!validate_first_name(user.first_name, &error_msg)) 
    {
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[0]), "");
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), error_msg);
        return;
    }
    user.first_name[0] = toupper(user.first_name[0]); 
    if (!validate_last_name(user.last_name, &error_msg)) 
    {
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[1]), "");
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), error_msg);
        return;
    }
    user.last_name[0] = toupper(user.last_name[0]); 

    if (!validate_email(user.email, &error_msg)) 
    {
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[2]), "");
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), error_msg);
        return;
    }

    if (!validate_password(password, confirm_password, &error_msg)) 
    {
        gtk_label_set_text(GTK_LABEL(app_data.entries[5]), "Hasła nie pasują do siebie.");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[3]), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data.entries[4]), "");
        return;
    }

    add_user(&(app_data.Users), user);
    create_admin_list_subpage();

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
    GtkWidget *child;

    for(int i = 0; i < 10; i++)
    {
        if(app_data.entries[i] != NULL)
        {
            gtk_widget_unparent(app_data.entries[i]);
            app_data.entries[i] = NULL;
        }
    }
    while ((child = gtk_widget_get_first_child(content_container)) != NULL)
    {
        gtk_box_remove(GTK_BOX(content_container), child);
    }
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

    GtkWidget *label1 = gtk_label_new("ADMIN PANEL");

    const char *formatted_text = g_strdup_printf("Witaj\n%s, %s!", current_user.first_name, current_user.last_name);
    GtkWidget *label2 = gtk_label_new(formatted_text);
    GtkWidget *label3 = gtk_label_new("\n");


    gtk_box_append(GTK_BOX(sidebar), label1);
    gtk_box_append(GTK_BOX(sidebar), label2);
    gtk_box_append(GTK_BOX(sidebar), label3);

    return sidebar;
}
GtkWidget* create_admin_panel()
{
    GtkWidget *admin_panel = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    // Sidebar
    GtkWidget *sidebar = create_admin_sidebar();
    gtk_box_append(GTK_BOX(admin_panel), sidebar);

    // Main content area

    GtkWidget *content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // Upper panel for book import
    GtkWidget *import_panel = create_book_import_panel();
    gtk_box_append(GTK_BOX(content_area), import_panel);

    // Lower panel for employee management
    GtkWidget *employee_panel = create_employee_management_panel();

    gtk_box_append(GTK_BOX(content_area), employee_panel);
    gtk_box_append(GTK_BOX(admin_panel), content_area);

    return admin_panel;
}

GtkWidget* create_book_import_panel()
{
    GtkWidget *import_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *label = gtk_label_new("Przeciągnij plik z książkami tutaj");
    GtkWidget *drop_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drop_area, 300, 100);

    gtk_box_append(GTK_BOX(import_panel), label);
    gtk_box_append(GTK_BOX(import_panel), drop_area);

    GtkDropTarget *target = gtk_drop_target_new(G_TYPE_STRING, GDK_ACTION_COPY);
    gtk_drop_target_set_preload(target, TRUE);
    g_signal_connect(target, "drop", G_CALLBACK(on_drop), NULL);
    gtk_widget_add_controller(drop_area, GTK_EVENT_CONTROLLER(target));

    return import_panel;
}

void on_drop(GtkDropTarget *target, const GValue *value, double x, double y, gpointer user_data)
{
    if (G_VALUE_HOLDS(value, G_TYPE_STRING)) {
        const char *uri = g_value_get_string(value);
        char *filename = g_filename_from_uri(uri, NULL, NULL);
        if (filename != NULL) {
            import_books_data(&(app_data.Books), filename);

            char *new_filename = g_strconcat(filename, "-IMPORTED", NULL);
            rename(filename, new_filename);

            g_free(new_filename);
            g_free(filename);
        }
    }
}

GtkWidget* create_employee_management_panel()
{

    GtkWidget *employee_panel = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    // Panel dodawania pracownika
    GtkWidget *add_employee = create_add_employee_page();
    gtk_box_append(GTK_BOX(employee_panel), add_employee);

    employee_list_box = gtk_list_box_new();
    gtk_box_append(GTK_BOX(employee_panel), employee_list_box);
    create_admin_list_subpage();
    return employee_panel;
}


GtkWidget* create_admin_list()
{
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    GtkWidget *list_box = gtk_list_box_new();

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), list_box);

    User_node *current = app_data.Users;
    while (current != NULL) {
        if (current->user_info.is_admin) {
            GtkWidget *row = gtk_list_box_row_new();
            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

            gchar *label_text = g_strdup_printf("%s %s (%s)", 
                current->user_info.first_name, 
                current->user_info.last_name, 
                current->user_info.email);
            GtkWidget *label = gtk_label_new(label_text);
            g_free(label_text);

            GtkWidget *delete_button = gtk_button_new_with_label("Usuń");
            g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_admin_clicked), GINT_TO_POINTER(current->user_info.id));

            // Dezaktywuj przycisk usuwania dla bieżącego użytkownika
            if (current->user_info.id == current_user.id) {
                gtk_widget_set_sensitive(delete_button, FALSE);
            }

            gtk_box_append(GTK_BOX(hbox), label);
            gtk_box_append(GTK_BOX(hbox), delete_button);
            gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), hbox);
            gtk_list_box_insert(GTK_LIST_BOX(list_box), row, -1);
        }
        current = current->next;
    }

    return scrolled_window;
}


void on_delete_admin_clicked(GtkButton *button, gpointer user_data)
{
    int user_id = GPOINTER_TO_INT(user_data);
    delete_user(&(app_data.Users), user_id);
    // Odśwież listę adminów
    create_admin_list_subpage();
}