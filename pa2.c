//Benjamin Cathelineau OS programing assignment 2

#include <stdio.h>
#include <gsl/gsl_rng.h>//random
#include <gsl/gsl_randist.h>//random
#include <time.h>
#include <sys/time.h>
#include <math.h>//round
#include <stdlib.h>

#define FIFO 0
#define SJF 1
#define SRT 2


//simulation counter
int simulation_time=0;
int average_gauss; //need to be accessible by the result function
//structure of a process
struct process{
  int id;
  int active;
  int arrival_time;
  int total_cpu_time;
  int remaining_cpu_time;
  int turnaround_time;
};

//Functions

void print_processes(struct process process_table[],int number_of_process){
  //write function for better print
  printf("id    | active | arrival_time | total_cpu_time | remaining_cpu_time | turnaround_time\n");
  for(int i = 0; i < number_of_process; i = i + 1){
    printf("%5d | %6d | %12d | %14d | %18d | %8d \n", process_table[i].id ,process_table[i].active,process_table[i].arrival_time, process_table[i].total_cpu_time, process_table[i].remaining_cpu_time,process_table[i].turnaround_time);
  }
}

void result(struct process process_table[], int number_of_process){//calculation of the mean of the turnaround_time
  double mean = 0;
  for(int i = 0;i<number_of_process ;i = i +1){
    mean = mean + process_table[i].turnaround_time;
  }
  mean = mean / number_of_process;
  print_processes(process_table, number_of_process);
  printf("Simulation took : %d unit time\n", simulation_time);
  printf("The average turnaround_time is : %lf\n", mean);
  printf("d / ATT : %lf \n",average_gauss / mean);

}
void swap(struct process *p1, struct process  * p2){//used by bubble sort
  struct process temp= *p1;
  *p1=*p2;
  *p2=temp;
}

void sort(struct process process_table[], int number_of_process, int sorting_type){//implementation of the bubble sort
  int i, j;
  switch(sorting_type){
    case FIFO://DONE
    for (i = 0; i < number_of_process-1; i++){
      for (j = 0; j < number_of_process-i-1; j++){
        if (process_table[j].arrival_time > process_table[j+1].arrival_time){
          swap(&process_table[j],&process_table[j+1]);
        }
      }
    }
    break;
    case SJF:
    //wrong
    for (i = 0; i < number_of_process-1; i++){
      for (j = 0; j < number_of_process-i-1; j++){
        if (process_table[j].total_cpu_time > process_table[j+1].total_cpu_time ){
          swap(&process_table[j],&process_table[j+1]);
        }
      }
    }
    break;
    case SRT:
    for (i = 0; i < number_of_process-1; i++){
      for (j = 0; j < number_of_process-i-1; j++){
        if (process_table[j].remaining_cpu_time > process_table[j+1].remaining_cpu_time){
          swap(&process_table[j],&process_table[j+1]);
        }
      }
    }
    break;
    default:
    printf("Error : Wrong scheduler number\n");
    break;
  }
}

int find_active(struct process process_table[], struct process active_process[],int number_of_process, int *finished){
  int active_process_count= 0;
  //to count the number of process that are active
  for(int i = 0; i<number_of_process;i = i + 1){//search the process list to see if all are finished
    if(process_table[i].active==0){
      *finished= *finished +1 ;
    }else{//if proccess is still active, add i to the current list of active process
      active_process[active_process_count]=process_table[i];
      active_process_count++;
    }
  }
  return active_process_count ;
}
int find_arrived(struct process active_process[], struct process arrived_process[], int active_process_count){
  int arrived_process_count=0;
  for(int i = 0; i<active_process_count;i = i + 1){//search the list of process for newly arrived processes and append them to a list
    if(simulation_time  >= active_process[i].arrival_time){//if another process arrived and is active
      arrived_process[arrived_process_count]= active_process[i];
      arrived_process_count++;
    }
  }
  return arrived_process_count;
}

//scheduling algorithms
//
void fifo(struct process process_table[], int number_of_process){
  sort(process_table,number_of_process, FIFO);
  //proccess are now sorted by arrival time
  for(int i = 0; i<number_of_process;i = i + 1){//executing the
    while(process_table[i].arrival_time > simulation_time){//wait for the process to arrive
      simulation_time ++;
    }
    //run the process
    while (process_table[i].remaining_cpu_time > 0){
      process_table[i].remaining_cpu_time--;
      simulation_time++;
    }
    printf("process id %d executed\n", process_table[i].id);//to show the order of execution
    process_table[i].active=0;
    process_table[i].turnaround_time = simulation_time -process_table[i].arrival_time;
  }
  result(process_table, number_of_process);
}
void sjf(struct process process_table[], int number_of_process){//this was very hard
  int finished =0;
  while(finished < number_of_process){
    struct process active_process[number_of_process];
    finished = 0;
    int active_process_count;
    active_process_count =find_active( process_table, active_process, number_of_process, &finished);
    if(finished>= number_of_process){//last iteration get out of the loop
      break;
    }
    sort(active_process,active_process_count, FIFO);//sort the process by arrival time amongs the active one only
    if (active_process[0].arrival_time > simulation_time){//waiting for the processes to arrive
      simulation_time++;
    }else{
      struct process arrived_process[active_process_count];
      int arrived_process_count= 0;
      arrived_process_count=find_arrived(active_process,arrived_process, active_process_count);
      sort(arrived_process,arrived_process_count, SJF);//sort the arrived and active process by shortest job
      int i = 0;
      for(;i<number_of_process; i = i + 1 ){//search the process to execute inside the original array of process
        if(process_table[i].id == arrived_process[0].id){
          break;
        }
      }
      while( arrived_process[0].remaining_cpu_time > 0){//execute the first process on the list (the shortest one)
        arrived_process[0].remaining_cpu_time--;
        process_table[i].remaining_cpu_time--;
        simulation_time++;
      }
      printf("process id %d executed\n", arrived_process[0].id);//to show the order of execution
      process_table[i].active=0;
      process_table[i].turnaround_time = simulation_time -process_table[i].arrival_time;
    }
  }
  result(process_table,number_of_process);
}

