#include<stdio.h>
#include<String.h>
#include<stdlib.h>
#include <math.h>

typedef char string[50];

struct process{
	int a;
	int b;
	int c;
	int StartTime;
	int WaitTime;
	int EndTime;
	int RemainingTime;
	int started;
	int hasValue;
	int numProcess;
	int Start[50];
	int End[50];
	int Wait[50];
	int numWait;
	int IsInQ;
};

float getAverageWaitTime(struct process *p, int length){
	
	float average = 0.0;
	int i;
	
	
	for(i = 0; i < length; i++){
		average = average + p[i].WaitTime;
	}
	
	return average / length;
}

void printResults(struct process *p, int y){
	int i, k;
	int result = -1;
	int amount, AProcess, AWait;
	
	struct process temp;
	
	for(i = 0; i < y - 1; i++){
		for(k = 0; k < (y - 1 - i); k++){
			if(p[k].a > p[k + 1].a){
				temp = p[k];
				p[k] = p[k + 1];
				p[k + 1] = temp;
			}
		}
	}
	
	for(amount = 0; amount < y; amount++){
		
		printf("p[%d]", p[amount].a);
		for(AProcess = 0; AProcess < p[amount].numProcess; AProcess++){
			printf(" Start Time: %d End Time: %d |"
			, p[amount].Start[AProcess], p[amount].End[AProcess]);
		}
		
		for(AWait = 0; AWait < p[amount].numWait; AWait++){
				printf(" Waiting Time: %d \n"
				,p[amount].Wait[AWait]);
		}
	}

	printf("Average wait time: %.1f\n", getAverageWaitTime(p, y));
}

int add(struct process *p, struct process *RQ, int time, int length, int RQlength){
	
	int i;
	
	for(i = 0; i < length; i++){
		if(p[i].b <= time && p[i].RemainingTime > 0 
		&& p[i].IsInQ == 0){
			p[i].IsInQ = 1;
			RQ[RQlength] = p[i];
			RQlength++;
		}
	}
	
	
	return RQlength;
}

int delete(int index, struct process *RQ, int RQlength){
	
	int i;
	
	for (i = index - 1; i < RQlength - 1; i++)  
    {  
		RQ[i] = RQ[i+1];   
    }  
    
    RQlength = RQlength - 1;
	return RQlength;
}

int swap(struct process *RQ, int RQlength){
	
	int i;
	struct process first = RQ[0];
	
	for (i = 0; i < RQlength - 1; i++)  
    {  
		RQ[i] = RQ[i+1];   
    }  
    
    RQ[RQlength - 1] = first;
	
	return RQlength;
}

int addToComplete(struct process *complete, struct process queue, int completed){
	
	complete[completed] = queue;
	completed++;
	
	return completed;
}

void CompletedProcess(struct process *Queue, int time, int current){
	
	Queue[current].EndTime = time;
	Queue[current].WaitTime = Queue[current].EndTime - (Queue[current].b + Queue[current].c);
	Queue[current].RemainingTime = 0; 
				
	Queue[current].End[Queue[current].numProcess] = Queue[current].EndTime;
	Queue[current].numProcess = Queue[current].numProcess + 1;
				
	Queue[current].Wait[Queue[current].numWait] = Queue[current].WaitTime;
	Queue[current].numWait = Queue[current].numWait + 1;
		
}

void InterruptedProcess(struct process *Queue, int time, int current){
	
	Queue[current].EndTime = time;			
	Queue[current].End[Queue[current].numProcess] = Queue[current].EndTime;
	Queue[current].numProcess = Queue[current].numProcess + 1;
}

