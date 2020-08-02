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
	//main thread�� consumer thread�� �������� ����ȴ�.
	for (i = 0; i < 1000; i++) {
		//produce an item in nextp
		nextp = 'a' + (i % 26);//buffer�� �־��� ������ ����->���ĺ��� �ֱ������� �־��� ��
		while ((in + 1) % n == out); //waiting loop
		buffer[in] = nextp;//buffer�� �����͸� ��´�.
		in++;
		in %= n;
		//test='A'+(i%26);
		//printf("%c",test);
		printf("%d", i % 10);//main loop�� ������ Ȯ�� ���(����)
		//sleep(1);
	}
	pthread_join(tid, NULL);//�ڽ� �����尡 ����� ������ ��ٸ���.
	printf("\nmain thread ����\n");//���� �����尡 ����Ǿ����� �˸�

	return 1;
}

void consumer(void) {
	char nextc = '0';//nextc���� ���ҹ��ڰ� ��� ������ �켱 ���� 0���� ��>��ȭ���ش�.
	int i;
	for (i = 0; i < 1000; i++) {
		while (in == out);//waiting loop
		nextc = buffer[out];
		//���������� buffer�� ����� �ִ� ���� ���� nextc�� ��´�.
		out++;
		out %= n;

		//consume the item in nextc
		printf("%c", nextc);
		//���� �����͸� ����Ѵ�.
	}
}