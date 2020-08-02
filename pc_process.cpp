#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#define BUFSIZE 10
int main() {
	int fd[2];//for using pipe
	//0 : �б�� ������, 1 : ����� ������
	//�����͸� fd[1]�� ���� fd[0]���� �� �����͸� ���� �� ����.
	pid_t pid;
	char bufin[BUFSIZE] = "bufin";
	char bufout[] = "write";
	int a = 0;//initialize a
	int status;

	if (pipe(fd) < 0) {//create pipe
		printf("pipe error\n");
		return 1;
	}
	if ((pid = fork()) < 0) {//perform fork
		printf("fork error\n");
		return 1;
	}


	a = strlen(bufin);//bufin�� ���̸� a�� ����
	if (pid > 0) {//parent process

		printf("This is parent process : %ld\n\n", (long)getpid());
		//strcpy(buf,"This is from parent\n");
		write(fd[1], bufout, strlen(bufout));
		//�������� bufout�� ������ ���´�.

		//wait(&status);
		waitpid(pid, &status, 0);//wait for returning child process

	}
	else {//child process
		printf("This is child process : %ld\n\n", (long)getpid());
		a = read(fd[0], bufin, BUFSIZE);
		//�������� ���� ������ bufin���� �о���δ�.
		//a�� ���� ���� ����Ʈ ��
	}

	printf("[%ld]: bufin is {%.*s}\n\tbufout is {%s}\n", (long)getpid(), a, bufin, bufout);
	//parent'a : strlen(buf), child'a : ������ read�� ��
	//
	printf("processer-consumer performed\n\n");

	return 0;

}