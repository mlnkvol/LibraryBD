#pragma once
#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Entities.c"
#include "Common.c"

//getAuthor
int getAuthor(struct Author* author, int id, char* error)
{
	FILE* indexTable = fopen("author.ind", "rb");
	FILE* authorsDatabase = fopen("author.fl", "rb");
	struct Indexer indexer;

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
	fread(author, sizeof(struct Author), 1, authorsDatabase);

	fclose(indexTable);
	fclose(authorsDatabase);

	return 1;
}

//--------------- insertAuthor ---------------
int findMinAvailableAuthorIndex()
{
	FILE* authorsGarbage = fopen("deleted_authors.txt", "r+");
	int garbageAmount;

	fscanf(authorsGarbage, "%d", &garbageAmount);

	int* deletedIds = (int*)malloc(garbageAmount * sizeof(int));
	for (int i = 0; i < garbageAmount; i++)
	{
		fscanf(authorsGarbage, "%d", &deletedIds[i]);
	}

	int minId = deletedIds[0];
	for (int i = 1; i < garbageAmount; i++)
	{
		if (deletedIds[i] < minId)
		{
			minId = deletedIds[i];
		}
	}

	fclose(authorsGarbage);
	authorsGarbage = fopen("deleted_authors.txt", "w");

	rewind(authorsGarbage);
	fprintf(authorsGarbage, "%d", garbageAmount - 1);
	for (int i = 0; i < garbageAmount; i++)
	{
		if (deletedIds[i] != minId)
		{
			fprintf(authorsGarbage, " %d", deletedIds[i]);
		}
	}

	free(deletedIds);
	fclose(authorsGarbage);

	return minId;
}
void inputAuthor(struct Author* author)
{
	char fullName[50];
	int birthYear;
	char country[30];

	fullName[0] = '\0';
	printf("Enter author's fullname: ");
	scanf("%s", fullName);
	strcpy(author->fullName, fullName);

	do {
		printf("Enter author's birth year: ");
		scanf("%d", &birthYear);

		if (birthYear > 2006 || birthYear < 1924) {
			printf("Enter the correct year of birth\n");
		}

	} while (birthYear > 2006 || birthYear < 1924);

	author->birthYear = birthYear;

	country[0] = '\0';
	printf("Enter author's country: ");
	scanf("%s", country);
	strcpy(author->country, country);
}
int insertAuthor(struct Author author)
{
	FILE* indexTable = fopen("author.ind", "a+b");
	FILE* authorsDatabase = fopen("author.fl", "a+b");
	FILE* authorsGarbage = fopen("deleted_authors.txt", "a+b");
	struct Indexer indexer;
	int garbageAmount = 0;

	fscanf(authorsGarbage, "%d", &garbageAmount);
	fclose(authorsGarbage);

	if (garbageAmount > 0)
	{
		int minAvailableIndex = findMinAvailableAuthorIndex();
		author.authorId = minAvailableIndex;
		fclose(indexTable);
		fclose(authorsDatabase);
		indexTable = fopen("author.ind", "r+b");
		authorsDatabase = fopen("author.fl", "r+b");
		fseek(indexTable, (author.authorId - 1) * sizeof(struct Indexer), SEEK_SET);
		fread(&indexer, sizeof(struct Indexer), 1, indexTable);
		fseek(authorsDatabase, indexer.address, SEEK_SET);
	}
	else
	{
		fseek(indexTable, 0, SEEK_END);

		if (ftell(indexTable))
		{
			fseek(indexTable, -((long)sizeof(struct Indexer)), SEEK_END);
			fread(&indexer, sizeof(struct Indexer), 1, indexTable);
			author.authorId = indexer.authorId + 1;
		}
		else
		{
			author.authorId = 1;
		}
	}

	author.firstBookAddress = -1;
	author.booksCount = 0;
	fwrite(&author, sizeof(struct Author), 1, authorsDatabase);

	indexer.authorId = author.authorId;
	indexer.address = (author.authorId - 1) * sizeof(struct Author);
	indexer.exists = 1;
	printf("Author's id: %d\n", author.authorId);

	fseek(indexTable, (author.authorId - 1) * sizeof(struct Indexer), SEEK_SET);
	fwrite(&indexer, sizeof(struct Indexer), 1, indexTable);

	fclose(indexTable);
	fclose(authorsDatabase);

	return 1;
}
//--------------- insertAuthor ---------------

//ut_m
void ut_m()
{
	FILE* authorsDatabase = fopen("author.fl", "rb");
	fseek(authorsDatabase, 0, SEEK_SET);

	struct Author author;

	printf("+-----+----------------------+------------+--------+----------------------+-------------+\n");
	printf("| Id  | Full Name            | Birth Year | Country| First book address   | Books count |\n");
	printf("+-----+----------------------+------------+--------+----------------------+-------------+\n");

	while (fread(&author, sizeof(struct Author), 1, authorsDatabase) == 1)
	{
		if (author.exists != 0)
		{
			printf("| %-3d | %-20s | %-10d | %-6s | %-20ld | %-11d |\n", author.authorId, author.fullName, author.birthYear, author.country, author.firstBookAddress, author.booksCount);
		}
	}

	printf("+-----+----------------------+------------+--------+----------------------+-------------+\n");

	fclose(authorsDatabase);
}
