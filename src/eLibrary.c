#include"eLibrary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/sha.h>
#include <openssl/crypto.h>

void import_books_data(Book_node** head, char* path)
{
    FILE *file = fopen(path, "r"); 
    if (file == NULL) 
    {
        fprintf(stderr, "Unable to open file!\n");
        return;
    }

    char line[512]; 
    while (fgets(line, sizeof(line), file))
    {
        Book book;
        sscanf(line, "%d;%63[^;];%255[^;];%63[^;];%d;%d;%f;",
               &book.id,
               book.author,
               book.title,
               book.genre,
               &book.publication_year,
               &book.total_copies,
               &book.retail_price);

        book.available_copies = book.total_copies;
        add_book(head, book);
    }

    fclose(file); 
}

void load_books_info(Book_node** head)
{
    FILE *file = fopen(BOOKS_DATA_PATH, "rb"); 
    if (file == NULL)
    {
        fprintf(stderr, "Unable to open file!\n");
    }

    Book book;
    while (fread(&book, sizeof(Book), 1, file)) 
    { 
        add_book(head, book);
    }

    fclose(file);
}
void save_books_info(Book_node*  head)
{
    FILE *file = fopen(BOOKS_DATA_PATH, "wb"); 
    if (file == NULL) 
    {
        fprintf(stderr, "Unable to write file!\n");
        
        return;
    }
    while(head != NULL)
    {   
        fwrite( &(head->book_info), sizeof(Book), 1, file); 
        head = head->next;
    }
    fclose(file);     
}

int books_counter = 0; 
void add_book(Book_node** head, Book book)
{
    ++books_counter;
    Book_node* iterator = *head;

    if (iterator == NULL)
    {
        if (((*head) = (Book_node*)malloc(sizeof(Book_node))) == NULL)
        {
          fprintf(stderr, "Za mało pamięci!\n");
          exit(1);
        }
        (*head)->book_info = book;
    }
    else
    {
        Book_node* element;
        if ((element = (Book_node *)malloc(sizeof(Book_node))) == NULL) {
          fprintf(stderr, "Za mało pamięci!\n");
          exit(1);
       }
       element->book_info = book;

        while (iterator->next != NULL)
        {
            iterator = iterator->next;
        }

        iterator->next = element;
    }
}
void delete_book(Book_node** head, int id)
{
    Book_node* iterator = *head;
    Book_node* prev = *head;
    while(iterator->next != NULL && iterator->book_info.id != id)
    {
        prev = iterator;
        iterator->next;
    }

    if(iterator->book_info.id != id)
    {
        fprintf(stderr, "Nie znaleziono ksiazki o id %d!\n", id);
    }
    else
    {
        if(iterator->book_info.total_copies - iterator->book_info.available_copies != 0)
        {
            fprintf(stderr, "Czesc ksiazek o id: %d, dalej jest wyporzyczona, nie można usunac pozycji!\n", id);
        }
        else
        {
            prev->next = iterator->next;
            free(iterator);

            save_books_info(*head);
        }
    }
}

void hash_password(const char* plain_password, uint8_t* hashed_password)
{
    SHA256((unsigned char*)plain_password, strlen(plain_password), hashed_password);
}
bool secure_compare_hashes(const uint8_t hash1[32], const uint8_t hash2[32]) {
    return CRYPTO_memcmp(hash1, hash2, 32) == 0;
}

void load_users_info(User_node** head)
{
    FILE *file = fopen(USERS_DATA_PATH, "rb"); 
    if (file == NULL)
    {
        fprintf(stderr, "Unable to open file!\n");
    }

    User user;
    while (fread(&user, sizeof(User), 1, file)) 
    { 
        add_user(head, user);
    }

    fclose(file);
}
void save_users_info(User_node*  head)
{
    FILE *file = fopen(USERS_DATA_PATH, "wb"); 
    if (file == NULL) 
    {
        fprintf(stderr, "Unable to write file!\n");
        
        return;
    }
    while(head != NULL)
    {   
        fwrite( &(head->user_info), sizeof(Book), 1, file); 
        head = head->next;
    }
    fclose(file);     
}

int users_counter = 0; 
void add_user(User_node** head, User user)
{
    ++users_counter;
    User_node* iterator = *head;

    if (iterator == NULL)
    {
        if (((*head) = (User_node*)malloc(sizeof(User_node))) == NULL)
        {
          fprintf(stderr, "Za mało pamięci!\n");
          exit(1);
        }
        (*head)->user_info = user;
    }
    else
    {
        User_node* element;
        if ((element = (User_node *)malloc(sizeof(User_node))) == NULL) {
          fprintf(stderr, "Za mało pamięci!\n");
          exit(1);
       }
       element->user_info = user;

        while (iterator->next != NULL)
        {
            iterator = iterator->next;
        }

        iterator->next = element;
    }
}
void delete_user(User_node** head, int id)
{
    User_node* iterator = *head;
    User_node* prev = *head;
    while(iterator->next != NULL && iterator->user_info.id != id)
    {
        prev = iterator;
        iterator->next;
    }

    if(iterator->user_info.id != id)
    {
        fprintf(stderr, "Nie znaleziono ksiazki o id %d!\n", id);
    }
    else
    {
        if(iterator->user_info.borrowed_books_ids[0] == -1)
        {
            fprintf(stderr, "Nie mozna zamknac konta usera o id %d, poniewaz ma nie oddane ksiazki!\n", id);
        }
        else
        {
            prev->next = iterator->next;
            free(iterator);

            save_users_info(*head);
        }
    }
}