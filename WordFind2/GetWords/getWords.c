#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
#include<stdbool.h>



//global variables to set the direction in the "patternSearch" function.
int u = 0;
int v = 0; 

//Function allocating space for the puzzle.
char** AllocMatrix(int grid_size)
{
    char** matrix = (char**)malloc(grid_size * sizeof(char*));
    for (int i = 0; i < grid_size; i++) {
        matrix[i] = (char*)malloc(grid_size * sizeof(char));
    }

    return matrix;
}



//search words in all the 8 directions
bool searchWord(char** grid, int row, int col, char word[], int size)
{
    int x[] = { -1, -1, -1,  0, 0,  1, 1, 1 }; //for columns
    int y[] = { -1,  0,  1, -1, 1, -1, 0, 1 }; //for rows                       

    if (grid[row][col] != word[0])              // if first letter does not match then word isnt present in the location.
        return false;

    int length = strlen(word);                  
    for (int dir = 0; dir < 8; dir++)
    {
        int k;
        int r = row + x[dir], c = col + y[dir];
        for (int k = 1; k < length; k++)
        {
            if (r >= size || r < 0 || c >= size || c < 0) //check if the word is out of bounds
                break;

            if (grid[r][c] != word[k])
            break;

            r += x[dir], c += y[dir];

            if (k == length-1)
            {
                u = x[dir];
                v = y[dir];
                
                return true;
            }

        }
       
    }
    return false;

}


//For inputting rows and colums into the searchWord Function and determining Direction.
void patternSearch(char** grid, char word[], int size)
{
    //check every position in the grid
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            if (searchWord(grid, row, col, word, size) == true)
            {
                printf("\n The word %s is at %d row and %d column", word, row+1, col+1);
                if (u == -1 && v == -1)
                {
                    printf(" Towards Left Top\n");
                }
                else if (u == -1 && v == 0)
                {
                    printf(" Upwards\n");
                }
                else if (u == -1 && v == 1)
                {
                    printf(" To Right Top\n");
                }
                else if (u == 0 && v == -1)
                {
                    printf(" Right to Left\n");
                }
                else if (u == 0 && v == 1)
                {
                    printf(" Left to Right\n");
                }
                else if (u == 1 && v == -1)
                {
                    printf(" To Bottom Left\n");
                }
                else if (u == 1 && v == 0)
                {
                    printf(" Downwards\n");
                }
                else if (u == 1 && v == 1)
                {
                    printf(" Towards Bottom Right\n");
                }
            }
        }
    }
}

int main()
{
    FILE* file; //grid file pointer
    int rows = 0;
    int cols = 0;
    int size = 0;
    char letter;
    char** grid;
    int i = 0, j = 0; //classic counter and index variables.

    char buf;

    //store 8 strings read from file
    char str1[30];
    char str2[50];
    char str3[50];
    char str4[50];
    char str5[50];
    char str6[50];
    char str7[50];
    char str8[50];



    // Open the file in read mode
    fopen_s(&file, "Puzzle.txt", "r");
   // fopen_s(&ofp, "rewrite puzzle.txt", "w");

    // Check if the file is opened successfully
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 0;
    }



    // Read the matrix elements from the file and determine dimensions
    while (fscanf(file, "%c", &letter) == 1) {
       


        // Check for a newline character to determine the end of a row
        if (fgetc(file) == '\n') {
            // Increment the row and column count. In this case the size since rows = columns.
            size++;
        }
    }

    printf("The puzzle size is %d rows x %d columns.\n", size, size);
    grid = AllocMatrix(size);
    fseek(file, 0, SEEK_SET); //reset the file pointer

    //Load the grid from the file
    for (i = 0; i < size; i++)
    {
        int temp = 0;
        for (j = 0; j <size*2; j++) { //size*2 accounts for white spaces.
            buf = fgetc(file);
            if (buf != ' ' && buf != '\n') {
                grid[i][temp] = buf;
                ++temp;
            }
        }
    }
   

    //reprinting the grid  
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }

    //load the strings to be found
    fscanf(file, "%s " "%s " "%s " "%s " "%s " "%s " "%s " "%s ", &str1, &str2, &str3, &str4, &str5, &str6, &str7, &str8);
    fclose(file);
    patternSearch(grid, str1, size);
    patternSearch(grid, str2, size);
    patternSearch(grid, str3, size);
    patternSearch(grid, str4, size);
    patternSearch(grid, str5, size);
    patternSearch(grid, str6, size);
    patternSearch(grid, str7, size);
    patternSearch(grid, str8, size);
    free(grid);
    int e=1;
    printf("\n Enter 0 to close");
    scanf("%d", &e);
    {
        if (e == 0)
        {
            exit(0);
        }
    }
}