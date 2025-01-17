#ifndef ELIBRARY_H
#define ELIBRARY_H

#define USERS_DATA_PATH "data/users.bin"
#define BOOKS_DATA_PATH "data/books.bin"

#include <gtk/gtk.h>
#include<stdint.h>
#include<stdbool.h>

extern int books_counter; 

typedef struct book
{
    int id;     

    char author[64];          
    char title[256];          
    char genre[64];           
    int publication_year;     

    int total_copies;         
    int available_copies;     
    float retail_price;       
} Book;

typedef struct book_node Book_node;
typedef struct book_node
{
    Book book_info;
    Book_node* next;    
}Book_node;

int import_books_data(Book_node** head, char* path);

void load_books_info(Book_node** head);
void save_books_info(Book_node*  head);

void add_book(Book_node** head, Book book);
void delete_book(Book_node** head, int id);

extern int users_counter; 
typedef struct user
{
    int id;

    char first_name[64];        
    char last_name[64];

    bool is_admin;
    char email[128]; 
    uint8_t password_hash[32]; 

    int borrowed_books_ids[3];  
} User;

typedef struct user_node User_node;
typedef struct user_node
{
    User user_info;
    User_node* next;    
}User_node;


extern User current_user;

void hash_password(const char* plain_password, uint8_t* hashed_password);
bool secure_compare_hashes(const uint8_t hash1[32], const uint8_t hash2[32]);

void load_users_info(User_node** head);
void save_users_info(User_node*  head);

void add_user(User_node** head, User user);
void delete_user(User_node** head, int id);

void print_books(Book_node* head);
void print_users(User_node* head);


void swap_books(Book* a, Book* b);

typedef enum {
    SORT_BY_TITLE,
    SORT_BY_AUTHOR,
    SORT_BY_GENRE,
    SORT_BY_PRICE,
    SORT_BY_YEAR
} SortType;

void sort_books(Book_node* head, SortType sort_type, bool asc);

#endif // ELIBRARY_H