void srt(struct process process_table[], int number_of_process){
  int finished =0;
  struct process active_process[number_of_process];
  struct process arrived_process[number_of_process];
  int active_process_count;
  int arrived_process_count;
  while(1){
    finished = 0;
    active_process_count =find_active( process_table, active_process, number_of_process, &finished);
    if(finished>= number_of_process){//last iteration get out of the loop
      break;
    }
    sort(active_process,active_process_count, FIFO);//
    while (active_process[0].arrival_time > simulation_time){//waiting for the processes to arrive
      simulation_time++;
    }
    arrived_process_count=find_arrived(active_process,arrived_process, active_process_count);
    sort(arrived_process,arrived_process_count, SRT);//sort the arrived process by remaining_cpu_times
    int i = 0;
    for( ;i<number_of_process; i = i + 1 ){//search the process to execute inside the original array of process
      if(process_table[i].id == arrived_process[0].id){
        break;
      }
    }
    simulation_time++;
    process_table[i].remaining_cpu_time--;
    printf("process number %d executing \n",arrived_process[0].id);
    if(process_table[i].remaining_cpu_time ==0){
      printf("process id %d executed\n", arrived_process[0].id);//to show the order of execution
      process_table[i].active=0;
      process_table[i].turnaround_time = simulation_time -process_table[i].arrival_time;
    }
  }
  result(process_table,number_of_process);

}



int main(int argc, char *argv[]){
  if(argc < 6){
    printf("Usage with 5 arguments, first number of processes, second arrival time limit, third gaussian mean, fourth standart deviation, fifth scheduling algoritms\n");
    exit(128);
  }
  //parsing all the argument
  int number_of_process=atoi(argv[1]);
  int arrival_time_limit=atoi(argv[2]);//is k
  average_gauss=atoi(argv[3]);
  int standart_deviation=atoi(argv[4]);
  int selected_scheduler=atoi(argv[5]);

  //to use gsl
  struct timeval time;
  gettimeofday(&time,NULL); // changes more than time(0)
  srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
  const gsl_rng_type * T;
  gsl_rng * r;
  gsl_rng_env_setup();
  T = gsl_rng_default;
  r = gsl_rng_alloc (T);
  gsl_rng_set(r, (time.tv_sec * 1000) + (time.tv_usec / 1000));//to reach true randomness you need the power of time


  struct process process_table[number_of_process];//Create the array of
  for(int i = 0; i < number_of_process; i = i + 1){//initialize all process
    process_table[i].id=i;
    process_table[i].active = 1;
    process_table[i].arrival_time=(rand()%(arrival_time_limit-0 +1));//random between 0 and arrival time limit
    process_table[i].total_cpu_time=abs(average_gauss+(round(gsl_ran_gaussian(r, standart_deviation))));//rounded absolute value
    if(process_table[i].total_cpu_time == 0 ){//if it's 0 add 1
    process_table[i].total_cpu_time = process_table[i].total_cpu_time +1;
  }
  process_table[i].remaining_cpu_time=process_table[i].total_cpu_time;
  process_table[i].turnaround_time=0;
}

//start scheduling
switch(selected_scheduler){
  case FIFO:
  printf("FIFO scheduler selected\n");
  printf ("Number of process %d\n",number_of_process);
  printf("printing the order of execution of process to see that it matches FIFO : \n");
  fifo(process_table,number_of_process);//calling the fifo scheduler
  break;
  case SJF:
  printf("SJF scheduler selected\n");
  printf ("Number of process %d\n",number_of_process);
  printf("printing the order of execution of process to see that it matches SJF : \n");
  sjf(process_table,number_of_process);//calling the SJF scheduler
  break;
  case SRT:
  printf("SRT scheduler selected\n");
  printf ("Number of process %d\n",number_of_process);
  printf("printing the order of execution of process to see that it matches SRT : \n");
  srt(process_table,number_of_process);//calling the SRT scheduler
  break;
  default:
  printf("Error : Wrong scheduler number\n");
  break;
}

return 0;
}
