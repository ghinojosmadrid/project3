#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12
#define maxArg 20

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	for (int i = 0; i < N; i++) {
		if (strcmp(cmd, allowed[i]) == 0) {
			return 1;
		}
	}

	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
    char *argv[21];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c

	char *cmd = strtok(line, " ");
	if (!cmd) continue;

	if(!isAllowed(cmd)) {
		printf("NOT ALLOWED!\n");
		continue;
	}

	int i = 0;
	argv[i++] = cmd;
	char *arg;

	while ((arg = strtok(NULL, " ")) && i < maxArg) {
		argv[i++] = arg;
	}
	argv[i] = NULL;

	if (strcmp(cmd, "exit") == 0) {
		return 0; //exit the shell
	}

	else if (strcmp(cmd, "help") == 0) {
		printf("The allowed commands are:\n");
		for (int j =0; j < N; j++) {
			printf("%d: %s\n", j + 1, allowed[j]);
		}
	}
	
	else if (strcmp(cmd, "cd") == 0) {
		if (i > 2) { //more than one arg for cd
			printf("-rsh: cd: too many arguments\n");
			}
		else if (i == 2) {
			chdir(argv[1]);
		}
		continue;
	}
	else { //process for external commands
		pid_t pid;
		int status;
		if (posix_spawnp(&pid, cmd, NULL, NULL, argv, environ) != 0) {
			continue;
		}
		waitpid(pid, &status, 0);
	}

    }
    return 0;
}
