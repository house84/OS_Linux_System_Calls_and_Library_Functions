//Author: Nick House
//Project: Linux SYstem Calls and Library Functions
//Course: CS-4760 Operating Systems,Spring 2021
//Date: 08 February 2021

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char **environ;
void show_help(char *filename);
void build_Name_List(char * programName);
char ** myNames = NULL;
void print_env();
int myNameListLength;
void free_Name_List();

main(int argc, char * argv[], char *envp[])
{

    //If no args Print environment
    if(argc == 1){

        //Build Name List
        build_Name_List(argv[0]);

        //Print Environment the hard way
        print_env();

        //You are Free
        free_Name_List();

        return 0;
    }

    int c = 0;
    int flag = 0;

    //Check for Command Line options
    while((c = getopt(argc, argv, "ih")) != -1){

        switch(c){

            case 'i': flag = 1;
                      break;

            case 'h': show_help(argv[0]);
                      break;

            default:
                     break;
        }
    }

    //Delete Environment
    if(flag == 1) {

       //Build Name List
       // build_Name_List(argv[0]);

       //Delete Environment Using unsetenv()
       // int i;
       // for (i = 0; i < myNameListLength; i++) {
            
       //   if(  unsetenv(myNames[i]) == -1 ){
	     
       //      	fprintf(stderr, "$s: ", argv[0]); 
       //     	perror("Error");
       //      	exit(EXIT_FAILURE); 
       //    }

       
	 //Clear Environment Parameters Using clearenv() 
       	if( clearenv() != 0) {
		fprintf(stderr, "@s: ", argv[0]);
		perror("Error");
		exit(EXIT_FAILURE); 
	}
    }

    //Initialize i 
    int i = optind;     
    
    //Set index tracker for Utility String
    int set_index_Utility = i+1; 

    //Set Name=Value, set Utility Index
    for(i; i < argc; ++i)
    {
	set_index_Utility = i; 


        //Extract Name and Values and Set Envrion
        //strtok() could have been utilized
        if(strchr(argv[i], '=')) {
 
            char envName[strlen(argv[i])+1];
            strcpy(envName, argv[i]);
	
            char * myptr;
            char * valueptr;

            myptr = strchr(envName, '=');
            *myptr = '\0';

            valueptr = myptr;
            valueptr++;

            //Add Environment Params, Check for Failure
	   if( setenv(envName, valueptr, 1) != 0){
	
		fprintf(stderr, "%s: ", argv[0]);
		perror("Error");
		exit(EXIT_FAILURE); 
	   }

        }

        else break;    
   }



        //Execute Utility with system(3)
        //Everything indexed after last name=value pair turn to string and pass to system()
        //set initial utility arg size
	int UASize = 50;
        char utilityArg[UASize];
	utilityArg[0] = '\0'; 

        //Convert Arguments into string 
	for(i = set_index_Utility; i < argc; ++i){
     		
	    //If Utility Argument larger than Allocated space increase size
	    if(sizeof(utilityArg) > 50) { 
		
		UASize += 1;
 
		char tempUtilityArg[sizeof(utilityArg)];

		strcpy(tempUtilityArg, utilityArg);

		utilityArg[UASize];

		strcpy(utilityArg, tempUtilityArg); 	

	    }

	    //Concatneate Args into String
	    strcat(utilityArg, argv[i]);
            
	    //Add space between args. 
	    if(i != argc-1){

                strcat(utilityArg, " ");

            }
      }
     
    //Call System(3) with Command Line inputs
    //Error Handling using perror 
    if(system(utilityArg) == -1 ){
	
	fprintf(stderr, "%s: ", argv[0]);
	
	perror("Error");
	
	free_Name_List();
		
	exit(EXIT_FAILURE);   
    }

  // printf("%s\n", system(utilityArg));
  //  free_Name_List(); 
  
    return 0;
}

//Usage Fumction
void show_help(char * filename)
{

    printf("\nUssage: %s [option] [NAME=VALUE]... {COMMAND1 [; COMMAND2] [: ...]}\n", filename);
    printf("Set each NAME to VALUE in the environment and run COMMAND.\n\n");
    printf("\tNo Argument displays environment\n");
    printf("\t-i\tStart with an empty environment\n");
    printf("\t-h\tHelp, Displaye Usage\n\n");
  
}


//free name list
void free_Name_List()
{
    if(myNames != NULL)
    {
        free(myNames);
        myNames=NULL;
    }

    myNameListLength = 0;
}


//Dynamically Create List of Environment Names
void build_Name_List(char *programName)
{
    //Reset Env Length
    myNameListLength = 0;
  
    int myNamesSize = 10;
    myNames = malloc(myNamesSize * sizeof(char *)); 

    int i;
    for (i = 0; environ[i+1] != NULL; i++){
 
	//Allocate Space
        if(i == myNamesSize)
        {
            myNamesSize += 2;
            char **temp = realloc(myNames, myNamesSize*sizeof(char *));

            if(temp == NULL )
            {
                //ERROR Check on Error Setting.
                fprintf(stderr, "%s: Error: Failed to reallocate memory.\n", programName); 
                exit(EXIT_FAILURE);
            }

            myNames = temp;
        }
 
	myNames[i] = malloc(strlen(environ[i]+1));
        
	strcpy(myNames[i], environ[i]);

        //Create Pointer for '=' Location
        char *ptr;
        ptr = strchr(myNames[i], '=');
        *ptr = '\0';

        //Increment List Length
        myNameListLength = i+1;
    }
}

//Display the Environment
void print_env() {
    int i;
    for (i = 0; i < myNameListLength; ++i) {
        printf("%s=%s\n", myNames[i], getenv(myNames[i]));
    }
}
