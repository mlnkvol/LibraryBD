#ifndef TABLES_H
#define TABLES_H

#include "Entities.c"

void printAuthorsTable(struct Author* authors, int authorsCount);
void printBooksTable(struct Book* books, int booksCount);
void printAuthorsAndBooksTable(struct Author* authors, int authorsCount, struct Book* books, int booksCount);

#endif // TABLES_H