/** מגישים
 * דביר גב 209530583
 * דניאל זקן 207296989
*/
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


void sig_handler(int signum)
{

	switch (signum)
	{
	case SIGCHLD:
		printf("I am in SIGCHLD\n");
		fflush(stdout);
		raise(SIGUSR1);
		break;

	case SIGUSR1:
		printf("I am in SIGUSR1\n");
		fflush(stdout);
		raise(SIGUSR2);
		break;

	case SIGUSR2:
		printf("I am in SIGUSR2\n");
		fflush(stdout);
		int id = getpid();
		kill(id, SIGINT);
		break;

	case SIGINT:
		printf("I am in SIGINT\n");
		fflush(stdout);
		int *a = 0;
		*a = 5;
		break;

	case SIGSEGV:                        // Invalid memory reference signal
		printf("I am in SIGSEGV\n");
		fflush(stdout);
		int b=5/0;
		break;

	case SIGFPE:                        //   /0
		printf("I am in SIGFPE\n");
		asm("ud2");
		break;
		
	case SIGILL:
		printf("I am in SIGILL\n");
		exit(0);
		break;
		
	}
}

int main()
{
	int status;
	signal(SIGCHLD, sig_handler);
	signal(SIGUSR1, sig_handler);
	signal(SIGFPE, sig_handler);
	signal(SIGUSR2, sig_handler);
	signal(SIGINT, sig_handler);
	signal(SIGSEGV, sig_handler);
	signal(SIGILL, sig_handler);

	if (!(fork()))
	{
		exit(1);
	}
	wait(0);
}
