#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

void sigroutine(int signo) {
	switch (signo) {
	case SIGALRM:
		printf("Catch a signal -- SIGALRM \n");
		break;
	case SIGVTALRM:
		printf("Catch a signal -- SIGVTALRM \n");
		break;
	case SIGPROF:
		printf("Catch a signal -- SIGPROF\n");
		break;
	}
}

int main(int argc, char **argv)
{
	struct itimerval value, value2, value3, ovalue;

	printf("process id is %d \n", getpid());
	signal(SIGALRM, sigroutine);                    /* 信号安装 */
	signal(SIGVTALRM, sigroutine);
	signal(SIGPROF, sigroutine);
	value.it_value.tv_sec = 2;	                /* 本次的设定值 */
	value.it_value.tv_usec = 0;
	value.it_interval.tv_sec = 1;                   /* 下次的取值 */
	value.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &value, &ovalue);	/* 按"实际系统时间"计时 */
	value2.it_value.tv_sec = 4;
	value2.it_value.tv_usec = 0;
	value2.it_interval.tv_sec = 1;
	value2.it_interval.tv_usec = 0;
	setitimer(ITIMER_VIRTUAL, &value2, &ovalue);    /* 按"该进程用户态执行时间"计时 */
	value3.it_value.tv_sec = 6;
	value3.it_value.tv_usec = 0;
	value3.it_interval.tv_sec = 1;
	value3.it_interval.tv_usec = 0;
	setitimer(ITIMER_PROF, &value3, &ovalue);       /* 按"该进程用户态执行时间+调用的系统调用执行时间"计时 */

	while(1);
}
