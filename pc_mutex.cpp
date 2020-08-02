#include<stdio.h>      
#include<stdlib.h>       
#include<pthread.h>       
#include<time.h>           
#include<semaphore.h>       

#define MAX_BUF 32 //buffer의 크기는 32로 설정     
#define MAX_LOOP 32 //loop도 32번 수행          

int buffer[MAX_BUF];                                
int counter=0;                                       
int in=-1,out=-1;                              

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;       
sem_t full,empty;                                     

void *producer(void *arg) {

	int i, j;                                                                                                                                                     unsigned long data;                                                                                                                                                                                                                                                                                                       for (i = 0; i < MAX_LOOP; i++) {
		srand(time(NULL));  
		for (j = 0; j < rand() % 0xFFFFFFFF; j++);   
		data = (unsigned long)j; //buffer에 할당할 data 생성  
		//pthread_mutex_lock(&mutex);    

		sem_wait(&full); //full의 신호가 올때까지 대기, 자원을 하나 주는 것         
		pthread_mutex_lock(&mutex); //mutex lock                                   
		
		/////////////////Critical Section///////////////         

		in++; in%=MAX_BUF; //produce++                                                      
		buffer[in]=(int)data; //buffer에 data를 집어넣음으로써 produce                      
		counter++;//공유변수인 counter에 1 더함으로써 생산했음을 표시.     

		printf("producer - in: %d, counter: %d, data: %ld\n",in,counter,data);    

		////////////////Critical Section//////////////////                  

		pthread_mutex_unlock(&mutex); //mutex unlock                          
		sem_post(&empty); //empty에 시그널을 보내 대기 상태에서 깨어나게 함, 자원을 하나 반납받는다.    

	}                                                                                                  
}     
void *consumer(void *arg) {

	int i, j;                                                                                                                                                     unsigned long data;                                                                                                                                                                                                                                                                                                       for (i = 0; i < MAX_LOOP; i++) {
		sem_wait(&empty); //empty의 신호가 올때까지 대기, 자원을 하나 주는 것.
		pthread_mutex_lock(&mutex); //mutex lock
									
		///////////////////Critical Section/////////////////////////

		out++; out%=MAX_BUF; //comsume++               
		data=(unsigned long)buffer[out]; //data consume
		counter--; //공유변수인 counter를 1 뺌으로써 데이터를 소비했음을 나타냄.  
		printf("consumer - out: %d, counter: %d, data: %ld\n",out,counter,data);  

		///////////////////Critical Section///////////////////////      

		pthread_mutex_unlock(&mutex); //mutex unlock                
		sem_post(&full); // full에게 시그널을 보내 대기 상태에서 깨어나게 한다, 자원을 하나 반납받음.   

		srand(time(NULL));                                                                               
		for(j=0;j<rand()%0xFFFFFFF;j++); //sleep과 같은 효과                                        
	}                                                                                             
}      
int main(void) {
	int j, i;   

	sem_init(&full, 0, MAX_BUF); //producer semaphore(생산자 입장에서 MAX_BUF개의 데이터가 없어지면 full)         
	sem_init(&empty,0,0); // consumer semaphore(소비자 입장에서 현재 아무것도 없으므로 0으로 초기화)   

	pthread_t threads[10];                     

	for(j=0;j<5;j++){                                                                                          
		pthread_create(&threads[2*j],NULL,&producer,NULL);//5 producer threads                                
		pthread_create(&threads[2*j+1],NULL,&consumer,NULL);//5 consumer threads                           
	}                 

	for(i=0;i<10;i++){                                                                                 
		pthread_join(threads[i],NULL);                                                              
	}        

	sem_destroy(&full);                                                                      
	sem_destroy(&empty);  

	return 0;                                                                       
}      