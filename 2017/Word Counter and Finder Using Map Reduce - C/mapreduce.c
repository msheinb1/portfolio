#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mapreduce.h"
#include "common.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>

#include <errno.h>
#include <sys/wait.h>

#include <poll.h>

void mapreduce(MAPREDUCE_SPEC * spec, MAPREDUCE_RESULT * result)
{
    
    // My Code ...
    int counter = 0;
    int doesMerge = 0;
    int status = 0;
    struct timeval start, end;

    if (NULL == spec || NULL == result)
    {
        EXIT_ERROR(ERROR, "NULL pointer!\n");
    }
    
    gettimeofday(&start, NULL);
    
    int i;
    pid_t pid;
    int numRead = 0;
    FILE *fptr;
    char temp;
    int approxSplit;
    int fd;
    
    //Creates the buffer that will later open the files where the intermediate files will be stored.
    i = ((spec->split_num/10)+9);
    //printf("Split_num = %d\n Split_num/10 = %d\n Proposed Buffer Size = %d\n", spec->split_num, spec->split_num/10, i);
    char * buffer = malloc(i * sizeof(char));
    int bufSize = i/sizeof(char);
    
    //Creates the object that holds the data splits for the program
    fptr = fopen(spec->input_data_filepath, "r");
    if (fptr == NULL) {
        EXIT_ERROR(ERROR, "NULL pointer!\n");
    }
    DATA_SPLIT * dataSplits = malloc(spec->split_num * sizeof(DATA_SPLIT));
    int * p_fd_in = malloc(spec->split_num * sizeof(int));
    
    struct stat myFile;
    int fileSize;
    if (stat(spec->input_data_filepath, &myFile) == 0) {
        fileSize = myFile.st_size;
    }
    
    char * fileBuffer = malloc(fileSize * sizeof(char));
    int currentSize;
    
    //This is the logic for the letter counter
    //The split number in this algorithm stands for the number of letters in the
    //array to be read in.
    //This loads the entire contents of the file into the file buffer.
    if (spec->usr_data == NULL) {  
        i = 0;
        while (temp != EOF) {
            temp = fgetc(fptr);
            fileBuffer[i] = temp;
            i++;
        }
        for (i = 0; i < fileSize; i++) {
            //printf("%c", fileBuffer[i]);
        }
        fclose(fptr);
        //printf("\n");
        fd = open(spec->input_data_filepath, O_RDONLY);
        approxSplit = fileSize/spec->split_num; //The ideal split size, assumes all split sizes are equal
        int extra;
        currentSize = 0;
        //printf("Approxsplit = %d\n", approxSplit);
        /*
            This algorithm determines the splitting for all but the last split,
            it first checks if the"i+1th" character in the file is a space. If
            it is not, it iterates through the file until it finds one. It keeps
            track of how many times it had to iterate an additional character to
            find a space. Then it makes the size of the array to hold the data
            the size of the ideal split size + any extra characters.
            If it is a space, it simply makes the size of the array to hold the
            data the ideal split size.
            Temp stands for temporary character, it's the current character being examined.
        */
        for(i = 0; i < spec->split_num-1; i++) {
            //Determine if this is a good split position
            if (!isspace(fileBuffer[approxSplit * (i+1)])) {
                //printf("Loop %d needs extra space\n", i);
                extra = 1;
                //Don't need extra seek because of this part, it increments the position indicator
                temp = fileBuffer[(approxSplit * (i+1))+extra];
                while (!isspace(temp)) {
                    extra++;
                    temp = fileBuffer[(approxSplit * (i+1))+extra];
                }
                //printf("Extra = %d\n", extra);
                dataSplits[i].fd = open(spec->input_data_filepath, O_RDONLY);
                dataSplits[i].size = approxSplit+extra;
                if (i == 0) {
                    lseek(dataSplits[i].fd, 0, SEEK_SET);
                } else {
                    lseek(dataSplits[i].fd, (i-1)*approxSplit+dataSplits[i].size, SEEK_SET);
                }
                dataSplits[i].usr_data = spec->usr_data;
            } else {
                //printf("Loop %d does not need extra space\n", i);
                dataSplits[i].fd = open(spec->input_data_filepath, O_RDONLY);
                dataSplits[i].size = approxSplit;
                if (i == 0) {
                    lseek(dataSplits[i].fd, 0, SEEK_SET);
                } else {
                    lseek(dataSplits[i].fd, (i-1)*approxSplit+dataSplits[i].size, SEEK_SET);
                }
                dataSplits[i].usr_data = spec->usr_data;
                lseek(fd, approxSplit, SEEK_CUR);
            }
            //printf("Datasplits[%d].size = %d\n", i, dataSplits[i].size);
            //Keeps track of the file size
            currentSize = currentSize + dataSplits[i].size;
        }
        /*
            This is for the final data split, it reads in the remaining characters
            left in the file.
        */
        dataSplits[spec->split_num-1].fd = open(spec->input_data_filepath, O_RDONLY);
        dataSplits[spec->split_num-1].size = fileSize - currentSize;
        lseek(dataSplits[spec->split_num-1].fd, -(dataSplits[spec->split_num-1].size), SEEK_END);
        dataSplits[spec->split_num-1].usr_data = spec->usr_data;
        //printf("Datasplits[%d].size = %d\n", i, dataSplits[i].size);
        //printf("Filesize = %d\n", fileSize);
        close(fd);
    } else {
    //This is the logic for the word counter/finder
    //The split number in this algorithm stands for the number of lines in the
    //array to be read in.
        currentSize = 0;
        int numLines = 1;
        i = 0;
        int n;
        int index;
        while (temp != EOF) {
            temp = fgetc(fptr);
            fileBuffer[i] = temp;
            i++;
            if (temp == '\n') {
                numLines++;
            }
        }
        fclose(fptr);
        fd = open(spec->input_data_filepath, O_RDONLY);
        approxSplit = numLines/spec->split_num;
        /*
            Unlike the letter counter above, since the split number is by lines,
            it simply counts the number of lines.
        */
        for(i = 0; i < spec->split_num-1; i++) {
            dataSplits[i].fd = fd;
            dataSplits[i].size = approxSplit;
            counter = 0;
            if (i == 0) {
                lseek(dataSplits[i].fd, 0, SEEK_SET);
            } else {
                index = 0;
                for (n = 0; n < fileSize; n++) {
                    if (fileBuffer[n] == '\n') {
                        //printf("Index = %d\n", n);
                        index = n;
                        counter++;
                    }
                    if (counter == i) {
                        break;
                    }
                }
                index++;
                //printf("Split position = %d\n", index);
                lseek(dataSplits[i].fd, index, SEEK_SET);
            }
            dataSplits[i].usr_data = spec->usr_data;
            currentSize = currentSize + dataSplits[i].size;
        }
        index = 0;
        counter = 0;
        if (spec->split_num != 1) {
            for (n = 0; n < fileSize; n++) {
                if (fileBuffer[n] == '\n') {
                    //printf("Index = %d\n", n);
                    index = n;
                    counter++;
                }
                if (counter == i) {
                    break;
                }
            }
        }
        index++;
        dataSplits[spec->split_num-1].fd = open(spec->input_data_filepath, O_RDONLY);
        dataSplits[spec->split_num-1].size = fileSize - currentSize;
        lseek(dataSplits[spec->split_num-1].fd, index, SEEK_SET);
        dataSplits[spec->split_num-1].usr_data = spec->usr_data;
    }
    
    //Creates the intermediate files and opens them to work with.
    for (i = 0; i < spec->split_num; i++) {
        snprintf(buffer, bufSize, "mr-%d.itm", i);
        p_fd_in[i] = open(buffer, O_RDWR | O_CREAT | O_TRUNC, 0777);
        if (p_fd_in[i] > 0) {
            //printf("Opening: p_fd_in[%d] = %d, with name %s and bufSize %d\n", i, p_fd_in[i], buffer, bufSize);
        } else {
            EXIT_ERROR(ERROR, "Exiting with errno %d\n", errno);
        }
    }
    
    int fd_out = open(result->filepath, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    
    //Pipes : child to parent
    int pfds[2];
    if (pipe(pfds) == -1) {
	      EXIT_ERROR(ERROR, "Pipe creation failed!\n");
	}
	
	int pcp[2];
    if (pipe(pcp) == -1) {
	      EXIT_ERROR(ERROR, "Pipe creation failed!\n");
	}
	
	int isLeft = 0;
	int pass;

    //printf("PARENT ALIVE: Pid of %d\n", getpid());
    
    //Splits, and makes a child process to do some work in.
    for (i = 0; i < spec->split_num; i++) {
        if ((pid = fork()) < 0) {
            EXIT_ERROR(ERROR, "Fork failed!\n");
		} else if (pid == 0) {
			//CHILD
			/*  Sets doesMerge on the first child to 1, indicates the first child
			    only will do the merging and no other. The other worker pids are
			    put into the array of the pids.
			*/
			if (i == 0) {
				doesMerge = 1;
			}
			counter = i;
		    result->map_worker_pid[i] = getpid();
			break;
		} else {
			//PARENT
		}
    }
    
    if(pid == 0) {
        close(pfds[0]);
        pass = getpid();
        //printf("CHILD ALIVE: Pid of %d\n", getpid());
        if(write(pfds[1], &pass, sizeof(pass)) <= 0) {
		}
    } else {
        numRead = 0;
        close(pfds[1]);
        while (numRead != spec->split_num) {
            if(read(pfds[0], &pass, sizeof(pass))) {
    	}
    		//printf("When numRead = %d, Pass value = %d\n", numRead, pass);
    		result->map_worker_pid[numRead] = pass;
    		if (numRead == 0) {
    		    result->reduce_worker_pid = pass;
    		}
    		numRead++;
        }
    }
    
    if (pid == 0) {
        //Childs go to the map function
        status = spec->map_func(&dataSplits[counter], p_fd_in[counter]);
        if (status == -1) {
            EXIT_ERROR(ERROR, "Map Function failed at pid %d\n", getpid());
        }
    } else {
        //Parent just waits for the children to finish
        
        if (spec->split_num > 1) {
            counter = spec->split_num;
            //printf("Counter to begin with: %d\n", counter);
            while ((pid = waitpid(-1, NULL, 0))) {
                //printf("Counter before = %d\n", counter);
                counter--;
                if (counter == 1) {
                    //printf("About to Exit first loop in pid %d\n", getpid());
                    break;
                }
                //printf("Counter after = %d\n", counter);
            }
            //printf("Parent %d finished looping\n", getpid());
        }
    }
    
    if (doesMerge == 1) {
        //The first child pid waits for other processes to finish before consolidating results
        //printf("\nWaiting to Merge!\n");
        close(pcp[1]);
        if (spec->split_num == 1) {
            isLeft = 1;
        }
        while (isLeft == 0) {
            if (poll(&(struct pollfd){ .fd = pcp[0], .events = POLLIN }, 1, 0)==1) {
        		//printf("Value of isLeft before read = %d\n", isLeft);
                if(read(pcp[0], &pass, sizeof(pass))) {
        		}
        		isLeft = pass;
        		//printf("Value of isLeft after read = %d\n", isLeft);
            }
        }
        status = spec->reduce_func(p_fd_in, spec->split_num, fd_out);
        if (status == -1) {
            EXIT_ERROR(ERROR, "Reduce Function failed\n");
        }
    } else if (pid > 0) {
        //Wait for reduce function to work
        
        close(pcp[0]);
        pass = 1;
        if(write(pcp[1], &pass, sizeof(pass)) <= 0) {
		}
		
		counter = 1;
		//printf("Counter At start of loop = %d\n", counter);
        while ((pid = wait(&status)) > 0) {
            if (errno == ECHILD) {
                break;
            }
        }
        //sleep(3);
   }
   
   //printf("About to finish processing time in pid %d\n", getpid());
   
    gettimeofday(&end, NULL);   

    result->processing_time = (end.tv_sec - start.tv_sec) * US_PER_SEC + (end.tv_usec - start.tv_usec);
    
    
    //If you free the commented out things, the result it spits out is inaccurate.
    //free(result->map_worker_pid);
    //free(dataSplits);
    //free(p_fd_in);
    //free(fileBuffer);
    for (i = 0; i < spec->split_num; i++) {
        close(p_fd_in[i]);
    }
    close(fd_out);
    
    if (pid == 0) {
        //printf("Process %d terminated\n", getpid());
        exit(0);
    }
    
    //printf("Process %d terminated\n", getpid());
}
