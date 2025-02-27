#include "Goodreads.h"

int main(int argc, char const *argv[])
{
	genre = argv[GENRE];

	read_csv(books, BOOKS_FILE);
	read_csv(reviews, REVIEWS_FILE);

	count_ratings(reviews, books);
	find_best_book(books);

	return ZERO;
}

void extract_new_book_info(Books& books, string line)
{
	istringstream templine(line);
	string data;
	vector<string> info;

	while (getline(templine, data, COMMA)){
		info.push_back(data);
	}

	Book *book = new Book(atoi(info[ID].c_str()), info[TITLE], info[GENRE_1], 
		info[GENRE_2], atoi(info[PAGES].c_str()), info[AUTHOR_NAME], 
		atof(info[AUTHOR_AVERAGE_RATING].c_str()));

	if (book->has_genre(genre))
		books[atoi(info[ID].c_str())] = book;
}

void extract_new_review_info(Reviews& reviews, string line)
{
	istringstream templine(line);
	string data;
	vector<string> info;

	while (getline(templine, data, COMMA)){
		info.push_back(data);
	}

	Review* review = new Review(atoi(info[BOOK_ID].c_str()), 
		atoi(info[RATING].c_str()), atoi(info[NUMBER_OF_LIKES].c_str()));

	if (contains_key(atoi(info[BOOK_ID].c_str())))
		reviews.push_back(review);
}

void read_csv(Books& books, string filename)
{
	ifstream file;
	file.open(filename);
	string line;
	getline(file, line, NEW_LINE);

	while (getline(file, line, NEW_LINE))
		extract_new_book_info(books, line);

	file.close();
}

void read_csv(Reviews& reviews, string filename)
{
	ifstream file;
	file.open(filename);
	string line;
	getline(file, line, NEW_LINE);

	while (getline(file, line, NEW_LINE))
		extract_new_review_info(reviews, line);

	file.close();
}

void count_ratings(Reviews reviews, Books& books)
{
    #pragma omp parallel for 
	for (int i = ZERO; i < reviews.size(); ++i)
	{
		Book_id book_id = reviews[i]->get_book_id();

		books[book_id]->update_rating(reviews[i]->get_rating(), 
			reviews[i]->get_number_of_likes());
	}
}

void find_best_book(Books& books)
{
	Book* best;
	double max_rating = ZERO;
	
	#pragma omp parallel for 
	for (Books::iterator it = books.begin(); 
		it != books.end(); ++it)
	{
		Book* book = it->second;		
    	book->calulate_average_rating();

    	if (book->get_average_rating() > max_rating)
    	{
    		best = book;
    	 	max_rating = book->get_average_rating();
    	}
	}

	cout << *best;
}

bool contains_key(int key)
{
	if (books.find(key) != books.end())
		return TRUE;

	return FALSE;
}