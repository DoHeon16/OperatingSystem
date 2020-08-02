#include<stdio.h>      
#include<stdlib.h>       
#include<pthread.h>       
#include<time.h>           
#include<semaphore.h>       

#define MAX_BUF 32 //buffer�� ũ��� 32�� ����     
#define MAX_LOOP 32 //loop�� 32�� ����          

int buffer[MAX_BUF];                                
int counter=0;                                       
int in=-1,out=-1;                              

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;       
sem_t full,empty;                                     

void *producer(void *arg) {

	int i, j;                                                                                                                                                     unsigned long data;                                                                                                                                                                                                                                                                                                       for (i = 0; i < MAX_LOOP; i++) {
		srand(time(NULL));  
		for (j = 0; j < rand() % 0xFFFFFFFF; j++);   
		data = (unsigned long)j; //buffer�� �Ҵ��� data ����  
		//pthread_mutex_lock(&mutex);    

		sem_wait(&full); //full�� ��ȣ�� �ö����� ���, �ڿ��� �ϳ� �ִ� ��         
		pthread_mutex_lock(&mutex); //mutex lock                                   
		
		/////////////////Critical Section///////////////         

		in++; in%=MAX_BUF; //produce++                                                      
		buffer[in]=(int)data; //buffer�� data�� ����������ν� produce                      
		counter++;//���������� counter�� 1 �������ν� ���������� ǥ��.     

		printf("producer - in: %d, counter: %d, data: %ld\n",in,counter,data);    

		////////////////Critical Section//////////////////                  

		pthread_mutex_unlock(&mutex); //mutex unlock                          
		sem_post(&empty); //empty�� �ñ׳��� ���� ��� ���¿��� ����� ��, �ڿ��� �ϳ� �ݳ��޴´�.    

	}                                                                                                  
}     
void *consumer(void *arg) {

	int i, j;                                                                                                                                                     unsigned long data;                                                                                                                                                                                                                                                                                                       for (i = 0; i < MAX_LOOP; i++) {
		sem_wait(&empty); //empty�� ��ȣ�� �ö����� ���, �ڿ��� �ϳ� �ִ� ��.
		pthread_mutex_lock(&mutex); //mutex lock
									
		///////////////////Critical Section/////////////////////////

		out++; out%=MAX_BUF; //comsume++               
		data=(unsigned long)buffer[out]; //data consume
		counter--; //���������� counter�� 1 �����ν� �����͸� �Һ������� ��Ÿ��.  
		printf("consumer - out: %d, counter: %d, data: %ld\n",out,counter,data);  

		///////////////////Critical Section///////////////////////      

		pthread_mutex_unlock(&mutex); //mutex unlock                
		sem_post(&full); // full���� �ñ׳��� ���� ��� ���¿��� ����� �Ѵ�, �ڿ��� �ϳ� �ݳ�����.   

		srand(time(NULL));                                                                               
		for(j=0;j<rand()%0xFFFFFFF;j++); //sleep�� ���� ȿ��                                        
	}                                                                                             
}      
int main(void) {
	int j, i;   

	sem_init(&full, 0, MAX_BUF); //producer semaphore(������ ���忡�� MAX_BUF���� �����Ͱ� �������� full)         
	sem_init(&empty,0,0); // consumer semaphore(�Һ��� ���忡�� ���� �ƹ��͵� �����Ƿ� 0���� �ʱ�ȭ)   

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