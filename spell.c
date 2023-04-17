#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spell.h"
#define MAX_CAPACITY 101
/* Suggestions
- When you work with 2D arrays, be careful. Either manage the memory yourself, or
work with local 2D arrays. Note C99 allows parameters as array sizes as long as
they are declared before the array in the parameter list. See:
https://www.geeksforgeeks.org/pass-2d-array-parameter-c/
Worst case time complexity to compute the edit distance from T test words
to D dictionary words where all words have length MAX_LEN:
Student answer: Theta(T*D*MAX_LEN^2)
*/
/*
Parameters:
- table - double pointer to edit_distance array
- string1_len - length of first_string + 1 to accomodate " " space
- string2_len - length of second_string + 1 to accomodate " " space
- first_string - pointer to the first string
- second_string - pointer to the second string
Return: void (formatted printed table with edit distances)
*/
void print( int ** table, int string2_len, int string1_len, char * first_string,
char * second_string)
{
//Print initial row
for (int i = 0; i <= string1_len; i++)
{
if (i == 0)
{
printf(" |");
}
else if (i == 1)
{
printf(" |");
}
else
{
printf("%3c|", second_string[i-2]);
}
}
printf("\n---");
for (int i = 0; i < string1_len; i++)
{
printf("----");
}
printf("\n");
for (int i = 0; i < string2_len; i++)
{
for (int j = 0; j <= string1_len; j++)
{
if (i==0 & j==0)
{
printf(" |");
}
else if (j==0)
{
printf("%2c|", first_string[i-1]);
}
else
{
printf("%3d|", table[i][j-1]);
}
}
printf("\n---");
for (int i = 0; i < string1_len; i++)
{
printf("----");
}
printf("\n");
}
printf("\n");
return;
}
/*
Parameters:
- top - value of edit distance above current cell + 1
- left - value of edit distance to left current cell + 1
- diagonal - value of edit distance diagonally to the upper left of the current
cell (+ 1 if chars are not equal)
Return: the smallest value from top, left, or diagonal (ties do not matter)
*/
int compare_smallest(int top, int left, int diagonal)
{
if ((top < left) && (top < diagonal))
{
return top;
}
else if (left < diagonal)
{
return left;
}
else
{
return diagonal;
}
}
/*
Parameters:
- first_string - pointer to the first string (displayed horizontal in the table)
- second_string - pointer to the second string (displayed vertical in the table)
- print_table - If 1, the table with the calculations for the edit distance will
be printed.
If 0 (or any value other than 1)it will not print the table
Return: the value of the edit distance
*/
int edit_distance(char * first_string, char * second_string, int print_table){
int string1_len = strlen(second_string) + 1;
int string2_len = strlen(first_string) + 1;
//Create 2D array of ints to store edit_distance values
int ** table = (int**)malloc(sizeof(int*)*string2_len);
for (int i=0; i < string2_len; i++)
{
*(table+i) = (int*)malloc(string1_len * sizeof(int));
}
//Initialize first row and column
for (int i=0; i<string2_len; i++)
{
table[i][0] = i;
}
for (int j=0; j<string1_len; j++)
{
table[0][j] = j;
}
//Fill edit distance table
for (int i=1; i<string2_len; i++)
{
for (int j=1; j<string1_len; j++)
{
int top = table[i-1][j];
int left = table[i][j-1];
int diagonal = table[i-1][j-1];
if (first_string[i-1] != second_string[j-1])
{
diagonal = diagonal+1;
}
table[i][j] = compare_smallest(top+1, left+1, diagonal);
}
}
//Print table
if (print_table == 1)
{
print(table, string2_len, string1_len, first_string, second_string);
}
int result = table[string2_len-1][string1_len-1];
//Free allocated memory
for (int i=0; i < string2_len; i++)
{
free(table[i]);
}
free(table);
return result;
}
/*
Parameters:
- testname - string containing the name of the file with the paragraph to spell
check, includes file extenssion e.g. "text1.txt"
- dictname - name of file with dictionary words. Includes file extenssion, e.g.
"dsmall.txt"
Behavior: If any of the files cannot be open displays a message and returns. (Does
not exit the program.)
*/
void spell_check(char * testname, char * dictname){
int dictn = 0;
int testn = 0;
char buffer[MAX_CAPACITY];
int distmin = MAX_CAPACITY;
int count = 1;
int choice;
FILE *fp;
//Open file, read number of dictn words and store elements in a 2D array
fp =fopen(dictname, "r");
if (fp == NULL){
printf("Dictionary file not found.\n");
return;
}
fscanf(fp, "%d", &dictn);
char ** dict_table = (char**)malloc(sizeof(char*)*dictn);
for (int i=0; i < dictn; i++)
{
*(dict_table+i) = (char*)malloc(MAX_CAPACITY);
fscanf(fp, "%s", dict_table[i]);
}
fclose(fp);
// Open test name for reading and writing
// If file fails to open, free allocated memory before returning
fp = fopen(testname, "r+");
if (fp == NULL)
{
printf("Test file not found.\n");
for (int i=0; i < dictn; i++)
{
free(dict_table[i]);
}
free(dict_table);
return;
}
// Create an array of ints to store edit distance for each dictionary word
int * distarray = (int*)malloc(dictn*sizeof(int));
// Create an array of ints to store index to min distance words
int * distindex = (int*)malloc(sizeof(int));
// Check each entry in test file and find the minimum distance
fscanf(fp, "%d", &testn);
for (int i=0; i < testn; i++)
{
fscanf(fp, "%s", buffer);
printf("---> |%s|\n", buffer);
for (int j = 0; j < dictn; j++)
{
// Store distance in array and update minimum distance
distarray[j] = edit_distance(buffer, dict_table[j], 0);
if (distarray[j] < distmin)
{
distmin = distarray[j];
}
}
printf("-1 - type correction\n");
printf(" 0 - leave word as is (do not fix spelling)\n");
printf("Minimum distance: %d\n", distmin);
printf("Words that give minimum distance:\n");
for (int j = 0; j < dictn; j++)
{
if(distarray[j] == distmin)
{
printf("%2d - %s\n", count, dict_table[j]);
distindex[count-1] = j;
count++;
distindex = realloc(distindex, sizeof(int)*count);
}
}
printf("Enter your choice: ");
scanf("%d", &choice);
if (choice == -1)
{
printf("Enter correct word: ");
scanf("%s", buffer);
printf("The corrected word is : |%s|\n", buffer);
}
else if (choice == 0)
{
printf("The original word will be kept.\n");
}
else if (choice > 1 && (count-choice) > 0 &&
dict_table[(distindex[choice-1])] != NULL)
{
printf("The corrected word is : |%s|\n",
dict_table[(distindex[choice-1])]);
}
else
{
printf("Invalid choice. The original word will be kept.\n");
}
printf("\n");
count = 1;
}
fclose(fp);
free(distindex);
free(distarray);
for (int i=0; i < dictn; i++)
{
free(dict_table[i]);
}
free(dict_table);
return;
}