void RR(struct process *p, int y, int z){
	int time = 0, completed = 0;
	int current = 0;
	int i, k, j, r;
	struct process temp;
	
	struct process RQ[y];
	int RQlength = 0;
	
	struct process CompletedQ[y];
	int CompletedQs = 0;
	
	
	for(i = 0; i < y - 1; i++){
		for(k = 0; k < (y - 1 - i); k++){
			if(p[k].b > p[k + 1].b){
				temp = p[k];
				p[k] = p[k + 1];
				p[k + 1] = temp;
			}
		}
	}
	
	while(completed != y){
		if(time == 0){
			RQlength = add(p, RQ, time, y, RQlength);
			RQ[current].StartTime = time;
			
			RQ[current].Start[RQ[current].numProcess] = RQ[current].StartTime;
			if(RQ[current].RemainingTime <= z){
				time = time + RQ[current].RemainingTime;
				CompletedProcess(RQ, time, current);
				
				CompletedQs = addToComplete(CompletedQ, RQ[current], CompletedQs);
				RQlength = add(p, RQ, time, y, RQlength);
				RQlength = delete(current + 1, RQ, RQlength);
				
				completed++;
				
			}
			else{
				RQ[current].RemainingTime = RQ[current].RemainingTime - z;
				time = time + z;
				
				InterruptedProcess(RQ, time, current);
				
				RQlength = add(p, RQ, time, y, RQlength);
				RQlength = swap(RQ, RQlength);
			}
			
		}
		else if(time > 0){
			RQ[current].StartTime = time;
			
			RQ[current].Start[RQ[current].numProcess] = RQ[current].StartTime;
			RQlength = add(p, RQ, time, y, RQlength);
			if(RQ[current].RemainingTime <= z){
				time = time + RQ[current].RemainingTime;
				CompletedProcess(RQ, time, current);
				
				CompletedQs = addToComplete(CompletedQ, RQ[current], CompletedQs);
				RQlength = add(p, RQ, time, y, RQlength);
				RQlength = delete(current + 1, RQ, RQlength);
				
				completed++;
				
			}
			else{
				RQ[current].RemainingTime = RQ[current].RemainingTime - z;
				time = time + z;
				InterruptedProcess(RQ, time, current);
				RQlength = add(p, RQ, time, y, RQlength);
				RQlength = swap(RQ, RQlength);
			}
		}
		
	}
	
	printResults(CompletedQ, CompletedQs);
}

int getShortestRemaining(int time, struct process *shortest, int minm, int length){
	int i;
	int result = -1;
	
	
	
	for(i = 0; i < length; i++){
		if(shortest[i].b <= time && shortest[i].RemainingTime < minm 
		&& shortest[i].RemainingTime > 0){
			minm = shortest[i].RemainingTime;
			result = i; 
			
		}
		
		
	}
	
	return result;
}


void SRTF(struct process *p, int y){
	int j, i, k;
	int current;
	int time = 0, shortest = 0;
	struct process queue;
	int minm = INT_MAX;
	int started = 0;
	int completed = 0;
	int amount, AProcess, AWait;
	
	struct process temp;
	
	for(i = 0; i < y - 1; i++){
		for(k = 0; k < (y - 1 - i); k++){
			if(p[k].b > p[k + 1].b){
				temp = p[k];
				p[k] = p[k + 1];
				p[k + 1] = temp;
			}
		}
	}
	
	
	while(completed != y){
		if(time == 0){
			current = 0;
			minm = p[current].RemainingTime;
			p[current].StartTime = time;
		}
		
		else if (time > 0 && p[current].RemainingTime != 0){
			if(getShortestRemaining(time, p, minm, y) != -1){
				if(p[current].a != p[getShortestRemaining(time, p, minm, y)].a){
					p[current].Start[p[current].numProcess] = p[current].StartTime;
					
					InterruptedProcess(p, time, current);
					
					
					current = getShortestRemaining(time, p, minm, y);
					minm = p[current].RemainingTime;
					p[current].StartTime = time;
				}	
			}
		}
		
		if(p[current].RemainingTime == 0){
			p[current].Start[p[current].numProcess] = p[current].StartTime;
			
			CompletedProcess(p, time, current);
			
			
			completed++;
			
			current = getShortestRemaining(time, p, INT_MAX, y);
			minm = p[current].RemainingTime;
			p[current].StartTime = time;
			
		}
		
		p[current].RemainingTime = p[current].RemainingTime - 1;
		
		time++;
	}
	
	printResults(p, y);
}




