#pragma once

struct Author
{
	int authorId;
	char fullName[50];
	int birthYear;
	char country[30];
	int exists;
	long firstBookAddress;
	int booksCount;
};

struct Book
{
	int bookId;
	int authorId;
	char title[100];
	char category[50];
	char genre[30];
	int exists;
	long selfAddress;
	long nextAddress;
};

struct Indexer
{
	int authorId;
	int address;
	int exists;
};