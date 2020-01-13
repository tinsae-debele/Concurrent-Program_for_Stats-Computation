


/**
 *
 * @author Tinsae 
 *
 * This program used for sorting an array of integer in non increaseing way by using semaphore base mechanism
 * while running concurrent program for stats compulation. There are four process is created. Each process are
 * associated with two array. every process P repeatedly checks its two elements until the entire list of integers is sorted
 *
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include "format.h"
#include <sys/sem.h>
#include "semun.h"

static int set_semvalue(void);           // intialize the methods semvalue
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);
static int sem_id;


void printR(int Val[5]);
/**
 *  this function prints the value with formated way;
 */

void printResult(int val[5])               // a fucntion for printing the result value
{
	for (int i = 1; i <= 5; i++)             // a loop for printing each values
	{
		printf("%3i ", val[i - 1]);            // agive a speace for each vlaue
		if (i%5 == 0)
			printf("\n");
	}
}

static int sem_id;

int main(int argc, char *argv[])
{
 /*---------------------------------------------------------------------------*/


 /**
  * creates a shared memory for Four process to exchange value but concurrently
  *
  */
    void *shared_memory = (void *)0;   // intialize the shared memory
    struct resultStr *result;          // sturucter of shared memory
    int shmid;                         // shm id intialziing

    srand((unsigned int)getpid());     //
    shmid = shmget((key_t)4567, sizeof(struct resultStr), 0666 | IPC_CREAT);   //creates a share memory

    if (shmid == -1)                   // check if the sheget filed
	{
		fprintf(stderr, "shmget failed\n");      // out put a message if the shmget is failed
		exit(EXIT_FAILURE);                      // exit with faile user
	}


	shared_memory = shmat(shmid, (void *)0, 0);

	if (shared_memory == (void *)-1)                   // check if the shared memory is reutrn -1 which is faile
	{
		fprintf(stderr, "shmat failed\n");               // out put the message if the share value is failed
		exit(EXIT_FAILURE);
	}

	result = (struct resultStr *)shared_memory;        // attach the shared memory to the reuslt
/*---------------------------------------------------------------------*/
    /**
     * A for loop to store an inteial entered values in to the result value for sorting process
     */
    printf("\nDo you want to enable Debuging Mode [y for YES and n for NO]: \n");    // request the user for input of debuging or not
    char t;                                                                          // char to store the users value
    scanf("%c", &t);                                                                 // reads the user imput


    printf("\nPlease, Enter a Five Distinct Integer value(separated by space Ex.1 2 3 4 5) : \n");      // request fo user to imput the number to arrange
     for (int i = 0; i < 5; i++) {                      // loops five time for each input to compute

        int a;                                          // int value that we store the user input
        scanf("%d", &a);                                // reads user values for each integer
        if(a >= 0){
            (result->data)[i] = a;                      // stores it on the shared memory
        }else{                                           // out put an error message if the user input wrong value
            printf("Invalid input: Please enter valid value and try again\n");
            exit(EXIT_FAILURE);
        }

    }

	printf("\nThe Five distinc Integers Value:\n");
	printResult(result->data);
	printf("\n");

	pid_t pid[4], Tpid;                             // initalize the pid for chech children

    int tin;
/*----------------------------------------------*/
  //Semaphore create and decleration


    srand((unsigned int)getpid());

    sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);    // semget created with a a call semget and return the ID

    if (argc > 1) {                                                   // chekc if the semaphor is intialize properly
        if (!set_semvalue()) {                                        // check if the semaphor is created
            fprintf(stderr, "Failed to initialize semaphore\n");      // return an error message if the semaphore is filed to created
            exit(EXIT_FAILURE);
        }
    }

