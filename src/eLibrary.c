#include"eLibrary.h"
#include "utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/sha.h>
#include <openssl/crypto.h>


User current_user = {0};
int import_books_data(Book_node** head, char* path)
{
    int imported_books = 0;

    FILE *file = fopen(path, "r"); 
    if (file == NULL) 
    {
        fprintf(stderr, "Unable to open file!\n");
        return imported_books;
    }

    char line[512]; 
    while (fgets(line, sizeof(line), file))
    {
        Book book;
        char tocken1[64], tocken2[64], tocken3[256];
        char tocken4[64], tocken5[64], tocken6[64], tocken7[64];

        split(line, tocken1, tocken2, tocken3, tocken4, tocken5, tocken6, tocken7, ';');

        book.id = books_counter;
        strcpy(book.author, tocken2);
        strcpy(book.title, tocken3);
        strcpy(book.genre, tocken4);
        book.publication_year = atoi(tocken5);
        book.total_copies = atoi(tocken6);
        book.retail_price = atof(tocken7);
        
        book.available_copies = book.total_copies;
        ++imported_books;
        add_book(head, book);
    }

    fclose(file); 
    return imported_books;
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
    return;
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
       element->next = NULL;

        while (iterator->next != NULL)
        {
            iterator = iterator->next;
        }

        iterator->next = element;
    }
}
void delete_book(Book_node** head, int id)
{
    if((*head) == NULL)
    {
        fprintf(stderr, "Lista jest pusta, nie mozna usunac książki!\n");
        return;
    }

    Book_node* iterator = *head;
    Book_node* prev = *head;
    while(iterator->next != NULL && iterator->book_info.id != id)
    {
        prev = iterator;
        iterator = iterator->next;
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
            if(prev == iterator)
            {
                if(iterator->next == NULL)
                {
                    free(iterator);
                    (*head) = NULL;
                }
                else
                {
                    *head = iterator->next;
                    free(iterator);
                }

            }
            else
            {
                prev->next = iterator->next;
                free(iterator);
            }
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
        fwrite( &(head->user_info), sizeof(User), 1, file); 
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
        element->next = NULL;

        while (iterator->next != NULL)
        {
            iterator = iterator->next;
        }

        iterator->next = element;
    }
}
void delete_user(User_node** head, int id)
{
    if (*head == NULL)
    {
        fprintf(stderr, "Lista jest pusta, nie mozna usunac uzytkownika!\n");
        return;
    }

    User_node* iterator = *head;
    User_node* prev = *head;
    while(iterator->next != NULL && iterator->user_info.id != id)
    {
        prev = iterator;
        iterator = iterator->next;
    }
    if(iterator->user_info.id != id)
    {
        fprintf(stderr, "Nie znaleziono użytkownika o id %d!\n", id);
    }
    else
    {
        if(iterator->user_info.borrowed_books_ids[0] == -1 && iterator->user_info.borrowed_books_ids[1] == -1 && iterator->user_info.borrowed_books_ids[2] == -1) 
        {
            //obsługa usuwania użytkowników
            if(prev == iterator)
            {
                if(iterator->next == NULL)
                {
                    free(iterator);
                    (*head) = NULL;
                }
                else
                {
                    (*head) = iterator->next;
                    free(iterator);
                }
            }
            else
            {
                prev->next = iterator->next;
                free(iterator);

            }
            save_users_info(*head);

        }
        else
        {
            fprintf(stderr, "Nie mozna zamknac konta usera o id %d, poniewaz ma nie oddane ksiazki!\n", id);
        }
    }
}

void swap_books(Book* a, Book* b) 
{
    Book temp = *a;
    *a = *b;
    *b = temp;
}

void sort_books(Book_node* head, SortType sort_type, bool asc)
{
    if (!head) return;

    int swapped;
    Book_node* ptr;
    Book_node* lptr = NULL;

    do {
        swapped = 0;
        ptr = head;

        while (ptr->next != lptr) {
            int compare = 0;
            switch (sort_type) {
                case SORT_BY_TITLE:
                    compare = strcmp(ptr->book_info.title, ptr->next->book_info.title);
                    break;
                case SORT_BY_AUTHOR:
                    compare = strcmp(ptr->book_info.author, ptr->next->book_info.author);
                    break;
                case SORT_BY_GENRE:
                    compare = strcmp(ptr->book_info.genre, ptr->next->book_info.genre);
                    break;
                case SORT_BY_PRICE:
                    compare = (ptr->book_info.retail_price > ptr->next->book_info.retail_price) ? 1 : -1;
                    break;
                case SORT_BY_YEAR:
                    compare = (ptr->book_info.publication_year > ptr->next->book_info.publication_year) ? 1 : -1;
                    break;
            }

            if (asc ? (compare > 0) : (compare < 0)) {
                swap_books(&ptr->book_info, &ptr->next->book_info);
                swapped = 1;
            }

            ptr = ptr->next;
        }
        lptr = ptr;
    } while (swapped);
}


//_____________________ DEBUG _______________________

void print_books(Book_node* head) {
    Book_node* current = head;
    printf("Lista książek:\n");
    while (current != NULL) {
        Book book = current->book_info;
        printf("ID: %d\n", book.id);
        printf("Autor: %s\n", book.author);
        printf("Tytuł: %s\n", book.title);
        printf("Gatunek: %s\n", book.genre);
        printf("Rok publikacji: %d\n", book.publication_year);
        printf("Liczba kopii: %d\n", book.total_copies);
        printf("Dostępne kopie: %d\n", book.available_copies);
        printf("Cena detaliczna: %.2f\n", book.retail_price);
        printf("-------------------------\n");
        current = current->next;
    }
}

void print_users(User_node* head) {
    User_node* current = head;
    printf("Lista użytkowników:\n");
    while (current != NULL) {
        User user = current->user_info;
        printf("ID: %d\n", user.id);
        printf("Imię: %s\n", user.first_name);
        printf("Nazwisko: %s\n", user.last_name);
        printf("Email: %s\n", user.email);
        printf("Administrator: %s\n", user.is_admin ? "Tak" : "Nie");
        printf("Wypożyczone książki (ID):");
        for (int i = 0; i < sizeof(user.borrowed_books_ids) / sizeof(user.borrowed_books_ids[0]); i++) {
            if (user.borrowed_books_ids[i] != -1) 
            {
                printf(" %d", user.borrowed_books_ids[i]);
            }
            else
            {
                printf(" -");
            }
        }
        printf("\n-------------------------\n");
        current = current->next;
    }
}
