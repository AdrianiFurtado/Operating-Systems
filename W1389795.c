
//	Adriani Furtado W1389795

//Program will create 2 children whom will pass random letters through a pipe and print to the screen
//Program will only start after giving it a SIGUSR2 signal
//Once running it will create the random number by pressing CTRL-C
//The number will be converted into a lowercase letter and piped from child 1 to child 2
//Child 2 converts lowercase letter to uppercase and prints it to the screen
//Parent will print information to a file called "Output.txt"
//program will exit, cleaning itself up

#include<time.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<setjmp.h>

void handler(int signo);

int randomN=97;	//global variables to be accessed from anywhere in the code
pid_t Pid2;

int main(){

	int i, fd[2];
	pid_t Pid1;
	FILE *fp;
	char cwd[100];
	jmp_buf jb;
	int mask;
	time_t timenow = time(NULL); //get time program started 

		printf("Time:%s", ctime(&timenow));//conver time to a readable format and print time to the screen
		printf("To begin the program open a new terminal and type:\nkill -SIGUSR2 %i\n", getpid());//User info
		signal(SIGUSR1, handler);

		if((signal(SIGUSR2, handler))==SIG_ERR){ //Signal handler
		
			printf("Signal SIGTSTP Error!\n");//Error check message 
		}
	
		pause();//await user input
				
		if((signal(SIGINT, handler))==SIG_ERR){//Signal handler error check
			printf("Signal Interrupt Error!");
		}
		if((signal(SIGALRM, handler))==SIG_ERR){
			printf("Signal Alarm Error!");
		}

	if(pipe(fd) == -1){//pipe creation error check
		
		printf("Pipe creating error\n");
		return 1;
	}

	if((Pid1 = fork()) == -1){//Fork 1 error checking
		
		printf("Fork error!\n");
		return 2;

	}else if(Pid1 == 0){			//Child 1

		pause();
		close(fd[0]);
		dup2(fd[1],STDOUT_FILENO);
		char letter = randomN;
		printf("%c\n", letter);

	}else if(Pid1 > 0){		//Parent V1
		
		wait();//wait for *Child 1* to finish before continuing the code
		
		if((Pid2=fork()) == -1){// Error check fork 2

			printf("Fork error\n");
			exit(3);

		}else if(Pid2 == 0){	//Child 2

			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			char word = NULL;
			scanf("%c", &word);
			printf("\nValue received is [%i]\n", word);
			printf("Value letter is [%c]\n", word);
			printf("Letter converted to [%c]\n",word-32);
			alarm(5);
	
		}else if(Pid2 >0){	//Parent V2
			
			wait();//wait for child 2 to finish 
			fp = fopen("Output.txt","w+");//open file Output.txt/ create if non existent
	                dup2(fileno(fp),STDOUT_FILENO);//man STDOUT to file
			getcwd(cwd, sizeof(cwd));//get current working directory
			printf("Output file location is: %s\n", cwd);
			printf("Program started %s", ctime(&timenow));//convert time value to a readable string
			printf("\nPID's used\n");
			printf("Parent PID:%i\n", getpid());
			printf("Child 1 PID:%i\n", Pid1);
			printf("Child 2 PID:%i\n", Pid2);
			fclose(fp);
	
		}
	} 	
}

void handler(int signo){

	if(signo == SIGINT){//SIGINT handler 
		struct timeval t; // creat a variable with a timevalue structure
		gettimeofday(&t, NULL); //get time in milliseconds and ignore the time zone

		srand((unsigned) t.tv_sec); // seed random with time in milliseconds
		randomN =(rand()%26)+97; //generate a number starting at 97 with a 26 number range. 97 = ascii value of letter 'a'

	}else if(signo == SIGUSR2){//SIGTSTP handler  !!!!!!CHANGE TO SIGUSR

                printf("\tPress CTRL-C to generate random number\n");
  
	}else if(signo == SIGALRM){//SIGALRM handler

		printf("Child 2 terminated\n");
		kill(Pid2 , SIGKILL);

	}else if(signo == SIGUSR1){//Easter egg

                //*Easter egg goes here*
                int num , spaces=12, stars = 1, i;
                for(num=0;num<7;num++){
                        for(i=0;i<spaces;i++){

                                printf(" ");
                        }
                        spaces-=1;
                        for(i=0;i<stars;i++){
                                printf("*");
                        }
                        stars+= 2;
                        printf("\n");
                }
                for(i=0;i<12;i++){
                        printf(" ");
                }
                printf("|\n");
                printf("    Merry Christmass\n");
                exit(10);
	}
}
