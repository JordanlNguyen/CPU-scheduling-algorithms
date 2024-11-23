#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void sort(int arr[][9], int num_rows){
    for(int i = 0; i < num_rows - 1; i++){
        for(int j = i + 1; j < num_rows; j++){
            if(arr[i][1] > arr[j][1]){
                for(int k = 0; k < 4; k++){
                    int temp = arr[i][k];
                    arr[i][k] = arr[j][k];
                    arr[j][k] = temp;
                }
            }
        }
    }
}

void fcfs(float q){
    int p = q;
    //generate array for pid
    int pid[p][9]; // 0 - pid; 1 - AT; 2 - BT; 3 - Priority; 4 - completion time; 5 - turn around; 6 - wait time;
    //7 - response time  8- first time getting cpu
    pid[0][0] = 0; pid[0][1] = rand()%100; pid[0][2] = rand()%10; pid[0][3] = rand()%100;
    int max_at = pid[0][1]; int min_at = pid[0][1]; 
    int max_bt = pid[0][2]; int min_bt = pid[0][2];
    int min_pr = pid[0][3]; int max_pr = pid [0][3];
    for(int i = 1; i < p; i++){
        pid[i][0] = i;
        pid[i][1] = rand()%100;
        if(pid[i][1] > max_at){max_at = pid[i][1];}
        if(pid[i][1] < min_at){min_at = pid[i][1];}
        pid[i][2] = 1 + rand()%10;
        if(pid[i][2] > max_bt){max_bt = pid[i][2];}
        if(pid[i][2] < min_bt){min_bt = pid[i][2];}
        pid[i][3] = rand()%100;
        if(pid[i][3] > max_pr){max_pr = pid[i][3];}
        if(pid[i][3] < min_pr){min_pr = pid[i][3];}
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
        printf("%-15d %-15d %-15d %-15d\n", pid[i][0], pid[i][1], pid[i][2], pid[i][3]);
        printf("----------------------------------------------------------\n\n");
    }
    // //calculate fcfs queue by AT
    sort(pid, p);
    //calculate and store completion time, response time and first time getting cpu
    int time = 0;
    int cur_bt;
    for(int i = 0; i < p; i++){
        if(pid[i][1] <= time){
            pid[i][8] = time;
            cur_bt = pid[i][2];
            while(cur_bt > 0){
                time = time + 1;
                cur_bt = cur_bt - 1;
            }
            pid[i][4] = time;
            pid[i][7] = pid[i][8] - pid[i][1];
        }
        else if(pid[i][1] > time){
            while(pid[i][1] > time){
                time = time + 1;
            }
            cur_bt = pid[i][2];
            pid[i][8] = time;
            while(cur_bt > 0){
                time = time + 1;
                cur_bt = cur_bt - 1;
            }
            pid[i][4] = time;
            pid[i][7] = pid[i][8] - pid[i][1];
        }
    }
    //calculate and store turn around time
    for(int i = 0; i < p; i++){
        pid[i][5] = pid[i][4] - pid[i][1];
    }
    //calculate and store waiting time
    for(int i = 0; i < p; i++){
        pid[i][6] = pid[i][5] - pid[i][2];
    }
    //display complete table
    printf("------------------------------------------------------------ complete table ------------------------------------------------------------\n\n");
    printf("%-15s %-15s %-15s %-15s %-20s %-20s %-15s", "proccess id", "arrival time", "burst time" ,"priority", "complettion time",
     "turn around time", "wait time");
    printf("%s\n\n", "response time");
    for(int i = 0; i < p; i++){
        printf("%-15d %-15d %-15d %-15d %-20d %-20d %-15d", pid[i][0], pid[i][1], pid[i][2], pid[i][3], pid[i][4],pid[i][5], pid[i][6]);
        printf("%d\n", pid[i][7]);
        printf("---------------------------------------------------------------------------------------------------------------------------------------\n\n");
    }
    //averages and through put
    printf("-------- averages and throughput --------\n\n");
    //average wait time
    float tot_wait = 0;
    float tot_tat = 0;
    float tot_rt= 0;
    for(int i = 0; i<p; i++){
        tot_wait += pid[i][6];
        tot_tat += pid[i][5];
        tot_rt += pid[i][7];
    }
    float ave_wt = tot_wait/q;
    float ave_tat = tot_tat/q;
    float ave_rt = tot_rt/q;
    printf("Average Wait Time: %f\n", ave_wt);
    printf("Average Turn Around Time: %f\n", ave_tat);
    printf("Average Response Time: %f\n", ave_rt);
    //throughput
    printf("Through put: %d/%d\n\n", p, pid[p -1][4]);
    //generate gant chart
    printf("-------- gant chart --------\n\n");
    printf("%-13s %-4s\n\n", "process", "time");
    time = 0;
    if(pid[0][1] > 0){
        printf("--------------%d\n",0);
    }
    for(int i = 0; i < p; i++){
        if(pid[i][1] > time){
            printf("--------------%d\n", pid[i][1]);//arrival time
            printf("process %d\n", pid[i][0]);
            time = pid[i][4];
            printf("--------------%d\n", time);//completion time
        }
        else{
            printf("process %d\n", pid[i][0]);
            printf("--------------%d\n", pid[i][4]);
            time = pid[i][4];
        }
    }

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
    fcfs(amount);
    return 0;
}
