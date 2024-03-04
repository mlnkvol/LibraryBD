#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Entities.c"
#include "tables.h"

//--------------- checks ---------------
int checkIndexExists(FILE* indexTable, char* error, int id)
{
	fseek(indexTable, 0, SEEK_END);

	long indexTableSize = ftell(indexTable);

	if (indexTableSize == 0 || id * sizeof(struct Indexer) > indexTableSize)
	{
		strcpy(error, "ID doesn't exist");
		return 0;
	}

	return 1;
}
int checkEntityExists(struct Indexer indexer, char* error)
{
	if (!indexer.exists)
	{
		strcpy(error, "Entity has been deleted");
		return 0;
	}

	return 1;
}
//--------------- checks ---------------

//--------------- deleteBook ---------------
void relinkAddresses(FILE* database, struct Book previous, struct Book book, struct Author* author)
{
	if (book.selfAddress == author->firstBookAddress)
	{
		if (book.selfAddress == book.nextAddress)
		{
			author->firstBookAddress = -1;
		}
		else
		{
			author->firstBookAddress = book.nextAddress;
		}
	}
	else
	{
		if (book.selfAddress == book.nextAddress)
		{
			previous.nextAddress = previous.selfAddress;
		}
		else
		{
			previous.nextAddress = book.nextAddress;
		}

		fseek(database, previous.selfAddress, SEEK_SET);
		fwrite(&previous, sizeof(struct Book), 1, database);
	}
}
void markDeletedBook(int id)
{
	FILE* booksGarbage = fopen("deleted_books.txt", "rb");
	int garbageAmount;
	fscanf(booksGarbage, "%d", &garbageAmount);

	if (garbageAmount < 0)
	{
		garbageAmount = 0;
	}

	int* deletedIds = (int*)malloc(garbageAmount * sizeof(int));

	for (int i = 0; i < garbageAmount; i++)
	{
		fscanf(booksGarbage, "%d", deletedIds + i);
	}

	fclose(booksGarbage);
	booksGarbage = fopen("deleted_books.txt", "wb");
	fprintf(booksGarbage, "%d", garbageAmount + 1);

	for (int i = 0; i < garbageAmount; i++)
	{
		fprintf(booksGarbage, " %d", deletedIds[i]);
	}

	fprintf(booksGarbage, " %d", id);
	free(deletedIds);
	fclose(booksGarbage);
}
void deleteBook(struct Author author, struct Book book, int bookId, char* error)
{
	FILE* booksDatabase = fopen("book.fl", "r+b");
	struct Book previous;

	fseek(booksDatabase, author.firstBookAddress, SEEK_SET);

	do
	{
		fread(&previous, sizeof(struct Book), 1, booksDatabase);
		fseek(booksDatabase, previous.nextAddress, SEEK_SET);
	} while (previous.nextAddress != book.selfAddress && book.selfAddress != author.firstBookAddress);

	relinkAddresses(booksDatabase, previous, book, &author);
	markDeletedBook(book.bookId);

	book.exists = 0;

	fseek(booksDatabase, book.selfAddress, SEEK_SET);
	fwrite(&book, sizeof(struct Book), 1, booksDatabase);
	fclose(booksDatabase);

	author.booksCount--;
	updateAuthor(author, error);
}
//--------------- deleteBook ---------------

