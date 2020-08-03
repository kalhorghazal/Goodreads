#include "Book.h"
#include "Review.h"

#include <map>
#include <omp.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <pthread.h>

#define ONE 1
#define ZERO 0
#define GENRE 1
#define TRUE true
#define FALSE false
#define NEW_LINE '\n'
#define BOOKS_FILE "books.csv"
#define REVIEWS_FILE "reviews.csv"
#define NUMBER_OF_BOOK_THREADS 4
#define NUMBER_OF_REVIEW_THREADS 6

using namespace std;

typedef int Book_id;
typedef string Genre;
typedef vector<Review*> Reviews;
typedef map<Book_id, Book*> Books;

enum Book_feature
{
	ID,
	TITLE,
	GENRE_1,
	GENRE_2, 
	PAGES,
	AUTHOR_NAME,
	AUTHOR_AVERAGE_RATING
};

enum Review_feature
{
	BOOK_ID,
	RATING,
	NUMBER_OF_LIKES
};

typedef struct 
{
	int start; 
	int length; 
} Read_data;

Books books;
Genre genre;
Reviews reviews;
pthread_mutex_t mutex_read_book;
pthread_mutex_t mutex_read_review;
pthread_t book_threads[NUMBER_OF_BOOK_THREADS];
pthread_t review_threads[NUMBER_OF_REVIEW_THREADS];

void call_book_readers();
void call_review_readers();
bool contains_key(int key);
void* read_books(void* arg);
void* read_reviews(void* arg);
void find_best_book(Books& books);
int get_file_length(string filename);
void count_ratings(Reviews reviews, Books& books);
void extract_new_book_info(Books& books, string line);
void extract_new_review_info(Reviews& reviews, string line);