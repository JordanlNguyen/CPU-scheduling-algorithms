#include <stdbool.h>
#include <stdio.h>
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

// void sort_back(process pid[], int p){
//     for(int i = 0; i < p; i++){
//         for(int j = 0; j < p; j++){
//             if(pid[i].pid < pid[j].pid){
//                 process temp = pid[i];
//                 pid[i] = pid[j];
//                 pid[j] = temp;
//             }
//         }
//     }
// }

void sort(process pid[], int p){
    for(int i = 0; i < p; i++){
        for(int j = 0; j < p; j++){
            if(pid[i].at < pid[j].at){
                process temp = pid[i];
                pid[i] = pid[j];
                pid[j] = temp;
            }
        }
    }
}

void round_robin(float r, int q){
    int p = r;
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

    //sort by arrival time
    sort(pid, p);
    int completed = 0;
    int current = 0;
    int time = 0;
    int curr_quantum = q;
    while(completed < p){
        if(pid[current].at > time){
            time++;
        }
        else{
            if(pid[current].bt == pid[current].remaining_bt){pid[current].first_time_cpu = time;}
            if(pid[current].remaining_bt > 0){
                while(q != 0){
                    time++;
                    pid[current].remaining_bt--;
                    curr_quantum--;
                    if(pid[current].remaining_bt == 0){
                        //calculate time
                        pid[current].ct = time;
                        pid[current].tat = pid[current].ct - pid[current].at;
                        pid[current].wt = pid[current].tat - pid[current].bt;
                        pid[current].resp_time = pid[current].first_time_cpu - pid[current].at;
                        completed++;
                        curr_quantum = q;
                        current = (current + 1)%p;
                        break;
                    }
                    if(curr_quantum == 0){
                        curr_quantum = q;
                        current = (current + 1)%p;
                        break;
                    }
                }
            }
            else if(pid[current].remaining_bt == 0){
                current = (current + 1)%p;
            }
        }
    }
    //display complete table
    printf("---------- complete table ----------\n\n");
    printf("%-15s %-15s %-15s %-15s %-20s %-20s %-15s %s\n", "proccess id", "arrival time", "burst time" ,"priority", "completion time",
     "turn around time", "wait time", "response time");
    for(int i = 0; i < p; i++){
        printf("%-15d %-15d %-15d %-15d %-20d %-20d %-15d %d\n", pid[i].pid, pid[i].at, pid[i].bt, pid[i].priority, pid[i].ct, pid[i].tat, pid[i].wt, pid[i].resp_time);
        printf("---------------------------------------------------------------------------------------------------------------------------------------\n");
    }
    //generate gant chart
    //reset remaiining time
    for(int i = 0; i < p; i++){
        pid[i].remaining_bt = pid[i].bt;
    }
    printf("-------- gant chart --------\n\n");
    completed = 0;
    current = 0;
    time = 0;
    curr_quantum = q;
    while(completed < p){
        if(pid[current].at > time){
            printf("--------------%d\n",time);
            time++;
        }
        else{
            if(pid[current].remaining_bt > 0){
                while(q != 0){
                    printf("--------------%d\n",time);
                    printf("process %d\n", pid[current].pid);
                    time++;
                    pid[current].remaining_bt--;
                    curr_quantum--;
                    if(pid[current].remaining_bt == 0){
                        //calculate time
                        completed++;
                        curr_quantum = q;
                        current = (current + 1)%p;
                        break;
                    }
                    if(curr_quantum == 0){
                        curr_quantum = q;
                        current = (current + 1)%p;
                        break;
                    }
                }
            }
            else if(pid[current].remaining_bt == 0){
                current = (current + 1)%p;
            }
        }
    }
    printf("--------------%d\n\n",time);
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
    float av_wait = tot_wait/r;
    float av_tat = tot_tat/r;
    float av_resp = tot_resp/r;
    printf("Average Wait time: %f\n", av_wait);
    printf("Average Turn Around Time: %f\n", av_tat);
    printf("Average Response Time: %f\n", av_resp);
    printf("Throughput: %d/%d\n", p, actual_time);
}

int main(){
    float amount;
    int quantum;
    bool valid = false;
    while(valid == false){
        printf("Enter amount of processes (maximum of 50): ");
        scanf("%f", &amount);
        if(amount > 0 && amount <= 50){valid = true;}
        else{printf("incorrect input, try again");}
        
    }
    valid = false;
    while(valid == false){
        printf("Enter quantum amount: ");
        scanf("%d", &quantum);
        if(quantum > 0 && quantum <= 50){valid = true;}
        else{printf("invalid quantum enter again \n");}
    }
    printf("\n");
    round_robin(amount, quantum);
    return 0;
}
