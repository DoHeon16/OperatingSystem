#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
void consumer(void);

char buffer[1000];
int n = 1000, in = 0, out = 0;

int main() {
	char nextp, test; int i;
	pthread_t tid;

	pthread_create(&tid, NULL, (void*)&consumer, NULL);//consumer thread create

	//main thread performs  producer role
	//main thread와 consumer thread는 병행으로 수행된다.
	for (i = 0; i < 1000; i++) {
		//produce an item in nextp
		nextp = 'a' + (i % 26);//buffer에 넣어줄 데이터 생성->알파벳을 주기적으로 넣어줄 것
		while ((in + 1) % n == out); //waiting loop
		buffer[in] = nextp;//buffer에 데이터를 담는다.
		in++;
		in %= n;
		//test='A'+(i%26);
		//printf("%c",test);
		printf("%d", i % 10);//main loop이 도는지 확인 출력(숫자)
		//sleep(1);
	}
	pthread_join(tid, NULL);//자식 스레드가 종료될 때까지 기다린다.
	printf("\nmain thread 종료\n");//메인 스레드가 종료되었음을 알림

	return 1;
}

void consumer(void) {
	char nextc = '0';//nextc에는 영소문자가 담길 것으로 우선 숫자 0으로 초>기화해준다.
	int i;
	for (i = 0; i < 1000; i++) {
		while (in == out);//waiting loop
		nextc = buffer[out];
		//전역변수인 buffer에 담겨져 있던 것을 빼내 nextc에 담는다.
		out++;
		out %= n;

		//consume the item in nextc
		printf("%c", nextc);
		//빼낸 데이터를 출력한다.
	}
}