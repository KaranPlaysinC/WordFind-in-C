#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
#include<stdbool.h>

#define EMPTY '-'    // Character representing empty spaces

enum direction {
    HORIZONTAL,
    VERTICAL,
    DIAGONAL
};
enum orientation {
    FORWARD,
    BACKWARD
};

typedef struct {
    char* wordtext;
    int startrow, startcol;
    int forward;
    int orientation;
    bool found;
}word;
typedef struct {
    word* wordsInCategory;
    char* categoryname;
    int catNum;
}Category;

int WillConflict(char** grid, int row, int col) {
    return grid[row][col] != EMPTY;
}

//Function to allocate grid space
char** AllocMatrix(int grid_size)
{
    char** matrix = (char**)malloc(grid_size * sizeof(char*));
    for (int i = 0; i < grid_size; i++) {
        matrix[i] = (char*)malloc(grid_size * sizeof(char));
    }
    return matrix;
}

// Function to display the grid
void InitializeMatrix(char** grid, int grid_size) {
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            grid[i][j] = EMPTY;
        }
    }
    return;
}

// Function to display the grid
void displayGrid(char** grid, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}



int canPlaceWord(char** grid, char* word, int size, int startrow, int startcol, enum direction d1, enum orientation d2) {
    int canplace = 1;
    int row = startrow, col = startcol;
    int len = strlen(word);
    for (int i = 0; i < len; ++i) {
        switch (d1) {
        case HORIZONTAL:
            row = startrow;
            if(d2 == FORWARD)
            {
                col = startcol + i;
            }
            else
            {
                col = startcol - i;
            }
            break;
        case VERTICAL:
            col = startcol;
            if (d2 == FORWARD)
            {
               row = startrow + i;
            }
            else
            {
                row = startrow - i;
            }
            break;
        case DIAGONAL:
            if (d2 == FORWARD)
            {
                row = startrow + i; 
                col = startcol + i;
            }
            else
            {
                row = startrow - i;
                col = startcol - i;
            }
            break;
        }
        if (row < 0 || row >= size || col < 0 || col >= size) {
            canplace = 0;
            break;
        }
        else if (WillConflict(grid, row, col) == 1) {
            if (grid[row][col] != word[i]) {
                canplace = 0;
                break;
            }
        }
    }
    return canplace;

}

// Function to place a word in the grid
void placeWord(char** puzzle, Category C1, int griddim) {
    enum direction d[8];
    enum orientation d2[8];
    for (int i = 0; i < 8; ++i) {    //initalize orientations
        d[i] = C1.wordsInCategory[i].orientation;
        d2[i] = C1.wordsInCategory[i].forward;
    }
    for (int i = 0; i < 8; ++i) { //find places for words and fill words
        int startrow, startcol;
        do {
            if (d[i] == HORIZONTAL) {
                startrow = rand() % griddim;
                startcol = rand() % (griddim - strlen(C1.wordsInCategory[i].wordtext) + 1);
            }
            else if (d[i] == VERTICAL) {
                startrow = rand() % (griddim - strlen(C1.wordsInCategory[i].wordtext) + 1);
                startcol = rand() % griddim;
            }
            else {//DIAGONAL
                startrow = rand() % (griddim - strlen(C1.wordsInCategory[i].wordtext) + 1);
                startcol = rand() % (griddim - strlen(C1.wordsInCategory[i].wordtext) + 1);
            }
        } while (!canPlaceWord(puzzle, C1.wordsInCategory[i].wordtext, griddim, startrow, startcol, d[i], d2[i])); //keep cycling until find a suitable place for the word
        C1.wordsInCategory[i].startcol = startcol;
        C1.wordsInCategory[i].startrow = startrow;
        for (int k = 0; k < strlen(C1.wordsInCategory[i].wordtext); ++k) { // Populate the puzzle
            int row, col;
            switch (d[i]) {
            case HORIZONTAL:
                row = startrow;
                col = d2[i] == FORWARD ? startcol + k : startcol - k;
                break;
            case VERTICAL:
                row = d2[i] == FORWARD ? startrow + k : startrow - k;
                col = startcol;
                break;
            case DIAGONAL:
                row = d2[i] == FORWARD ? startrow + k : startrow - k;
                col = d2[i] == FORWARD ? startcol + k : startcol - k;
            }
            puzzle[row][col] = C1.wordsInCategory[i].wordtext[k];
        }
    }
}

