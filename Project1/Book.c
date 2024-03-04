#pragma once
#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Entities.c"
#include "Common.c"

//getBook
int getBook(struct Author author, struct Book* book, int bookId, char* error)
{
	if (!author.booksCount)
	{
		strcpy(error, "Author doesn't have any books");
		return 0;
	}

	FILE* booksDatabase = fopen("book.fl", "rb");
	fseek(booksDatabase, author.firstBookAddress, SEEK_SET);
	fread(book, sizeof(struct Book), 1, booksDatabase);

	for (int i = 0; i < author.booksCount; i++)
	{
		if (book->bookId == bookId)
		{
			fclose(booksDatabase);
			return 1;
		}

		fseek(booksDatabase, book->nextAddress, SEEK_SET);
		fread(book, sizeof(struct Book), 1, booksDatabase);
	}

	strcpy(error, "Book doesn't exist");
	fclose(booksDatabase);

	return 0;
}

//outputBook
void outputBook(struct Book book, struct Author author)
{
	printf("Author's full name: %s, birth year: %d, country: %s\n", author.fullName, author.birthYear, author.country);
	printf("Title: %s\n", book.title);
	printf("Category: %s\n", book.category);
	printf("Genre: %s\n", book.genre);
}

//inputBook
void inputBook(struct Book* book)
{
	char title[100];
	char category[50];
	char genre[30];

	title[0] = '\0';
	printf("Enter book's title: ");
	scanf("%s", title);
	strcpy(book->title, title);

	category[0] = '\0';
	printf("Enter book's category: ");
	scanf("%s", category);
	strcpy(book->category, category);

	genre[0] = '\0';
	printf("Enter book's genre: ");
	scanf("%s", genre);
	strcpy(book->genre, genre);
}


//updateBook
int updateBook(struct Book book, int bookId)
{
	FILE* booksDatabase = fopen("book.fl", "r+b");
	book.exists = 1;
	fseek(booksDatabase, book.selfAddress, SEEK_SET);
	fwrite(&book, sizeof(struct Book), 1, booksDatabase);
	fclose(booksDatabase);

	return 1;
}

//ut_s
void ut_s()
{
	FILE* booksDatabase = fopen("book.fl", "rb");
	fseek(booksDatabase, 0, SEEK_SET);

	struct Book book;

	printf("+-----+------------+----------------------+----------------------+----------------------+----------------------+----------------------+\n");
	printf("| Id  | Author's id| Title                | Category             | Genre                | Self Address         | Next Address         |\n");
	printf("+-----+------------+----------------------+----------------------+----------------------+----------------------+----------------------+\n");

	while (fread(&book, sizeof(struct Book), 1, booksDatabase) == 1)
	{
		if (book.exists != 0)
		{
			printf("| %-3d | %-10d | %-20s | %-20s | %-20s | %-20ld | %-20ld |\n", book.bookId, book.authorId, book.title, book.category, book.genre, book.selfAddress, book.nextAddress);
		}
	}

	printf("+-----+------------+----------------------+----------------------+----------------------+----------------------+----------------------+\n");

	fclose(booksDatabase);
}