//--------------- deleteAuthor ---------------
void markDeletedAuthor(int id)
{
	FILE* authorsGarbage = fopen("deleted_authors.txt", "rb");
	int garbageAmount;

	fscanf(authorsGarbage, "%d", &garbageAmount);

	if (garbageAmount < 0)
	{
		garbageAmount = 0;
	}

	int* deleteIds = (int*)malloc(garbageAmount * sizeof(int));

	for (int i = 0; i < garbageAmount; i++)
	{
		fscanf(authorsGarbage, "%d", deleteIds + i);
	}

	fclose(authorsGarbage);
	authorsGarbage = fopen("deleted_authors.txt", "wb");
	fprintf(authorsGarbage, "%d", garbageAmount + 1);

	for (int i = 0; i < garbageAmount; i++)
	{
		fprintf(authorsGarbage, " %d", deleteIds[i]);
	}

	fprintf(authorsGarbage, " %d", id);
	free(deleteIds);
	fclose(authorsGarbage);
}
int deleteAuthor(int id, char* error)
{
	FILE* indexTable = fopen("author.ind", "r+b");
	struct Author author;
	struct Indexer indexer;

	if (!checkIndexExists(indexTable, error, id))
	{
		return 0;
	}

	fseek(indexTable, (id - 1) * sizeof(struct Indexer), SEEK_SET);
	fread(&indexer, sizeof(struct Indexer), 1, indexTable);

	if (indexer.exists == 0)
	{
		fclose(indexTable);
		return 0;
	}

	FILE* authorDatabase = fopen("author.fl", "r+b");
	fseek(authorDatabase, indexer.address, SEEK_SET);
	fread(&author, sizeof(struct Author), 1, authorDatabase);

	author.exists = 0;
	markDeletedAuthor(author.authorId);

	fseek(authorDatabase, indexer.address, SEEK_SET);
	fwrite(&author, sizeof(struct Author), 1, authorDatabase);

	fclose(authorDatabase);

	indexer.exists = 0;

	fseek(indexTable, (id - 1) * sizeof(struct Indexer), SEEK_SET);
	fwrite(&indexer, sizeof(struct Indexer), 1, indexTable);

	fclose(indexTable);

	FILE* bookDatabase = fopen("book.fl", "r+b");
	struct Book book;
	fseek(bookDatabase, author.firstBookAddress, SEEK_SET);

	for (int i = 0; i < author.booksCount; i++)
	{
		fread(&book, sizeof(struct Book), 1, bookDatabase);
		deleteBook(author, book, book.bookId, error);

		fseek(bookDatabase, book.nextAddress, SEEK_SET);
	}

	fclose(bookDatabase);

	return 1;
}
//--------------- deleteAuthor ---------------

//updateAuthor
int updateAuthor(struct Author author, char* error)
{
	FILE* indexTable = fopen("author.ind", "r+b");
	FILE* authorsDatabase = fopen("author.fl", "r+b");
	struct Indexer indexer;
	int id = author.authorId;

	if (!checkIndexExists(indexTable, error, id))
	{
		return 0;
	}

	fseek(indexTable, (id - 1) * sizeof(struct Indexer), SEEK_SET);
	fread(&indexer, sizeof(struct Indexer), 1, indexTable);

	if (!checkEntityExists(indexer, error))
	{
		return 0;
	}

	fseek(authorsDatabase, indexer.address, SEEK_SET);
	fwrite(&author, sizeof(struct Author), 1, authorsDatabase);

	fclose(indexTable);
	fclose(authorsDatabase);

	return 1;
}

