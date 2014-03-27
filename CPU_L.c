/*
 * Без имени.c
 * 
 * Copyright 2014 Lex <lex22@ubuntu>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>



void handler(int a){
	exit(0);
}

void cpu_limit(double percent, char** argv){
	int pid = fork();
	if(pid == -1){
		fprintf(stderr, "error\n");
		exit(1);
	}
	if(pid == 0){//child
		int qw = execvp(argv[2], argv + 2);
		if(qw == -1){
			fprintf(stderr, "error\n");
			exit(1); 
		}
	}
	else{//parent
		struct sigaction q;//signal to die
		q.sa_handler = handler;
		q.sa_flags = SA_NOCLDSTOP;// kid sends SIGCHLD signal when he dies or recieves SIGSTOP signal. ignore it 
		sigaction(SIGCHLD, &q, NULL);//3 - where send old struct sigaction
		while(1){
			usleep(percent * 10000);//work
			kill(pid, SIGSTOP);//sleep
			
			usleep((100 - percent) * 10000);//sleep
			kill(pid, SIGCONT);//wake up
		}
	}
}

int main(int argc, char **argv){
	double percent = 0;
	if (argc < 3){
		fprintf(stderr, "error\n");
		exit(1);
	}
	
	sscanf(argv[1], "%lf", &percent);
	
	if (percent <= 0 || percent >=100){
		fprintf(stderr, "error\n");
		exit(1); 
	}
	cpu_limit(percent, argv);
	return 0;
}

