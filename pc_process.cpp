#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#define BUFSIZE 10
int main() {
	int fd[2];//for using pipe
	//0 : 읽기용 파이프, 1 : 쓰기용 파이프
	//데이터를 fd[1]에 쓰면 fd[0]으로 그 데이터를 읽을 수 있음.
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


	a = strlen(bufin);//bufin의 길이를 a에 대입
	if (pid > 0) {//parent process

		printf("This is parent process : %ld\n\n", (long)getpid());
		//strcpy(buf,"This is from parent\n");
		write(fd[1], bufout, strlen(bufout));
		//파이프에 bufout의 내용을 적는다.

		//wait(&status);
		waitpid(pid, &status, 0);//wait for returning child process

	}
	else {//child process
		printf("This is child process : %ld\n\n", (long)getpid());
		a = read(fd[0], bufin, BUFSIZE);
		//파이프에 적힌 내용을 bufin으로 읽어들인다.
		//a는 실제 읽은 바이트 수
	}

	printf("[%ld]: bufin is {%.*s}\n\tbufout is {%s}\n", (long)getpid(), a, bufin, bufout);
	//parent'a : strlen(buf), child'a : 실제로 read한 수
	//
	printf("processer-consumer performed\n\n");

	return 0;

}