bool checkWord(Category C1, char* s, int* index) {
    bool found = false;
    for (int i = 0; i < 8; i++) {
        if (C1.wordsInCategory[i].found == false && strcmp(C1.wordsInCategory[i].wordtext, s) == 0) {
            found = true;
            *index = i;
        }
    }
    return found;
}

void ShowRemaining(Category C1) {
    for (int i = 0; i < 8; i++) {
        if (!C1.wordsInCategory[i].found)
            printf("\n%s", C1.wordsInCategory[i].wordtext);
    }
    printf("\n");
    return;
}

//check if the user selected word is present in the grid


void Help(Category C1) { //function to handle Help input
    bool helped = true;
    while (helped) {
        int i = rand() % 8;
        if (C1.wordsInCategory[i].found == false) {
            helped = false;
            printf("The Help word is : %s, ", C1.wordsInCategory[i].wordtext);
            printf("It is located at : %d,%d", C1.wordsInCategory[i].startrow + 1, C1.wordsInCategory[i].startcol + 1);
            C1.wordsInCategory[i].found = true;
        }
    }
    return;
}
 


// Function to fill empty spaces in the grid with random letters
void fillEmptySpaces(char** grid, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j] == EMPTY) {
                grid[i][j] = 'A' + rand() % 26; // Assign a random letter
            }
        }
    }
}
void capitalize(char* userstring) {   //capitalize the user entered string
    int len = strlen(userstring);
    for (int i = 0; i < len; ++i) {
        userstring[i] = toupper(userstring[i]);
    }
    return;
}
int checkcat(Category* CL, char* userinput) {
    int flag = 0;
    int found;
    for (int i = 0; i < 10; i++) {
        if (strcmp(CL[i].categoryname, userinput) == 0) {
            flag = 1;
            found = i;
            break;
        }
    }
    if (flag == 1) {
        return found;
    }
    else {
        return -1;
    }
}
int randomint(int min, int max) { 
    return (rand() % (max - min + 1) + min);
}
void random8(int* a, int min, int max) {
    int range = max - min;
    int n = 0;
    while (n < 8) { 
        int randnum = randomint(min, max);
        int isunique = 1;
        for (int i = 0; i < n; i++) { 
            if (a[i] == randnum) {
                isunique = 0;
                break;
            }
        }
        if (isunique) {
            a[n] = randnum;
            n++;
        }
    }
    return;
}