//--------------- insertBook ---------------
int findMinAvailableBookIndex()
{
	FILE* booksGarbage = fopen("deleted_books.txt", "r+");
	int garbageAmount;

	fscanf(booksGarbage, "%d", &garbageAmount);

	int* deletedIds = (int*)malloc(garbageAmount * sizeof(int));
	for (int i = 0; i < garbageAmount; i++)
	{
		fscanf(booksGarbage, "%d", &deletedIds[i]);
	}

	int minId = deletedIds[0];
	for (int i = 1; i < garbageAmount; i++)
	{
		if (deletedIds[i] < minId)
		{
			minId = deletedIds[i];
		}
	}

	fclose(booksGarbage);
	booksGarbage = fopen("deleted_books.txt", "w");

	rewind(booksGarbage);
	fprintf(booksGarbage, "%d", garbageAmount - 1);
	for (int i = 0; i < garbageAmount; i++)
	{
		if (deletedIds[i] != minId)
		{
			fprintf(booksGarbage, " %d", deletedIds[i]);
		}
	}

	free(deletedIds);
	fclose(booksGarbage);

	return minId;
}
int insertBook(struct Author author, struct Book book, char* error)
{
	FILE* booksDatabase = fopen("book.fl", "a+b");
	FILE* booksGarbage = fopen("deleted_books.txt", "a+b");
	int garbageAmount = 0;

	fscanf(booksGarbage, "%d", &garbageAmount);
	fclose(booksGarbage);

	if (garbageAmount > 0)
	{
		fclose(booksDatabase);
		booksDatabase = fopen("book.fl", "r+b");
		int minAvailableId = findMinAvailableBookIndex();
		book.bookId = minAvailableId;
		book.selfAddress = (minAvailableId - 1) * sizeof(struct Book);
		book.nextAddress = book.selfAddress;
		book.authorId = author.authorId;
		book.exists = 1;
		fseek(booksDatabase, book.selfAddress, SEEK_SET);
		fwrite(&book, sizeof(struct Book), 1, booksDatabase);
		fclose(booksDatabase);
	}
	else
	{
		fclose(booksDatabase);
		booksDatabase = fopen("book.fl", "r+b");
		book.exists = 1;
		fseek(booksDatabase, 0, SEEK_END);
		int dbSize = ftell(booksDatabase);
		book.selfAddress = dbSize;
		book.nextAddress = dbSize;
		book.bookId = (dbSize / sizeof(struct Book)) + 1;
		fwrite(&book, sizeof(struct Book), 1, booksDatabase);
		fclose(booksDatabase);
	}

	booksDatabase = fopen("book.fl", "r+b");
	author.booksCount++;
	int min = -1;

	if (author.booksCount == 1)
	{
		author.firstBookAddress = book.selfAddress;
		updateAuthor(author, error);
		printf("Book's id: %d\n", book.bookId);
		return 1;
	}
	else
	{
		fseek(booksDatabase, 0, SEEK_END);
		int dbSize = ftell(booksDatabase);

		fseek(booksDatabase, 0, SEEK_SET);
		struct Book current_book1;
		int min = -1;
		int n = dbSize / sizeof(struct Book);

		for (int i = 0; i < n; i++)
		{
			fread(&current_book1, sizeof(struct Book), 1, booksDatabase);

			if (current_book1.exists && (min == -1 || current_book1.bookId < min) && current_book1.authorId == author.authorId)
			{
				min = current_book1.bookId;
				author.firstBookAddress = current_book1.selfAddress;
			}
		}

		updateAuthor(author, error);

		fseek(booksDatabase, author.firstBookAddress, SEEK_SET);
		struct Book current_book, next_book;

		fread(&current_book, sizeof(struct Book), 1, booksDatabase);

		for (int i = 0; i < n; i++)
		{
			fread(&next_book, sizeof(struct Book), 1, booksDatabase);

			if (next_book.exists && current_book.authorId == next_book.authorId)
			{
				current_book.nextAddress = next_book.selfAddress;
				fseek(booksDatabase, current_book.selfAddress, SEEK_SET);
				fwrite(&current_book, sizeof(struct Book), 1, booksDatabase);
				fseek(booksDatabase, next_book.selfAddress, SEEK_SET);
				fread(&current_book, sizeof(struct Book), 1, booksDatabase);
			}
		}

		printf("Book's id: %d\n", book.bookId);
	}

	fclose(booksDatabase);
	return 1;
}
//--------------- insertBook ---------------

//--------------- calculate ---------------
void outputAuthor(struct Author author)
{
	printf("Full Name: %s\n", author.fullName);
	printf("Birth Year: %d\n", author.birthYear);
	printf("Country: %s\n", author.country);
	printf("Books count: %d\n\n", author.booksCount);
}
void iterateOverAuthors(int ids, struct Author* author, char* error, int* authorsCount, int* booksCount)
{
	for (int i = 1; i <= ids; i++)
	{
		if (getAuthor(author, i, error))
		{
			(*authorsCount)++;
			(*booksCount) += (*author).booksCount;
		}
	}
}
void iterateOverAuthorsPrint(int ids, struct Author* author, char* error, int* authorsCount, int* booksCount)
{
	for (int i = 1; i <= ids; i++)
	{
		if (getAuthor(author, i, error))
		{
			(*authorsCount)++;
			(*booksCount) += (*author).booksCount;
			printf("Author with id %d: %d books\n", i, (*author).booksCount);
		}
	}
}
void calculate(int n)
{
	FILE* indexTable = fopen("author.ind", "rb");
	int authorsCount = 0;
	int booksCount = 0;
	struct Author author;
	char error[100];

	fseek(indexTable, 0, SEEK_END);
	int indAmount = ftell(indexTable) / sizeof(struct Indexer);

	iterateOverAuthors(indAmount, &author, error, &authorsCount, &booksCount);

	fclose(indexTable);

	if (n == 9) printf("All authors: %d\n", authorsCount);
	if (n == 10) printf("All books: %d\n", booksCount);
	if (n == 11) iterateOverAuthorsPrint(indAmount, &author, error, &authorsCount, &booksCount);
}
//--------------- calculate ---------------
