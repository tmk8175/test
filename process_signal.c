
// https://www.ibm.com/developerworks/cn/linux/l-ipc/part2/index2.html
// gcc -o signalrecv signalrecv.c
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
void sig_function(int,siginfo_t*,void*);
int main(int argc,char**argv)
{
	if (argc<2)
	{
		printf("%s signum\n", argv[0]);
		return 0;
	}
	struct sigaction act;
	int sig;
	pid_t pid;
	pid=getpid();
	sig=atoi(argv[1]);
	
	sigemptyset(&act.sa_mask);
	act.sa_sigaction=sig_function;
	act.sa_flags=SA_SIGINFO;
	if(sigaction(sig,&act,NULL)<0)
	{
		printf("install sigal error\n");
	}
	while(1)
	{
		sleep(2);
	}
	return 0;
}
void sig_function(int signum,siginfo_t *info,void *myact)
{
	printf("signum:%d, the int value is %d \n", signum, info->si_int);
}



// gcc -o signalsend signalsend.c
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
main(int argc,char**argv)
{
	if (argc<3)
	{
		printf("%s signum pid\n", argv[0]);
		return 0;
	}
	pid_t pid;
	int signum;
	union sigval mysigval;
	signum=atoi(argv[1]);
	pid=(pid_t)atoi(argv[2]);
	mysigval.sival_int=8;//不代表具体含义，只用于说明问题
	if(sigqueue(pid,signum,mysigval)==-1)
		printf("send error\n");
}