/*-----------------------------------------------------------------------------------*/
				for (int i = 0; i < 4; i++)   //  the loop is used to create 4 childprocess
				{
					pid[i] = fork();            // creates a four child process.
					if (pid[i] == 0)            // condaiton checks if the chiild process is created
					{
						break;                    // exit if the condaiton is treue
					}
				}

   /**
    * Then we have a loop which enters and leaves the critical section ten times.
    * There, we first make a call to semaphore_p which sets the semaphore to wait,
    *  as this program is about to enter the critical section.
    */


   while((result->data)[0] < (result->data)[1] |           // condatin to be true to go through the loop and check if it is sorted non increaased
         (result->data)[1] < (result->data)[2] |
         (result->data)[2] < (result->data)[3] |
         (result->data)[3] < (result->data)[4]){
           for(int i = 0; i < 4; i++) {                    // For loop two create four process taht are assoccated with two array element;
					 pid[i] = fork();
						                                     // create a child process and store is in PID array;
             if(pid[0] == 0){
                    if (!semaphore_p()) exit(EXIT_FAILURE);                              // make a call to semaphore_p which sets the semaphore to wait,
                    if((result->data)[i] < (result->data)[i + 1]){                       // checks its two elements until the entire list of integers is sorted
                        if(t == 'y'){
                            printf("Process P%d: Performing Swapping: \n",(1));
                        }
                        int temp =(result->data)[i];                                     // stores the first value to temp
                        (result->data)[0] = (result->data)[1];                       // changes the first valwe with 2nd
                        (result->data)[1] = temp;                                    // store the temp(1st ) to the 2nd
                        exit(EXIT_SUCCESS);
                    } else{
                          if(t == 'y'){
                            printf("Process P%d: No Swapping: \n",(1));
														exit(EXIT_SUCCESS);
                        }
                    }
                /**
                 * After the critical section, we call semaphore_v, setting the semaphore available,
                 * After the loop, the call to del_semvalue is made to clean up the code.
                 */

                    if (!semaphore_v()) exit(EXIT_FAILURE);
                    exit(EXIT_SUCCESS);
                }else if(pid[1] == 0){

                    if (!semaphore_p()) exit(EXIT_FAILURE);                              // make a call to semaphore_p which sets the semaphore to wait,
                    if((result->data)[i] < (result->data)[i + 1]){                       // checks its two elements until the entire list of integers is sorted
                        if(t == 'y'){
                             printf("Process P%d: Performing Swapping: \n",(2));
                        }

                        int temp =(result->data)[1];                                     // stores the first value to temp
                        (result->data)[1] = (result->data)[2];                       // changes the first valwe with 2nd
                        (result->data)[2] = temp;                                    // store the temp(1st ) to the 2nd
                        exit(EXIT_SUCCESS);
                    }else
                    {
                        if(t == 'y'){
                             printf("Process P%d: No Swapping: \n",(2));
														 exit(EXIT_SUCCESS);
                        }
                    }

                /**
                 * After the critical section, we call semaphore_v, setting the semaphore available,
                 * After the loop, the call to del_semvalue is made to clean up the code.
                 */
                    if (!semaphore_v()) exit(EXIT_FAILURE);
                    exit(EXIT_SUCCESS);
                }else if(pid[2] == 0){

                    if (!semaphore_p()) exit(EXIT_FAILURE);                              // make a call to semaphore_p which sets the semaphore to wait,
                    if((result->data)[i] < (result->data)[i + 1]){                       // checks its two elements until the entire list of integers is sorted
                        if(t == 'y'){
                             printf("Process P%d: Performing Swapping: \n",(3));
                        }
                        int temp =(result->data)[2];                                     // stores the first value to temp
                        (result->data)[2] = (result->data)[3];                       // changes the first valwe with 2nd
                        (result->data)[3] = temp;                                    // store the temp(1st ) to the 2nd
                        exit(EXIT_SUCCESS);
                    } else{
                        if(t == 'y'){
                             printf("Process P%d: No Swapping: \n",(3));
														 exit(EXIT_SUCCESS);
                     }
                     }

                /**
                 * After the critical section, we call semaphore_v, setting the semaphore available,
                 * After the loop, the call to del_semvalue is made to clean up the code.
                 */
                    if (!semaphore_v()) exit(EXIT_FAILURE);
                    exit(EXIT_SUCCESS);
                }else if(pid[3] == 0){

                    if (!semaphore_p()) exit(EXIT_FAILURE);                              // make a call to semaphore_p which sets the semaphore to wait,
                    if((result->data)[i] < (result->data)[i + 1]){                       // checks its two elements until the entire list of integers is sorted
                        if(t == 'y'){
                             printf("Process P%d: Performing Swapping: \n",(4));
                        }
                        int temp =(result->data)[3];                                     // stores the first value to temp
                        (result->data)[3] = (result->data)[4];                       // changes the first valwe with 2nd
                        (result->data)[4] = temp;                                    // store the temp(1st ) to the 2nd
                        exit(EXIT_SUCCESS);
                    }else{
                        if(t == 'y'){
                             printf("Process P%d: No Swapping: \n",(4));
														 exit(EXIT_SUCCESS);
                     }
                    }
                /**
                 * After the critical section, we call semaphore_v, setting the semaphore available,
                 * After the loop, the call to del_semvalue is made to clean up the code.
                 */
                    if (!semaphore_v()) exit(EXIT_FAILURE);
                    exit(EXIT_SUCCESS);
                }

           }


        }


      /* After the loop, the call to del_semvalue is made to clean up the code. before going through the for loop again after a  wait. */

        if (argc > 1) {
                sleep(10);
                del_semvalue();
                exit(EXIT_SUCCESS);
        }



    // Parent process waits for all children to die
	while ((Tpid = wait(&tin)) > 0);
    //Parent process waits for all children to die
	printf("Sorted Result: \n");
	printResult(result->data);
    printf("\nThe highest Value : %d \n", (result->data)[0]);
    printf("The lowest Value  : %d \n", (result->data)[4]);
    printf("The median Value  : %d \n", (result->data)[2]);
	     /**
		 *clear and delelate the share memory and exit
		 */
	if (shmdt(shared_memory) == -1)
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}

	if (shmctl(shmid, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

/* The function set_semvalue initializes the semaphore using the SETVAL command in a
 semctl call. We need to do this before we can use the semaphore. */

static int set_semvalue(void)
{
    union semun sem_union;

    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return(0);
    return(1);
}

/* The del_semvalue function has almost the same form, except the call to semctl uses
 the command IPC_RMID to remove the semaphore's ID. */

static void del_semvalue(void)
{
    union semun sem_union;

    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        fprintf(stderr, "Failed to delete semaphore\n");
}

/* semaphore_p changes the semaphore by -1 (waiting). */

static int semaphore_p(void)
{
    struct sembuf sem_b;

    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* P() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return(0);
    }
    return(1);
}

/* semaphore_v is similar except for setting the sem_op part of the sembuf structure to 1,
 so that the semaphore becomes available. */

static int semaphore_v(void)
{
    struct sembuf sem_b;

    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* V() */
    sem_b.sem_flg = SEM_UNDO;

    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return(0);
    }
    return(1);
}
