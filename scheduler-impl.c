#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
void substract(Process*);
void outprint(int time_x, int time_y, int pid, int arrival_time, int remaining_time);
LinkedQueue* return_non_empty(LinkedQueue**,int*,int);
void scheduler(Process* proc, LinkedQueue** ProcessQueue, int proc_num, int queue_num, int period){
    printf("Process number: %d\n", proc_num);
    for (int i = 0;i < proc_num; i++)
        printf("%d %d %d\n", proc[i].process_id, proc[i].arrival_time, proc[i].execution_time);

    printf("\nQueue number: %d\n", queue_num);
    printf("Period: %d\n", period);
    for (int i = 0;i < queue_num; i++){
        printf("%d %d %d\n", i, ProcessQueue[i]->time_slice, ProcessQueue[i]->allotment_time);
    }

    /*
       Test outprint function, it will output "Time_slot:1-2, pid:3, arrival-time:4, remaining_time:5" to output.loc file.
    */
    outprint(1,2,3,4,5);

    int pin = 0;
    int tmp_time = 0;
    int start_time = 0;
    int queue_index = 0;
    Process de_proc;
    LinkedQueue* current_queue = (LinkedQueue*)malloc(sizeof(LinkedQueue));

    do{
       //Enqueue the new_coming processes
       for(int i=0;i<proc_num;i++){
           if (tmp_time == proc[i].arrival_time){
		proc[i].turnaround_time = ProcessQueue[queue_num-1]->allotment_time;
		proc[i].completion_time = ProcessQueue[queue_num-1]->time_slice;
                ProcessQueue[queue_num-1] = EnQueue(ProcessQueue[queue_num-1], proc[i]);
                printf("Arrival, tmp=%d, pid:%d, allotment_time:%d,time_slice:%d\n",tmp_time,proc[i].process_id,proc[i].turnaround_time,proc[i].completion_time);
           }
       }
       //decide the first approchable non_empty queue
       if(pin==0){
	     current_queue = return_non_empty(ProcessQueue,&queue_index,queue_num);
	     printf("queue_index:%d\n",queue_index);
	     //printf("NOrmal or not\n");
	     if(!IsEmptyQueue(current_queue))
	         pin = 1;
	     start_time = tmp_time;
	     //printf("Start time:%d\n",tmp_time);
    }
   if(!IsEmptyQueue(current_queue)){
       Process current_proc = FrontQueue(current_queue);
       substract(&(FrontQueue(current_queue)));
       printf("Execution time:%d, tmp:%d\n",current_proc.execution_time,tmp_time);
       if(current_proc.execution_time==0){
	   printf("Finished?tmp:%d\n",tmp_time);
           pin = 0;
           de_proc = DeQueue(current_queue);
	   outprint(start_time,tmp_time,de_proc.process_id,de_proc.arrival_time,de_proc.execution_time);
	   start_time = tmp_time;
       }       
       
       //Use turnaround_time to represent allotment_time
        if(current_proc.turnaround_time==0){
           pin = 0;
           de_proc = DeQueue(current_queue);
	   de_proc.turnaround_time = ProcessQueue[queue_index-1]->allotment_time;
	   de_proc.completion_time = ProcessQueue[queue_index-1]->time_slice;
	   EnQueue(ProcessQueue[queue_index-1],de_proc);
           outprint(start_time,tmp_time,de_proc.process_id,de_proc.arrival_time,de_proc.execution_time);
           start_time = tmp_time;
       }
       

	//Use comppletion_time to represent time_slice
	if(current_proc.completion_time==0){
           pin = 0;
           de_proc = DeQueue(current_queue);
	   de_proc.completion_time = current_queue->time_slice;
           EnQueue(current_queue,de_proc);
           outprint(start_time,tmp_time,de_proc.process_id,de_proc.arrival_time,de_proc.execution_time);
           start_time = tmp_time;
       }
   }
	if(tmp_time == 200){
            break;
	}
    }
    while(++tmp_time);

}
LinkedQueue* return_non_empty(LinkedQueue** original_queue,int* queue_number,int queue_num){
     for (int i=queue_num-1;i>=0;i--){
         if(!IsEmptyQueue(original_queue[i])){
	     *queue_number = i;
	     return original_queue[i];
	 }
     }
     return original_queue[0];
}     
      
		
void substract(Process* proc){
    proc->execution_time-=1;
    proc->turnaround_time-=1;
    proc->completion_time-=1;
    printf("SUbstract pid:%d\n",proc->process_id);
}
     

