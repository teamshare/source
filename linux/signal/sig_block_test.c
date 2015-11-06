/*
 * 说明：本例用于展示信号集的用法
 * 执行sig_block_test &后，在BLOCK_SLEEP_TIME_S时间内执行kill -s signo pid的话，会看到该信号被pending了。
 * */

#include "signal.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"

#define TEST_SIGNO              (SIGRTMIN + 10)
#define BLOCK_SLEEP_TIMS_S      10

static void my_op(int signum)
{
	printf("receive signal %d \n",signum);
}

int main(int argc, char **argv)
{
	sigset_t new_mask, old_mask, pending_mask;
	struct sigaction act;

	printf("process id is %d \n", getpid());

	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = (void*)my_op;
	if(sigaction(TEST_SIGNO, &act, NULL))
		printf("install signal SIGRTMIN+10 error\n");

	sigemptyset(&new_mask);
	sigaddset(&new_mask, TEST_SIGNO);
	if(sigprocmask(SIG_BLOCK, &new_mask, &old_mask))             /* SIG_BLOCK，在进程当前阻塞集中添加new_mask指向信号集中的信号; SIG_UNLOCK，反之 */
		printf("block signal SIGRTMIN+10 error\n");
	sleep(BLOCK_SLEEP_TIMS_S);

	printf("now begin to get pending mask and unblock SIGRTMIN+10\n");

	if(sigpending(&pending_mask) < 0)                            /* 获得当前已递送到进程，却被阻塞的所有信号，在pending_mask指向的信号集中返回结果 */
		printf("get pending mask error\n");

	if(sigismember(&pending_mask, TEST_SIGNO))                   /* 判定TEST_SIGNO是否在pending_mask指向的信号集中 */
		printf("signal SIGRTMIN+10 is pending\n");

	if(sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)            /* SIG_SETMASK, 更新进程阻塞信号集为old_mask指向的信号集 */
		printf("recover signal mask error\n");

	printf("signal mask recover\n");

	while(1);
}

