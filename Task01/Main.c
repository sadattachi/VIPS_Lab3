#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#define NUMBER_OF_BOOKS_IN_FILE 7

struct SBook {
  char m_szAuthorFullName[30];
  char m_szTitle[50];
  int m_nYear;
  int m_nPagesCount;
  int m_nPrice;
  struct SBook *m_psbNext;
};
//------------------------------------------------------------------------------
// Gets struct field from file, parses strings, initializes fields, adds to end
void ReadStruct(
    FILE *psfRead, 
    char szBookInfo[], 
    char szSeparators[], 
    char *pcSeparatedInfo, 
    struct SBook *psbCurrent, 
    struct SBook **psbStart);
//------------------------------------------------------------------------------
// Sorts books by release year in descending order
void SortBooksByYear(struct SBook **psbStart);
//------------------------------------------------------------------------------
// Swaps two books in list
void SwapBooks(
    struct SBook *psbLeft, 
    struct SBook *psbRight, 
    struct SBook **psbStart);
//------------------------------------------------------------------------------
// Allows you to enter data from console to add book in list
void AddNewBookFromConsole(struct SBook **psbStart);
//------------------------------------------------------------------------------
// Deletes first book from list with given name
void DeleteBookByName(struct SBook **psbStart);
//------------------------------------------------------------------------------
// Returns average price of books
int CountAveragePrice(struct SBook *psbStart);
//------------------------------------------------------------------------------
/* Using CountAveragePrice function permanently deletes every book, 
   which price is lower that average,  from list */
void DeleteLowerThanAverage(struct SBook **psbStart);
//------------------------------------------------------------------------------
// Prints every books in console
void PrintBooksTable(struct SBook *psbStart);
//------------------------------------------------------------------------------
// Prints line used to create table
void PrintSeparatorLine();

