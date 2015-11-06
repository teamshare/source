/*
 * 测试case说明: 向进程本身发送信号，并传递指针参数。
 * 这个例子中，信号实现了附加信息的传递，信号究竟如何对这些信息进行处理则取决于具体的应用。
 *
 * */

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DATA_SIZE         10

void new_op(int signum, siginfo_t *info, void *myact)
{
	int i;
	printf("\nhandle signal %d.\n", signum);
	printf("si_ptr: ");
	for(i = 0; i < DATA_SIZE; i++) {
		printf("%c ", *((char *)(info->si_ptr) + i));
	}
	printf("\nhandle signal %d over.\n", signum);
}

int main(int argc,char**argv)
{
	int i;
	int sig;
	pid_t pid;
	struct sigaction act;
	union sigval mysigval;                        //union型(.sigval_int/.sigval_ptr)
	char data[DATA_SIZE];

	pid = getpid();
	printf("process id is %d \n", pid);

	sig = atoi(argv[1]);
	sigemptyset(&act.sa_mask);
	act.sa_sigaction = new_op;                    //三参数信号处理函数
	act.sa_flags = SA_SIGINFO;                    //信息传递开关，允许传参数信息给new_op
	if(sigaction(sig, &act, NULL) < 0){
		printf("install sigal error\n");
	}

	memset(data, 0, sizeof(data));
	for(i=0; i < DATA_SIZE; i++)
		data[i] = '0' + i;
	mysigval.sival_ptr = data;
	while(1) {
		sleep(2);
		printf("wait for the signal %d\n", sig);
		sigqueue(pid, sig, mysigval);         //向本进程发送信号，并传递附加信息
	}
}