int main() {

    //initializations   
    Category C1, * AllCats;
    int size;
    FILE* sfp, * ofp;
    char buf[50];
    char userword[50];
    int i = 0;
    int j = 0;
    char usercat[50];
    int pos;
    int diff8[8];
    char** grid;
    int tbf=8; //Number of word to be found
    int found;
    int ind;
    int userrow, usercol;
    int helpcount=0;

    // Seed the random number generator
    srand(time(NULL));
    
    fopen_s(&sfp, "Words.txt", "r"); //read words from file
    fopen_s(&ofp, "Puzzle.txt", "w");   //to write the grid into a file


   
    //Allocate Memory for categoryies and words
    AllCats = (Category*)malloc(10 * sizeof(Category));
    for (int i = 0; i < 10; i++) {
        AllCats[i].wordsInCategory = (word*)malloc(15 * sizeof(word));
        AllCats[i].categoryname = (char*)malloc(35 * sizeof(char));
        for (int j = 0; j < 15; j++) {
            AllCats[i].wordsInCategory[j].wordtext = (char*)malloc(sizeof(char) * 50);
        }
    }
    C1.categoryname = (char*)malloc(35 * sizeof(char));

    //Get Category and following words from file
    for (i = 0; i < 10; i++) {
        fscanf(sfp, "%s", &buf);
        strcpy(AllCats[i].categoryname, buf);
        for (j = 0; j < 15; ++j) {
            fscanf(sfp, "%s", &buf);
            strcpy(AllCats[i].wordsInCategory[j].wordtext, buf);
        }
    }
    // Get the size of the grid from the user
L0: printf("\nChoose the size of the grid from either 20 or 30:\n");
    scanf("%d", &size);

    // Validate the size
    if (size != 20 && size != 30) {
        printf("Invalid size. Please re-enter\n");
        goto L0;
    }

    //show categories
    printf("\nCategories:\n");
    for (int i = 0; i < 10; i++) {
        printf("%s\n", AllCats[i].categoryname);
    }

    //User Inputs Category name
L1:	printf("\n Kindly enter your choice of category: ");
    scanf("%s", usercat);
    capitalize(usercat);
    pos = checkcat(AllCats, usercat);
    if (pos == -1) {         //Assert error in category input
        printf("Invalid Category\n");
        printf("%s", usercat);
        goto L1;
    }

    //Get 8 Random words from selected category
    random8(diff8, 0, 14);
    C1.wordsInCategory = (word*)malloc(sizeof(word) * 8);
    for (int i = 0; i < 8; ++i) {
        C1.wordsInCategory[i].wordtext = (char*)malloc(sizeof(char) * 35);
        strcpy(C1.wordsInCategory[i].wordtext, AllCats[pos].wordsInCategory[diff8[i]].wordtext);
    }
    strcpy(C1.categoryname, AllCats[pos].categoryname);

    //create grid
    grid = AllocMatrix(size);
    InitializeMatrix(grid, size);

    //Assign orientation and direction
    for (int i = 0; i < 8; ++i) {   
        C1.wordsInCategory[i].orientation = randomint(0, 2);
        C1.wordsInCategory[i].forward = randomint(0, 1);

    }
    placeWord(grid, C1, size); //place words in grid
 
    // Fill empty spaces with random letters
    fillEmptySpaces(grid, size);

    //set found as false before solving
    for (int i = 0; i < 8; ++i) {
    C1.wordsInCategory[i].found = false;
    }


    //Copy Grid to File
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {

            if (j == size - 1)
            {
                fprintf(ofp, "%c", grid[i][j]);
            }
            else
            {
                fprintf(ofp, "%c ", grid[i][j]);
            }
            
        }
        fprintf(ofp,"\n");
    }

    //write words to be found in the file.
    for (int i = 0; i < 8; i++) {
        if (!C1.wordsInCategory[i].found)
        {
            printf("\n%s", C1.wordsInCategory[i].wordtext);
            fprintf(ofp, "%s ", C1.wordsInCategory[i].wordtext);
        }
    }

    fclose(ofp);
    fclose(sfp);
    free(AllCats);
   //free(grid);

    
 L3: if (tbf == 0) { // is puzzle solved
	printf("\n You have found all the words:");
    free(grid);
	getchar();
	exit(0);
	}

    //list down the words to be found
	printf("\n%d Words to find are: \n", tbf);
	ShowRemaining(C1);
    

	displayGrid(grid, size);
    

	//Solving the Puzzle
L4:	printf("Enter the word you found!: ");
	scanf("%s", userword);
	capitalize(userword);
	found = checkWord(C1, userword,&ind);
	if (strcmp(userword, "HELP") == 0)
    {
        helpcount += 1;
        if (helpcount > 3)//Limit the number of times user can opt for help
        {
            printf("\n No more Help available.\n");
            goto L4;
        }
		Help(C1);
		--tbf;
		goto L3;
	}
	if (found == false) {                            
		printf("Invalid input. Try again\n");
		goto L4;
	}
L5:	printf("The word is present! Enter the row number of the starting letter! : ");
	scanf("%d", &userrow);

    printf("Enter the column number of the starting letter! : ");
    scanf("%d", &usercol);
	if ((C1.wordsInCategory[ind].startrow+1 == userrow) && ((C1.wordsInCategory[ind].startcol+1) == usercol)) {
		printf("Great!You Found it!\n");
		--tbf;
		C1.wordsInCategory[ind].found = true;
		goto L3;
	}
	else {
		printf("Hmmm Thats not it. Try again!\n");
		goto L5;
	}
	if (tbf >= 0) {
		goto L3;
	}
}