int main() {
  struct SBook *psbCurrent = (struct SBook*)malloc(sizeof(struct SBook));
  struct SBook *psbStart = NULL;
  
  char szBookInfo[100];
  char szSeparators[] = "<>";
  char *pcSeparatedInfo = NULL;
  FILE *psfRead = NULL;
  if ((psfRead = fopen("books.txt", "r")) == NULL) {
    printf("Cannot open file.");
    return 0;
  }
  ReadStruct(psfRead, szBookInfo, szSeparators, 
  pcSeparatedInfo, psbCurrent, &psbStart);
  fclose(psfRead);
  SortBooksByYear(&psbStart);
  PrintBooksTable(psbStart);
  
  AddNewBookFromConsole(&psbStart);
  SortBooksByYear(&psbStart);
  PrintBooksTable(psbStart);
  
  DeleteBookByName(&psbStart);
  PrintBooksTable(psbStart);
  SortBooksByYear(&psbStart);
  
  DeleteLowerThanAverage(&psbStart);
  PrintBooksTable(psbStart);
  return 0;
}
//------------------------------------------------------------------------------
void ReadStruct(
    FILE *psfRead, 
    char szBookInfo[], 
    char szSeparators[], 
    char *pcSeparatedInfo, 
    struct SBook *psbCurrent, 
    struct SBook **psbStart) {
  for (int j = 0; j < NUMBER_OF_BOOKS_IN_FILE; j++) {
    fgets(szBookInfo, 100, psfRead);
    pcSeparatedInfo = strtok(szBookInfo, szSeparators);
    strcpy(psbCurrent->m_szAuthorFullName, pcSeparatedInfo);
    pcSeparatedInfo = strtok(NULL, szSeparators);
    strcpy(psbCurrent->m_szTitle, pcSeparatedInfo);
    pcSeparatedInfo = strtok(NULL, szSeparators);
    psbCurrent->m_nYear = atoi(pcSeparatedInfo);
    pcSeparatedInfo = strtok(NULL, szSeparators);
    psbCurrent->m_nPagesCount = atoi(pcSeparatedInfo);
    pcSeparatedInfo = strtok(NULL, szSeparators);
    psbCurrent->m_nPrice = atoi(pcSeparatedInfo);
    if (!(*psbStart))
      (*psbStart) = psbCurrent;
    struct SBook *m_psbNext = (struct SBook*)calloc(1, sizeof(struct SBook));
    m_psbNext->m_psbNext = NULL;
    psbCurrent->m_psbNext = m_psbNext;
    psbCurrent = m_psbNext;
  }
  
}
//------------------------------------------------------------------------------
void SortBooksByYear(struct SBook **psbStart) {
  struct SBook *psbStartCopy = (*psbStart);
  int nChecker = 1;
  while (1) {
    nChecker = 1;
    psbStartCopy = (*psbStart);
    while (1) {
      if (psbStartCopy->m_psbNext->m_nYear) {
        if (psbStartCopy->m_nYear > psbStartCopy->m_psbNext->m_nYear) {
          SwapBooks(psbStartCopy, psbStartCopy->m_psbNext, psbStart);
          nChecker = 0;
          break;
        } else {
          psbStartCopy = psbStartCopy->m_psbNext;
        }
      } else {
        break;
      }
    }
    if (nChecker) {
      break;
    }
  }
}
//------------------------------------------------------------------------------
void PrintBooksTable(struct SBook *start) {
  printf("\n\n\nAuthor\t\t\tBook\t\t\t\t\t       Year\t  Pages\t       Value\n");
  PrintSeparatorLine();
  while (start->m_nYear) {
    printf("\n|%-15s\t|%-35s\t|%10d\t|%5d\t|\t%1d\t|\n",
        start->m_szAuthorFullName, start->m_szTitle, start->m_nYear, 
        start->m_nPagesCount, start->m_nPrice);
    PrintSeparatorLine();
    start = start->m_psbNext;
  }
  printf("\n");
}
//------------------------------------------------------------------------------
void PrintSeparatorLine() {
  for (int j = 0; j < 105; j++)
    printf("=");
}
//------------------------------------------------------------------------------
void SwapBooks(
    struct SBook *psbLeft, 
    struct SBook *psbRight, 
    struct SBook **psbStart) {
  if (psbLeft == (*psbStart)) {
    psbLeft->m_psbNext = psbRight->m_psbNext;
    psbRight->m_psbNext = psbLeft;
    (*psbStart) = psbRight;
  } else {
    struct SBook *psbIterator = (*psbStart);
    while (1) {
      if (psbIterator->m_psbNext == psbLeft) {
        psbIterator->m_psbNext = psbRight;
        psbLeft->m_psbNext = psbRight->m_psbNext;
        psbRight->m_psbNext = psbLeft;
        break;
        } else {
          if (psbIterator->m_psbNext) {
            psbIterator = psbIterator->m_psbNext;
        }
      }
    }
  }
}
//------------------------------------------------------------------------------
void AddNewBookFromConsole(struct SBook **psbStart) {
  char szInsertInfo[50];
  int nInsertInfo = 0;
  struct SBook *psbToInsert = (struct SBook*)calloc(1, sizeof(struct SBook));
  
  printf("Enter author: ");
  fgets(szInsertInfo, sizeof(szInsertInfo), stdin);
  szInsertInfo[strlen(szInsertInfo) - 1] = 0;
  strcpy(psbToInsert->m_szAuthorFullName, szInsertInfo);
  
  printf("Enter book's name: ");
  fgets(szInsertInfo, sizeof(szInsertInfo), stdin);
  szInsertInfo[strlen(szInsertInfo) - 1] = 0;
  strcpy(psbToInsert->m_szTitle, szInsertInfo);
  
  printf("Enter year: ");
  scanf("%d", &nInsertInfo);
  psbToInsert->m_nYear = nInsertInfo;
  printf("Enter number of pages: ");
  scanf("%d", &nInsertInfo);
  psbToInsert->m_nPagesCount = nInsertInfo;
  printf("Enter price: ");
  scanf("%d", &nInsertInfo);
  psbToInsert->m_nPrice = nInsertInfo;
  
  psbToInsert->m_psbNext = (*psbStart);
  (*psbStart) = psbToInsert;
}
//------------------------------------------------------------------------------
void DeleteBookByName(struct SBook **psbStart) {
  struct SBook *psbStartCopy = (*psbStart);
  char szToDelete[50];
  gets();
  printf("Enter name of a book to delete: ");
  fgets(szToDelete, sizeof(szToDelete), stdin);
  szToDelete[strlen(szToDelete) - 1] = 0;
  if (!strcmp(psbStartCopy->m_szTitle, szToDelete)) {
    (*psbStart) = (*psbStart)->m_psbNext;
    free(psbStartCopy);
    return;
  }
  while (1) {
    if (psbStartCopy->m_psbNext->m_nYear) {
      if (!strcmp(psbStartCopy->m_psbNext->m_szTitle, szToDelete)) {
        psbStartCopy = psbStartCopy->m_psbNext;
      } else {
      psbStartCopy->m_psbNext = psbStartCopy->m_psbNext->m_psbNext;
      break;
      }
    }
  }
}
//------------------------------------------------------------------------------
int CountAveragePrice(struct SBook *psbStart) {
  int nSum = 0;
  int nNumberOfBooks = 0;
  while (psbStart->m_psbNext->m_nYear) {
    nSum += psbStart->m_nPrice;
    psbStart = psbStart->m_psbNext;
    nNumberOfBooks++;
  }
  return nSum / nNumberOfBooks;
}
//------------------------------------------------------------------------------
void DeleteLowerThanAverage(struct SBook **psbStart) {
  struct SBook *psbStartCopy = (*psbStart);
  int nAverage = CountAveragePrice((*psbStart));
  while (psbStartCopy->m_nPrice < nAverage) {
    (*psbStart) = (*psbStart)->m_psbNext;
    free(psbStartCopy);
    psbStartCopy = (*psbStart);
  }
  psbStartCopy = (*psbStart);
  while (1) {
    if (psbStartCopy->m_psbNext->m_nYear) {
      if (psbStartCopy->m_psbNext->m_nPrice > nAverage) {
        psbStartCopy = psbStartCopy->m_psbNext;
      } else {
        psbStartCopy->m_psbNext = psbStartCopy->m_psbNext->m_psbNext;
        psbStartCopy = (*psbStart);
        continue;
      }
    }
    break;
  }
}