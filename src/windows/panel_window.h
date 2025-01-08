#include <gtk/gtk.h>
#include "../eLibrary.h"

#ifndef PANEL_WINDOW
#define PANEL_WINDOW



void on_logout_clicked();
void update_current_panel(GtkWidget *new_panel);
void on_borrowed_books_clicked();
void on_search_books_clicked();
void on_profile_management_clicked();


void on_return_button_clicked(GtkButton *button, Book_node *book_wsk);

void on_change_email_button_clicked(GtkButton *button);
void on_change_passward_button_clicked(GtkButton *button);
void on_delete_button_clicked(GtkButton *button);
void on_borrow_book_button_clicked(GtkButton *button, Book_node *book_wsk);
void on_buy_book_button_clicked(GtkButton *button, Book_node *book_wsk);
void on_confirmed_buy(GtkButton *button, Book_node *book_wsk);
void on_sortbooks_button_clicked(GtkButton *button);
void on_borrow_book_button_clicked(GtkButton *button, Book_node *book_wsk);

void create_book_list_subpage();
GtkWidget* create_purchase_subpage(bool is_sukcess, char *message, Book_node *book_wsk);
GtkWidget* create_borrow_book_info_subpage(bool is_sukcess, char *message);
GtkWidget* create_borrowed_books_page();
GtkWidget* create_search_books_page();
GtkWidget* create_profile_management_page();

GtkWidget* load_panel_screen();
GtkWidget* load_deletion_info_screen();


#endif