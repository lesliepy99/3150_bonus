#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
void substract(LinkedQueue*);
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

    int pin = 0;
    int tmp_time = 0;
    int start_time = 0;
    int queue_index = 0;
    int first_arrival=1;
    Process de_proc;
    LinkedQueue* current_queue = (LinkedQueue*)malloc(sizeof(LinkedQueue));

    do{
       printf("Start time:%d",start_time);
       //Enqueue the new_coming processes
       for(int i=0;i<proc_num;i++){
           if (tmp_time == proc[i].arrival_time){
		   //Decide the start time of the first arrival
		   if(first_arrival==1){
		       	start_time = tmp_time;
		        first_arrival = 0;}
		proc[i].turnaround_time = ProcessQueue[queue_num-1]->allotment_time;
		proc[i].completion_time = ProcessQueue[queue_num-1]->time_slice;
                ProcessQueue[queue_num-1] = EnQueue(ProcessQueue[queue_num-1], proc[i]);
                printf("Arrival, tmp=%d, pid:%d, allotment_time:%d,time_slice:%d\n",tmp_time,proc[i].process_id,proc[i].turnaround_time,proc[i].completion_time);
           }
       }
/*
       // Handle the re-order when reaches period
       if(tmp%period==0){
           pin = 0;//No process is pined then

       }
*/
       //decide the first approchable non_empty queue
       if(pin==0){
	     current_queue = return_non_empty(ProcessQueue,&queue_index,queue_num);
	     printf("queue_index:%d\n",queue_index);
	     //printf("NOrmal or not\n");
	     if(!IsEmptyQueue(current_queue)){
	         pin = 1;
	      // start_time = tmp_time;
	       }
	     //printf("Start time:%d\n",tmp_time);
    }
      // printf("IS currentqueue empty:%d, queue_index:%d,tmp:%d\n",IsEmptyQueue(current_queue),queue_index,tmp_time);
   if(!IsEmptyQueue(current_queue)){
       //printf("Before substraction\n");
       Process current_proc = FrontQueue(current_queue);
       if(current_proc.arrival_time!=tmp_time)  substract(current_queue);
       current_proc = FrontQueue(current_queue);
       //printf("After substraction: pid=%d\n",current_proc.process_id);

       printf("Execution time:%d, tmp:%d\n",current_proc.execution_time,tmp_time);
       if(current_proc.execution_time==0){
	   printf("Finished?tmp:%d\n",tmp_time);
           pin = 0;
           de_proc = DeQueue(current_queue);
	   current_proc = FrontQueue(current_queue);

        //   printf("The result of dequeue: process_id:%d\n",de_proc.process_id);
	   printf("The first output\n");
	   outprint(start_time,tmp_time,de_proc.process_id,de_proc.arrival_time,de_proc.execution_time);
	   printf("Assign first start time:%d\n",start_time);
	   start_time = tmp_time;
	   printf("Assign first start time:%d\n",start_time);
       }       
       
       //Use turnaround_time to represent allotment_time
        if(current_proc.turnaround_time==0){
           pin = 0;
	   printf("alloement time used up, is current queue empty:%d\n",IsEmpty(current_queue));
	   if(!IsEmptyQueue(current_queue)){
           de_proc = DeQueue(current_queue);
	   de_proc.turnaround_time = ProcessQueue[queue_index-1]->allotment_time;
	   de_proc.completion_time = ProcessQueue[queue_index-1]->time_slice;

	   EnQueue(ProcessQueue[queue_index-1],de_proc);
	   printf("THe second output\n");
           printf("Assign second start time:%d\n",start_time);
           outprint(start_time,tmp_time,de_proc.process_id,de_proc.arrival_time,de_proc.execution_time);

	   start_time = tmp_time;
           printf("Assign second start time:%d\n",start_time);
	   }
       }
       

	//Use comppletion_time to represent time_slice
	else if(current_proc.completion_time==0){
	   printf("PId:%d, slice_time:%d\n",current_proc.process_id,current_proc.completion_time);
           pin = 0;
           de_proc = DeQueue(current_queue);
	   de_proc.completion_time = current_queue->time_slice;
           EnQueue(current_queue,de_proc);
	   printf("THe third output\n");
	   printf("Assign third start time:%d\n",start_time);
           outprint(start_time,tmp_time,de_proc.process_id,de_proc.arrival_time,de_proc.execution_time);
       
       	   start_time = tmp_time;
	   printf("Assign third start time:%d\n",start_time);

       }
   }
	if(tmp_time == 400){
            break;
	}
	tmp_time+=1;
    }
    while(tmp_time);

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
      
		
void substract(LinkedQueue* LQueue){
    if (IsEmpty(LQueue)){
        printf("Error! Empty!");
    }else{
        LinkedQueue* pt = LQueue;
        while (pt->next){
	//	printf("Loop\n");
	//	printf("pid:%d\n",pt->next->proc.process_id);
            if (pt->next->next == NULL){
		pt->next->proc.execution_time-=1;	
		pt->next->proc.turnaround_time-=1;
		pt->next->proc.completion_time-=1;
                break;
            }
	    pt = pt->next;
        }
    }
}

     

