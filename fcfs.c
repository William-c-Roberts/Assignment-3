#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

#include<ctype.h>
#include<sys/wait.h>

struct threadinfo
{
    int p_id;
    int arr_time;
    int burst_time;
    int waiting_time;
    int completion_time;
    int turn_around_time;
    /* data */
};




int main(int argc, char *argv[]){
    FILE *file = fopen(argv[1],"r");
    char line[256];
    int temp_arr[3];
    int i = 0;
    int x = 0;
    int timer = 0;
    int count=0;
    float avg_wait = 0;
    float avg_tat = 0;
    char c;
    


    

    while(fscanf(file, "%c", &c)!=EOF){
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
    }
    
    fclose(file); 
    file = fopen(argv[1], "r");  
    struct threadinfo ti[count];



    while ((fgets(line, sizeof(line), file))){
        char * token = strtok(line, ", ");
        i = 0;
        while (token != NULL){
            temp_arr[i] = atoi(token);
            //printf("string = %s",token);
            token = strtok(NULL,", ");
            i = i+1;
        }
        ti[x].p_id = temp_arr[0];
        ti[x].arr_time = temp_arr[1];
        ti[x].burst_time = temp_arr[2];
        x = x+1;   
    }
    fclose(file);

    for (int i = 0; i<count; ++i){
        if (ti[i].arr_time > timer){
            timer = ti[i].arr_time;
        }
        ti[i].completion_time = (timer + ti[i].burst_time);
        timer = timer + ti[i].burst_time;
        ti[i].turn_around_time = (ti[i].completion_time-ti[i].arr_time);
        ti[i].waiting_time = (ti[i].turn_around_time - ti[i].burst_time);
    }
    printf("Thread ID\tArrival Time\tBurst Time\tCompletion Time\tTurn-Around Time\tWaiting Time\n");
    for (int i = 0; i<count; ++i){
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n", ti[i].p_id,ti[i].arr_time, ti[i].burst_time,ti[i].completion_time,ti[i].turn_around_time, ti[i].waiting_time);
        avg_wait = avg_wait + ti[i].waiting_time;
        avg_tat = avg_tat + ti[i].turn_around_time;
      
    }
    avg_wait = avg_wait/count;
    avg_tat = avg_tat/count;
    
    printf("The average waiting time: %.2f\nThe average turn around time: %.2f\n",avg_wait,avg_tat);

}
