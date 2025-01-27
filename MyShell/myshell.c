#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int spawn (char* program, char** arg_list)
{
	pid_t child_pid;

	/* Duplicate this process.  */
	child_pid = fork ();
	if (child_pid != 0)
		/* This is the parent process.  */
		return child_pid;
	else {
		/* Now execute PROGRAM, searching for it in the path.  */
		execvp (program, arg_list);
		/* The execvp function returns only if an error occurs.  */
		fprintf (stderr, "an error occurred in execvp\n");
		abort ();
	}
}

int printfile(char* filename){
	//Code for printfile
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen(filename, "r");
        //printf("%s", "what3");
	if (fp == NULL){
    		//printf("%s", "what");
        	exit(EXIT_FAILURE);
	}
    	
	char buffer[256];  
	char newlinekey;               
	while((fgets(buffer, 256, fp))!= NULL) {
		buffer[strcspn(buffer,"\n")] = 0;
		fflush( stdout );
		printf(buffer);
		fflush(fp);
		getc(stdin);
		fflush( stdout );
		
	}

	//printf("%s", "what2");
	fclose(fp);
}

int printftof(char* buffer){
	// print one file lines to another file

	char *p;
	p = strtok (buffer," ");
	char *strs[4]; 
  	int i = 0;
	while (p != NULL){
		strs[i] = p;
		//printf ("%s\n", strs[i]);
		p = strtok (NULL, " ,");
		i = i +1;
	}
	
	char* arg_list_file[] = {
		"cat ",
		strs[1],
		" >> ",
		strs[3],
		NULL
	};
	
	char caty[128] = "cp "; 
	strncat(caty, strs[1], 50);
	strncat(caty, " ", 50);
	strncat(caty, strs[3], 50);
	system(caty);
	
	
	//spawn ("cat", arg_list_file);
	fflush( stdout ); 
}


int dididothat(char* buffer, char history[15][128], int j ){
	// dididothat function read history and decide is command in it
	int i = 0;
	char string[128];
	char * token;
	strcpy(string , buffer);
	int len = strlen(buffer);
	token = strtok(string, "\"");
	token = strtok(NULL, "\"");
	//printf(token);


	if (j > 15) {
		j = 15;
	}
	
	char answer[4] = "No\0";
	while (i < j){
        	//printf("\n Elment is %s", history[i]);
        	if (strcmp(history[i],token) == 0){
        		strcpy(answer,"Yes\0");
        	}
		i = i + 1;
        }
        printf("%s\n", answer);
        
}

int main ()
{
	/* The argument list for the commands. */
	char* arg_list_hostname[] = {
		"hostname",    
		NULL      
	};

	char* arg_list_ls[] = {
		"ls",
		NULL
	};

	char* arg_list_ip[] = {
		"hostname",
		"-i",
		NULL
	};

	char* arg_list_file[] = {
		"file",
		"-f",
		"./Pipe.c",
		NULL
	};
	char* arg_list_who[] = {
		"whoami",
		NULL
	};
	
	char* arg_list_gedit[] = {
		"gedit",
		NULL
	};
	
	char command_history[15][128];
  

     	char user[64];
	getlogin_r(user,sizeof(user)-1);
	
	int i = 0;
	int j = 0;
	char * token;
	char string[128];
	int ishist = 1;
	while(1){

		char buffer[128];
        	fflush( stdin );
        	printf(user);
        	printf(" >>> ");
        	fflush( stdout );
        	fgets(buffer, 128, stdin);
 
		int len = strlen(buffer);
		 
		// Remove the '\n' character and replace it with '\0'
		buffer[len - 1] = '\0';
        	
        	char command[128];
        	ishist = 1;
        	
        	strcpy(string , buffer);
        	//printf(buffer);
        	token = strtok(string, " ");
        	if (strcmp(token,"listdir") == 0){
        		//spawn ("ls", arg_list_ls); 
        		//fflush( stdout );
        		strcpy( command, "ls" );
   			system(command);
        	} else if (strcmp(token,"mycomputername") == 0){
        		//spawn ("hostname", arg_list_hostname);
        		strcpy( command, "hostname" );
   			system(command);
        		fflush( stdout ); 
        	} else if (strcmp(token,"whatsmyip") == 0){
        		//spawn ("hostname", arg_list_ip);
        		strcpy( command, "hostname -i" );
   			system(command);
        		fflush( stdout ); 
        	} else if (strcmp(token,"printfile") == 0){
        		char * s;
        		s = strchr(buffer,'>');
        		if (s == NULL) {
        			char * filename;
        			filename = strtok(NULL, " ");
        			printfile(filename);
        		} else {
        			char st[128];
        			strcpy(st,buffer);
        			printftof(st);
        		}
        		fflush( stdout ); 
        	} else if (strcmp(token,"dididothat") == 0){
        		dididothat(buffer,command_history,j);
        		ishist = 0;
        		fflush( stdout ); 
        	} else if (strcmp(token,"hellotext") == 0){
        		spawn ("gedit", arg_list_gedit);
        		fflush( stdout ); 
        	} else if (strcmp(token,"exit") == 0){
        		//printf ("done with main program\n");
        		break;
        	} else {
        		ishist = 0;
        	}
        	
		//printf(token);
        	token = "";
		//line_by_line("./Pipe.c");
		
		if(ishist){
			strcpy(command_history[i],buffer);
			j = j + 1;
			i = i + 1;
			i = i % 15;
		}

	}

	//printf ("done with main program\n");

  return 0;
}
