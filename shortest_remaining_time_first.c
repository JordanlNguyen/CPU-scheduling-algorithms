#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct{
    int pid;
    int at;
    int bt;
    int priority;
    int ct;
    int tat;
    int wt;
    int resp_time;
    int first_time_cpu;
    int remaining_bt;
}process;

void srtf(float q){
    int p = q;
    //generate array for pid
    process pid[p];
    pid[0].pid = 0; pid[0].at = rand()%50; pid[0].bt = rand()%50; pid[0].priority = rand()%50; pid[0].remaining_bt = pid[0].bt;
    int max_at = pid[0].at; int min_at = pid[0].at; 
    int max_bt = pid[0].bt; int min_bt = pid[0].bt;
    int min_pr = pid[0].priority; int max_pr = pid [0].priority;
    for(int i = 1; i < p; i++){
        pid[i].pid = i;
        pid[i].at = rand()%50;
        if(pid[i].at > max_at){max_at = pid[i].at;}
        if(pid[i].at < min_at){min_at = pid[i].at;}
        pid[i].bt = rand()%50;
        pid[i].remaining_bt = pid[i].bt;
        if(pid[i].bt > max_bt){max_bt = pid[i].bt;}
        if(pid[i].bt < min_bt){min_bt = pid[i].bt;}
        pid[i].priority = rand()%50;
        if(pid[i].priority > max_pr){max_pr = pid[i].priority;}
        if(pid[i].priority < min_pr){min_pr = pid[i].priority;}
    }
    //print mins and maxes
    printf("----- minimum and maximums -----\n\n");
    printf("minimum arrival time: %d\n", min_at); 
    printf("maximum arrival time: %d\n", max_at);
    printf("minimum burst time: %d\n", min_bt); 
    printf("maximum burst time: %d\n", max_bt);
    printf("minimum priority time: %d\n", min_pr); 
    printf("maximum priority time: %d\n\n", max_pr);
    //print table
    printf("--------------------- process table ---------------------\n\n");
    printf("%-15s %-15s %-15s %-15s\n\n", "proccess id", "arrival time", "burst time" ,"priority");
    for(int i = 0; i < p; i++){
        printf("%-15d %-15d %-15d %-15d\n", pid[i].pid, pid[i].at, pid[i].bt, pid[i].priority);
        printf("----------------------------------------------------------\n\n");
    }
    //calculate srtf
    int time = 0;
    int completed = 0;
    int shortest = -1;
    int shortest_time = 1000;
    while(completed < p){ //while the number of completed processes is less than total processes
        //find the shortest job
        for(int i = 0; i < p; i++){
            if(pid[i].at <= time && pid[i].remaining_bt > 0 && pid[i].remaining_bt < shortest_time){
                shortest = i;
                shortest_time = pid[i].remaining_bt;
            }
        }
        //checks if there is a process within the time frame
        if(shortest == -1){
            time++;
        }
        else{
            //finds a process with the shortest time
            //checks if first time getting cpu
            if(pid[shortest].bt == pid[shortest].remaining_bt){pid[shortest].first_time_cpu = time;}
            pid[shortest].remaining_bt--;
            time++;
            //calculates rest of times when completed
            if(pid[shortest].remaining_bt == 0){
                pid[shortest].ct = time;
                pid[shortest].tat = pid[shortest].ct - pid[shortest].at;
                pid[shortest].wt = pid[shortest].tat - pid[shortest].bt;
                pid[shortest].resp_time = pid[shortest].first_time_cpu - pid[shortest].at;
                completed++;
                shortest_time = 1000;
                shortest = -1;
            }
        }
    }
     //display complete table
    printf("---------------------------------------- complete table ----------------------------------------\n\n");
    printf("%-15s %-15s %-15s %-15s %-20s %-20s %-15s %s\n", "proccess id", "arrival time", "burst time" ,"priority", "completion time",
     "turn around time", "wait time", "response time");
    for(int i = 0; i < p; i++){
        printf("%-15d %-15d %-15d %-15d %-20d %-20d %-15d %d\n", pid[i].pid, pid[i].at, pid[i].bt, pid[i].priority, pid[i].ct, pid[i].tat, pid[i].wt, pid[i].resp_time);
        printf("---------------------------------------------------------------------------------------------------------------------------------------\n");
    }
    //generate gant chart
    time = 0;
    completed = 0;
    shortest = -1;
    shortest_time = 1000;
    //resets remaining time
    for(int i = 0; i < p; i++){
        pid[i].remaining_bt = pid[i].bt;
    }
    printf("-------- gant chart --------\n\n");
    printf("%-13s %-4s\n\n", "process", "time");
    while(completed < p){
        for(int i = 0; i < p; i++){
            if(pid[i].at <= time && pid[i].remaining_bt > 0 && pid[i].remaining_bt < shortest_time){
                shortest = i;
                shortest_time = pid[i].remaining_bt;
            }
        }
        if(shortest == -1){
            printf("--------------%d\n",time);
            time++;
        }
        else{
            printf("--------------%d\n",time);
            printf("process %d\n", pid[shortest].pid);
            pid[shortest].remaining_bt--;
            time++;
            if(pid[shortest].remaining_bt == 0){
                //check if it is the last to be completed
                if(completed == p - 1){
                    printf("--------------%d\n",pid[shortest].ct);
                    break;
                }
                completed++;
                shortest_time = 1000;
                shortest = -1;
            }
        }
    }
    //averages 
    printf("--------------------- Averages ---------------------\n\n");
    //totals 
    float tot_wait = 0;
    float tot_tat = 0;
    float tot_resp = 0;
    for(int i = 0; i < p; i++){
        tot_wait += pid[i].wt;
        tot_tat += pid[i].tat;
        tot_resp += pid[i].resp_time;
    }
    int actual_time = 0;
    for(int i = 0; i < p; i++){
        if(pid[i].ct > actual_time){actual_time = pid[i].ct;}
    }
    float av_wait = tot_wait/q;
    float av_tat = tot_tat/q;
    float av_resp = tot_resp/q;
    printf("Average Wait time: %f\n", av_wait);
    printf("Average Turn Around Time: %f\n", av_tat);
    printf("Average Response Time: %f\n", av_resp);
    printf("Throughput: %d/%d\n", p, actual_time);

    
}

int main(){
    float amount;
    bool valid = false;
    while(valid == false){
        printf("Enter amount of processes (maximum of 50): ");
        scanf("%f", &amount);
        if(amount > 0 && amount <= 50){valid = true;}
        else{printf("incorrect input, try again");}
    }
    printf("\n");
    srtf(amount);
    return 0;
}
