//libraries
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

//constants
#define ROWS 9
#define COLUMNS 9
#define NUM_THREADS 11

//structures
//structure for passing data to threads
typedef struct{
    int row;
    int column;
    int index;
}PARAMETERS;

//global variables
int sudokuPuzzle[ROWS][COLUMNS];
int results[11];

int linear_search(int *array, int key, int n){
    for(int i = 0; i < n; i++){
        if(array[i] == key){
            return i;
        }
    }
    return -1;
}


void readFile(int array[ROWS][COLUMNS], char *fileName){
    FILE *fp = fopen(fileName, "r");
    if(fp < 0){
        perror("File open error\n");
        return;
    }

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            fscanf(fp, "%d", &array[i][j]);
        }
    }
    fclose(fp);
}
void* check_columns(void *arg){
    PARAMETERS *data = (PARAMETERS *) arg;
    int seen[9];
    int flag_in_seen = 0;
    for(int j = 0; j < COLUMNS; j++){
        for(int m = 0; m < 9; m++){
            seen[m] = -1;
        }

        for(int i = 0; i < ROWS; i++){
            if(sudokuPuzzle[i][j] < 1 || sudokuPuzzle[i][j] > 9){
                pthread_exit(NULL);
            }

            flag_in_seen = linear_search(seen, sudokuPuzzle[i][j], i);
            if(flag_in_seen != -1){
                pthread_exit(NULL);
            }
            else{
                seen[i] = sudokuPuzzle[i][j];
            }
        }
    }
    results[data->index] = 1;
    pthread_exit(NULL);
}

void* check_rows(void *arg){
    PARAMETERS *data = (PARAMETERS *) arg;
    int seen[9];
    int flag_in_seen = 0;
    for(int i = 0; i < ROWS; i++){
        for(int m = 0; m < 9; m++){
            seen[m] = -1;
        }
        for(int j = 0; j < COLUMNS; j++){
            if(sudokuPuzzle[i][j] < 1 || sudokuPuzzle[i][j] > 9){
                pthread_exit(NULL);
            }

            flag_in_seen = linear_search(seen, sudokuPuzzle[i][j], j);
            if(flag_in_seen != -1){
                pthread_exit(NULL);
            }
            else{
                seen[j] = sudokuPuzzle[i][j];
            }
        }
    }
    results[data->index] = 1;
    pthread_exit(NULL);
}

void* check_grid(void* arg){
    PARAMETERS *data = (PARAMETERS *) arg;
    int start_row_index = 3*data->row;
    int start_column_index = 3*data->column;
    int seen[9] = {0};
    int flag_in_seen = 0;
    int i;
    int j;
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            //printf("%d", sudokuPuzzle[i+start_row_index][j+start_column_index]);
            if(sudokuPuzzle[i+start_row_index][j+start_column_index] < 1 || sudokuPuzzle[i+start_row_index][j+start_column_index] > 9){
                pthread_exit(NULL);
            }
            flag_in_seen = linear_search(seen, sudokuPuzzle[i+start_row_index][j+start_column_index], i*3+j);
            if(flag_in_seen != -1){
                pthread_exit(NULL);
            }
            else{
                seen[i*3+j] = sudokuPuzzle[i+start_row_index][j+start_column_index];
            }
        }
    }
    results[data->index] = 1;
    pthread_exit(NULL);

}
void display_sudoku(){
    printf("Sudoku Puzzle Solution is:\n");
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS;j++){
            printf("%d ", sudokuPuzzle[i][j]);
        }
        printf("\n");
    }
}

void display_validation(){
    int flag = 0;
    for(int i = 0; i < NUM_THREADS; i++){
        if(results[i] != 1){
            flag = -1;
        }
    }
    if(flag == 0){
        printf("Sudoku puzzle is valid\n");
    }else{
        printf("Sudoku puzzle is invalid\n");
    }
}
int main(int argc, char* argv[]){
    if (argc != 2)
    {
        perror("Insufficient parameters passed.\n");
        return 1;
    }
    pthread_t tids[NUM_THREADS];//array of threads
    pthread_attr_t attr[NUM_THREADS];//array of thread attributes
    readFile(sudokuPuzzle, argv[1]);
    for(int i = 0; i < NUM_THREADS; i++){
        //get default attributes
        pthread_attr_init(&attr[i]);
        //create thread
        if(i == 9){
            //check column thread
            PARAMETERS* data = (PARAMETERS*) malloc(sizeof(PARAMETERS));
            data->row = 0;
            data->column = 0;
            data->index = i;
            pthread_create(&tids[i], &attr[i], check_columns, data);
        }
        else if(i == 10){
            //check row thread
            PARAMETERS* data = (PARAMETERS*) malloc(sizeof(PARAMETERS));
            data->row = 0;
            data->column = 0;
            data->index = i;
            pthread_create(&tids[i], &attr[i], check_rows, data);
        }
        else{
            //check box thread
            PARAMETERS* data = (PARAMETERS*) malloc(sizeof(PARAMETERS));
            data->row = i / 3;
            data->column = i % 3;
            data->index = i;
            pthread_create(&tids[i], &attr[i], check_grid, data);
        }
    }

    //joining threads 
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(tids[i], NULL);
    }

    display_sudoku();
    display_validation();
    return 0;
}