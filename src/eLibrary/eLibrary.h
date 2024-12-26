#ifndef ELIBRARY
#define ELIBRARY

#define PAGE_SIZE 1024

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

typedef struct book_node
{
    Book book_info;
    Book_node* next;    
}Book_node;


typedef struct user
{
    int id;

    char first_name[64];        
    char last_name[64];         
    char email[128];            
    int borrowed_books_ids[3];  
} User;

typedef struct user_node
{
    User user_info;
    User_node* next;    
}User_node;





#endif 