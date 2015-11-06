/*
 * 测试方式：命令行参数为信号值，后台运行sig_rec_test signo &，
 * 可获得该进程的ID，假设为pid，然后再另一终端上运行kill -s signo pid
 * 验证信号的发送接收及处理。同时，可验证信号的排队问题。
 *
 * */

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void new_op(int signum,siginfo_t *info,void *myact)
{
	printf("receive signal %d\n", signum);
	sleep(5);
}

int main(int argc,char**argv)
{
	int sig;
	struct sigaction act;

	printf("process id is %d \n", getpid());
	sig = atoi(argv[1]);
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = new_op;

	if (sigaction(sig, &act, NULL) < 0) {
		printf("install sigal error\n");
	}

	while (1) {
		sleep(2);
		printf("wait for the signo %d\n", sig);
	}
}

