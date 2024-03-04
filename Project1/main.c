#pragma once
#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables.h"
#include "Entities.c"
#include "Common.c"
#include "Author.c"
#include "Book.c"

int main()
{
    struct Author author;
    struct Book book;

    while (1)
    {
        int choice;
        int id;
        char error[100];

        printf("Available database operations:\n1. get-m\n2. get-s\n3. del-m\n4. del-s\n5. update-m\n6. update-s\n7. insert-m\n8. insert-s\n9. calc-m\n10. calc-s\n11. calc-m all\n12. ut-m\n13. ut-s\n14. exit\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter author's ID: ");
            scanf("%d", &id);
            getAuthor((struct Author*)&author, id, error) ? outputAuthor(author) : printf("Error: %s\n", error);
            break;

        case 2:
            printf("Enter author's ID: ");
            scanf("%d", &id);
            if (getAuthor(&author, id, error))
            {
                printf("Enter book's ID: ");
                scanf("%d", &id);
                getBook(author, &book, id, error) ? outputBook(book, author) : printf("Error: %s\n", error);
            }
            else
            {
                printf("Error: %s\n", error);
            }
            break;

        case 3:
            printf("Enter author's ID: ");
            scanf("%d", &id);
            deleteAuthor(id, error) ? printf("Author was successfully deleted\n") : printf("Error: %s\n", error);
            break;

        case 4:
            printf("Enter author's ID: ");
            scanf("%d", &id);
            if (getAuthor(&author, id, error))
            {
                printf("Enter book's ID: ");
                scanf("%d", &id);
                if (getBook(author, &book, id, error))
                {
                    deleteBook(author, book, id, error);
                    printf("Book was successfully deleted\n");
                }
                else
                {
                    printf("Error: %s\n", error);
                }
            }
            else
            {
                printf("Error: %s\n", error);
            }
            break;

        case 5:
            printf("Enter author's ID: ");
            scanf("%d", &id);
            author.authorId = id;
            getAuthor(&author, id, error);
            inputAuthor(&author);
            updateAuthor(author, error) ? printf("Author was successfully updated\n") : printf("Error: %s\n", error);
            break;

        case 6:
            printf("Enter author's ID: ");
            scanf("%d", &id);
            if (getAuthor(&author, id, error))
            {
                printf("Enter book's ID: ");
                scanf("%d", &id);
                if (getBook(author, &book, id, error))
                {
                    inputBook(&book);
                    updateBook(book, id);
                    printf("Book was successfully updated\n");
                }
                else
                {
                    printf("Error: %s\n", error);
                }
            }
            else
            {
                printf("Error: %s\n", error);
            }
            break;

        case 7:
            author.booksCount = 0;
            author.exists = 1;
            inputAuthor(&author);
            insertAuthor(author);
            break;

        case 8:
            printf("Enter author's ID: ");
            scanf("%d", &id);
            if (getAuthor(&author, id, error))
            {
                book.exists = 1;
                book.authorId = author.authorId;
                inputBook(&book);
                insertBook(author, book, error);
            }
            else
            {
                printf("Error: %s\n", error);
            }
            break;

        case 9:
            calculate(9);
            break;

        case 10:
            calculate(10);
            break;

        case 11:
            calculate(11);
            break;

        case 12:
            ut_m();
            break;

        case 13:
            ut_s();
            break;

        case 14:
            return 0;

        default:
            printf("Input isn't valid; please select an option between 1 and 14\n");
        }
        printf("-----------------------------------------------------------------------\n");
    }

    return 0;
}


//int main() {
//    while (1) {
//        printf("Available database operations:\n1. get-m\n2. get-s\n3. del-m\n4. del-s\n5. update-m\n6. update-s\n7. insert-m\n8. insert-s\n9. calc-m\n10. calc-s\n11. calc-m all\n12. ut-m\n13. ut-s\n14. exit\n");
//        printf("Enter command: ");
//
//        char input[100]; // Задайте достатньо розмірний буфер для вводу
//        fgets(input, sizeof(input), stdin);
//
//        char command[20];
//        int id;
//
//        // Використовуємо sscanf для отримання команди та id з рядка вводу
//        if (sscanf(input, "%s %d", command, &id) == 2) {
//            // Реалізація обробки команди та id
//            struct Author author;
//            struct Book book;
//            char error[100];
//
//            if (strcmp(command, "get-m") == 0) {
//                // Ваша реалізація для команди get-m
//            }
//            else if (strcmp(command, "get-s") == 0) {
//                // Ваша реалізація для команди get-s
//            }
//            else if (strcmp(command, "del-m") == 0) {
//                // Ваша реалізація для команди del-m
//            }
//            else if (strcmp(command, "del-s") == 0) {
//                // Ваша реалізація для команди del-s
//            }
//            else if (strcmp(command, "update-m") == 0) {
//                // Ваша реалізація для команди update-m
//            }
//            else if (strcmp(command, "update-s") == 0) {
//                // Ваша реалізація для команди update-s
//            }
//            else if (strcmp(command, "insert-m") == 0) {
//                // Ваша реалізація для команди insert-m
//            }
//            else if (strcmp(command, "insert-s") == 0) {
//                // Ваша реалізація для команди insert-s
//            }
//            else if (strcmp(command, "calc-m") == 0) {
//                // Ваша реалізація для команди calc-m
//            }
//            else if (strcmp(command, "calc-s") == 0) {
//                // Ваша реалізація для команди calc-s
//            }
//            else if (strcmp(command, "calc-m all") == 0) {
//                // Ваша реалізація для команди calc-m all
//            }
//            else if (strcmp(command, "ut-m") == 0) {
//                // Ваша реалізація для команди ut-m
//            }
//            else if (strcmp(command, "ut-s") == 0) {
//                // Ваша реалізація для команди ut-s
//            }
//            else if (strcmp(command, "exit") == 0) {
//                return 0;
//            }
//            else {
//                printf("Invalid command\n");
//            }
//        }
//        else {
//            printf("Invalid command format\n");
//        }
//
//        printf("-----------------------------------------------------------------------\n");
//    }
//
//    return 0;
//}

