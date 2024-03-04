#include <stdio.h>
#include "tables.h"

// Функція для виведення таблиці авторів
void printAuthorsTable(struct Author* authors, int authorsCount) {
	printf("+-----+-------------------------+------------+-------------+\n");
	printf("| ID  | Full Name               | Birth Year | Country     |\n");
	printf("+-----+-------------------------+------------+-------------+\n");

	for (int i = 0; i < authorsCount; i++) {
		printf("| %-3d | %-23s | %-10d | %-11s |\n", authors[i].authorId, authors[i].fullName, authors[i].birthYear, authors[i].country);
	}

	printf("+-----+-------------------------+------------+-------------+\n");
}

// Функція для виведення таблиці книг
void printBooksTable(struct Book* books, int booksCount) {
	printf("+-----+-------------------------+-------------------------+-------------------------+\n");
	printf("| ID  | Author's ID             | Title                   | Category                |\n");
	printf("+-----+-------------------------+-------------------------+-------------------------+\n");

	for (int i = 0; i < booksCount; i++) {
		printf("| %-3d | %-23d | %-23s | %-23s |\n", books[i].bookId, books[i].authorId, books[i].title, books[i].category);
	}

	printf("+-----+-------------------------+-------------------------+-------------------------+\n");
}

// Функція для виведення таблиці авторів та книг
void printAuthorsAndBooksTable(struct Author* authors, int authorsCount, struct Book* books, int booksCount) {
	printf("+-----+-------------------------+------------+-------------+-------------------------+-------------------------+-------------------------+\n");
	printf("| ID  | Full Name               | Birth Year | Country     | Book ID                 | Title                   | Category                |\n");
	printf("+-----+-------------------------+------------+-------------+-------------------------+-------------------------+-------------------------+\n");

	int i, j;
	for (i = 0, j = 0; i < authorsCount && j < booksCount; i++, j++) {
		printf("| %-3d | %-23s | %-10d | %-11s | %-23d | %-23s | %-23s |\n",
			authors[i].authorId, authors[i].fullName, authors[i].birthYear, authors[i].country,
			books[j].bookId, books[j].title, books[j].category);
	}

	while (i < authorsCount) {
		printf("| %-3d | %-23s | %-10d | %-11s | %-23s | %-23s | %-23s |\n",
			authors[i].authorId, authors[i].fullName, authors[i].birthYear, authors[i].country, "", "", "");
		i++;
	}

	while (j < booksCount) {
		printf("| %-3s | %-23s | %-10s | %-11s | %-23d | %-23s | %-23s |\n",
			"", "", "", "", books[j].bookId, books[j].title, books[j].category);
		j++;
	}

	printf("+-----+-------------------------+------------+-------------+-------------------------+-------------------------+-------------------------+\n");
}