void SJF(struct process *p, int y){
	
	int j, i, k;
	int current;
	int time = 0, shortest = 0;
	struct process queue;
	int minm = INT_MAX;
	int started = 0;
	int completed = 0;
	
	
	while(completed != y){
		if(time == 0){
			current = getShortestRemaining(time, p, INT_MAX, y);
			minm = p[current].RemainingTime;
			p[current].StartTime = time;
		}
		
		
		if(p[current].RemainingTime == 0){
			p[current].Start[p[current].numProcess] = p[current].StartTime;
			CompletedProcess(p, time, current);
			
			
			completed++;
			
			current = getShortestRemaining(time, p, INT_MAX, y);
			minm = p[current].RemainingTime;
			p[current].StartTime = time;
		}
		
		p[current].RemainingTime = p[current].RemainingTime - 1;
		
		time++;
	}

	printResults(p, y);
}
	 
void FCFS(struct process *p, int y){
	
	int j, i, k, key;
	struct process temp;
	int Average = 0;
	
	for(i = 0; i < y - 1; i++){
		for(k = 0; k < (y - 1 - i); k++){
			if(p[k].b > p[k + 1].b){
				temp = p[k];
				p[k] = p[k + 1];
				p[k + 1] = temp;
			}
		}
	}
	
	for(j = 0; j < y; j++){
		
		
		if(j == 0){
			p[j].StartTime = p[j].b;
		}
		
		else if(j > 0){
			p[j].StartTime = p[j - 1].EndTime;	
		}
		
		p[j].WaitTime = p[j].StartTime - p[j].b;
		p[j].EndTime = p[j].StartTime + p[j].c;
		
		p[j].Start[p[j].numProcess] = p[j].StartTime;
		p[j].End[p[j].numProcess] = p[j].EndTime;
		p[j].numProcess = p[j].numProcess + 1;
			
		p[j].Wait[p[j].numWait] = p[j].WaitTime;
		p[j].numWait = p[j].numWait + 1;
		
		
	}
	
	printResults(p, y);
	
}

int main()
{
	int x, y, z, i;
	struct process *p;
	
	string fileName;
	FILE *pFile;
	
	printf ("Enter filename to load: ");
   	fgets(fileName,sizeof(string),stdin);
   	fileName[strlen(fileName)-1] = '\0';
	pFile = fopen(fileName, "rt");
	
	if(pFile == NULL){
		printf("%s  not  found", fileName);
		return 0;
	}
	else{
		fscanf(pFile, "%d", &x);
		fscanf(pFile, "%d", &y);
		fscanf(pFile, "%d", &z);
		
		p = malloc(y * sizeof(struct process));
	
		for(i = 0; i < y; i++){
			if(!fscanf(pFile, "%d", &p[i].a))
   				break;
   			if(!fscanf(pFile, "%d", &p[i].b))
   				break;
   			if(!fscanf(pFile, "%d", &p[i].c))
   				break;
   			p[i].StartTime = 0;	
   			p[i].WaitTime = 0;
   			p[i].EndTime = 0;
   			p[i].RemainingTime = p[i].c;
   			p[i].started = 0;
   			p[i].numProcess = 0;
   			p[i].numWait = 0;
   			p[i].IsInQ = 0;
		}
	
		fclose(pFile);	
	}
	
	
	if(x == 0){
		FCFS(p, y);
	}
	else if(x == 1){
		SJF(p, y);
	}
	else if(x == 2){
		SRTF(p, y);
	}
	else if(x == 3){
		RR(p, y, z);
	}	
	
}