//void processCommand(const char* command);
//void parseInsertM(const char* command);
//void parseInsertS(const char* command);
//void parseGetM(const char* command);
//void parseGetS(const char* command);
//void parseDelM(const char* command);
//void parseDelS(const char* command);
//void parseUpdateM(const char* command);
//void parseUpdateS(const char* command);
//void parseCalcM(const char* command);
//void parseCalcS(const char* command);
//void parseCalcMAll(const char* command);
//void parseUtM(const char* command);
//void parseUtS(const char* command);
//
//int main()
//{
//    char command[100];
//
//    while (1)
//    {
//        printf("Enter command: ");
//        fgets(command, sizeof(command), stdin);
//
//        // Видалення символу нового рядка, який додається функцією fgets
//        command[strcspn(command, "\n")] = 0;
//
//        processCommand(command);
//        printf("-----------------------------------------------------------------------\n");
//    }
//
//    return 0;
//}
//
//void processCommand(const char* command)
//{
//    // Отримуємо перший токен (команду)
//    char* token = strtok((char*)command, " ");
//
//    if (token == NULL)
//    {
//        printf("Invalid command\n");
//        return;
//    }
//
//    // Обробляємо команди
//    if (strcmp(token, "insert-m") == 0)
//    {
//        parseInsertM(command);
//    }
//    else if (strcmp(token, "insert-s") == 0)
//    {
//        parseInsertS(command);
//    }
//    else if (strcmp(token, "get-m") == 0)
//    {
//        parseGetM(command);
//    }
//    else if (strcmp(token, "get-s") == 0)
//    {
//        parseGetS(command);
//    }
//    else if (strcmp(token, "del-m") == 0)
//    {
//        parseDelM(command);
//    }
//    else if (strcmp(token, "del-s") == 0)
//    {
//        parseDelS(command);
//    }
//    else if (strcmp(token, "update-m") == 0)
//    {
//        parseUpdateM(command);
//    }
//    else if (strcmp(token, "update-s") == 0)
//    {
//        parseUpdateS(command);
//    }
//    else if (strcmp(token, "calc-m") == 0)
//    {
//        parseCalcM(command);
//    }
//    else if (strcmp(token, "calc-s") == 0)
//    {
//        parseCalcS(command);
//    }
//    else if (strcmp(token, "calc-m all") == 0)
//    {
//        parseCalcMAll(command);
//    }
//    else if (strcmp(token, "ut-m") == 0)
//    {
//        parseUtM(command);
//    }
//    else if (strcmp(token, "ut-s") == 0)
//    {
//        parseUtS(command);
//    }
//    else if (strcmp(token, "exit") == 0)
//    {
//        return 0;
//    }
//    else
//    {
//        printf("Unknown command: %s\n", token);
//    }
//}
//
//void parseInsertM(const char* command)
//{
//    // Ваша логіка для обробки команди insert-m
//}
//
//void parseInsertS(const char* command)
//{
//    // Ваша логіка для обробки команди insert-s
//}
//
//void parseGetM(const char* command)
//{
//    // Ваша логіка для обробки команди get-m
//}
//
//void parseGetS(const char* command)
//{
//    // Ваша логіка для обробки команди get-s
//}
//
//void parseDelM(const char* command)
//{
//    // Ваша логіка для обробки команди get-m
//}
//
//void parseDelS(const char* command)
//{
//    // Ваша логіка для обробки команди get-s
//}
//
//void parseUpdateM(const char* command)
//{
//    // Ваша логіка для обробки команди get-m
//}
//
//void parseUpdateS(const char* command)
//{
//    // Ваша логіка для обробки команди get-s
//}
//
//void parseCalcM(const char* command)
//{
//    // Ваша логіка для обробки команди get-m
//}
//
//void parseCalcS(const char* command)
//{
//    // Ваша логіка для обробки команди get-s
//}
//
//void parseCalcMAll(const char* command)
//{
//    // Ваша логіка для обробки команди get-m
//}
//
//void parseUtM(const char* command)
//{
//    // Ваша логіка для обробки команди get-s
//}
//
//void parseUtS(const char* command)
//{
//    // Ваша логіка для обробки команди get-s
//}