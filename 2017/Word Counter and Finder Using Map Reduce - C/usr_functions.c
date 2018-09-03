#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "common.h"
#include "usr_functions.h"
#include "mapreduce.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

#include <errno.h>
#include <sys/wait.h>

/* User-defined map function for the "Letter counter" task.  
   This map function is called in a map worker process.
   @param split: The data split that the map function is going to work on.
                 Note that the file offset of the file descripter split->fd should be set to the properly
                 position when this map function is called.
   @param fd_out: The file descriptor of the itermediate data file output by the map function.
   @ret: 0 on success, -1 on error.
 */
int letter_counter_map(DATA_SPLIT * split, int fd_out)
{
    // My Code
    
    int letters[26];
    int i;
    for (i = 0; i < 26; i++) {
        letters[i] = 0;
    }
    
    //printf("PID %d with Split Size = %d\n", getpid(), split->size);
    //printf("Process %d Processing Start\n", getpid());
    char temp;
    int index;
    int fd = split->fd;
    char buffer[2];
    /*
        temp takes in the first character in the buffer, makes it lowercase, checks to see if it's a letter.
        If it's a letter it increments the count associated with that letter.
    */
    for (i = 0; i < split->size; i++) {
        //printf("Process %d: Loop %d\n", getpid(), i);
        read(fd, buffer, 1);
        temp = buffer[0];
        temp = tolower(temp);
        //printf("%c", temp);
        index = (int)temp - (int)'a';
        if (index >= 0 && index < 26) {
            letters[index]++;
        }
    }
    /*
    printf("\n");
    for (i = 0; i < 26; i++) {
        printf("%d\n", letters[i]);
    }
    */
    
    //temp converted to new line character after processing the data is over and ready for writing.
    temp = '\n';
    
    //int orig;
    int tempInt;
    int count = 1;
    buffer[1] = ' ';
    char intBuf[100];
    //printf("Process %d Writing\n", getpid());
    //Output to file: letters array contents, one index per line
    //Outputs: CAPITAL LETTER #
    for(i = 0; i < 26; i++) {
        //printf("Process %d: Loop %d\n", getpid(), i);
        count = 0;
        tempInt = letters[i];
        if (tempInt == 0) {
            count = 1;
        } else {
            while (tempInt != 0) {
                tempInt = tempInt/10;
                count++;
            }
        }
        sprintf(intBuf, "%d", letters[i]);
	    buffer[0] = 65 + i; //65 is ASCII for A. Adding i to it when i = 0 will print out A
        write(fd_out, buffer, sizeof(buffer)/sizeof(buffer[0]));
        write(fd_out, intBuf, count);
        if (i < 25) {
            write(fd_out, &temp, 1);
        }
    }
    //printf("Process %d Exiting\n", getpid());
    return 0;
}

/* User-defined reduce function for the "Letter counter" task.  
   This reduce function is called in a reduce worker process.
   @param p_fd_in: The address of the buffer holding the intermediate data files' file descriptors.
                   The imtermeidate data files are output by the map worker processes, and they
                   are the input for the reduce worker process.
   @param fd_in_num: The number of the intermediate files.
   @param fd_out: The file descriptor of the final result file.
   @ret: 0 on success, -1 on error.
   @example: if fd_in_num == 3, then there are 3 intermediate files, whose file descriptor is 
             identified by p_fd_in[0], p_fd_in[1], and p_fd_in[2] respectively.

*/
int letter_counter_reduce(int * p_fd_in, int fd_in_num, int fd_out)
{
    // My code ...
    //ASCII A = 65
    //ASCII a = 97
    
    //read in intermediate data file
    int letters[26];
    int i;
    char buffer[100];
    
    for (i = 0; i < 26; i++) {
        letters[i] = 0;
    }
    int n;
    int g;
    int tempInt;
    int count; //keeps track of what letter of the alphabet is being processed.
    char readBuf[2];
    char temp = 'A';
    int isEnd = 1;
    for (i = 0; i < fd_in_num; i++) {
        lseek(p_fd_in[i], 0, SEEK_SET);
        for (n = 0; n < 26; n++) {
            count = 0;
            isEnd = read(p_fd_in[i], readBuf, 2);
            if (isEnd == 0) {
                //Incorrect file.
                printf("File Descriptor %d was incorrect\n", p_fd_in[i]);
                return -1;
            }
            temp = 'A';
            //printf("%s", readBuf);
            
            //puts the number currently being worked on into the char buffer
            while(temp != '\n' && isEnd != 0) {
                isEnd = read(p_fd_in[i], readBuf, 1);
                temp = readBuf[0];
                //printf("%c", temp);
                if (temp != '\n' && isEnd != 0){
                    buffer[count] = temp;
                    count++;
                }
            }
            
            //makes the buffer contents an integer, adds the count to the total sum
            //printf("%s %d\n", buffer, count);
            tempInt = atoi(buffer);
            //printf("Temp Int = %d\n", tempInt);
            letters[n] = letters[n] + tempInt;
            
            for (g = 0; g < count; g++) {
                buffer[g] = 'A';
            }
        }
    }
    /*
    for (i = 0; i < 26; i++) {
        printf("%d\n", letters[i]);
    }*/
    
    //int orig;
    //count = 1;
    char intBuf[100];
    char writeBuf[2];
    writeBuf[1] = ' ';
    temp = '\n';
    //Write to file here
    for(i = 0; i < 26; i++) {
        //printf("Process %d: Loop %d\n", getpid(), i);
        count = 0;
        tempInt = letters[i];
        if (tempInt == 0) {
            count = 1;
        } else {
            while (tempInt != 0) {
                tempInt = tempInt/10;
                count++;
            }
        }
        sprintf(intBuf, "%d", letters[i]);
	    writeBuf[0] = 65 + i;
        write(fd_out, writeBuf, sizeof(writeBuf)/sizeof(writeBuf[0]));
        write(fd_out, intBuf, count);
        if (i < 25) {
            write(fd_out, &temp, 1);
        }
    }
    return 0;
}

/* User-defined map function for the "Word finder" task.  
   This map function is called in a map worker process.
   @param split: The data split that the map function is going to work on.
                 Note that the file offset of the file descripter split->fd should be set to the properly
                 position when this map function is called.
   @param fd_out: The file descriptor of the itermediate data file output by the map function.
   @ret: 0 on success, -1 on error.
 */
int word_finder_map(DATA_SPLIT * split, int fd_out)
{

    // add your implementation here ...
    int i;
    char * isGood;
    //100 should be mallocable
    char * buffer = malloc(100 * sizeof(char));
    char * ptr;
    FILE * fptr = fdopen(split->fd, "r");
    if (fptr == NULL) {
        printf("Could not open input file\n");
        return -1;
    }
    FILE * fOut = fdopen(fd_out, "w");
    if (fOut == NULL) {
        printf("Could not open output file\n");
        return -1;
    }
    
    /*
        Reads in contents of the data split (one line at a time) and puts it in the buffer.
        Then looks to see if the word to be searched for is in the line.
        If it is then it prints the line to the file.
    */
    for (i = 0; i < split->size; i++) {
        isGood = fgets(buffer, 100, fptr); //Checks to make sure the line is valid
        if (isGood != NULL) {
            //printf("PID %d buffer: %s\n", getpid(), buffer);
            ptr = strstr(buffer, (char *)split->usr_data);
            if (ptr != NULL) {
                fprintf(fOut, "%s", buffer);
            }
        }
    }
    if(fclose(fptr) != 0) {
        printf("Error closing input file\n");
        return -1;
    }
    if (fclose(fOut) != 0) {
        printf("Error closing output file\n");
        return -1;
    }
    free(buffer);
    //printf("fd_out = %d\n", fd_out);
    return 0;
}

/* User-defined reduce function for the "Word finder" task.  
   This reduce function is called in a reduce worker process.
   @param p_fd_in: The address of the buffer holding the intermediate data files' file descriptors.
                   The imtermeidate data files are output by the map worker processes, and they
                   are the input for the reduce worker process.
   @param fd_in_num: The number of the intermediate files.
   @param fd_out: The file descriptor of the final result file.
   @ret: 0 on success, -1 on error.
   @example: if fd_in_num == 3, then there are 3 intermediate files, whose file descriptor is 
             identified by p_fd_in[0], p_fd_in[1], and p_fd_in[2] respectively.

*/
int word_finder_reduce(int * p_fd_in, int fd_in_num, int fd_out)
{
    //printf("In Word finder reduce function\n");    
    // My Code here ...
    int i;
    FILE * working_file;
    FILE * fOut = fdopen(fd_out, "w");
    char * isGood;
    i = (fd_in_num/10)+9;
    char * fbuffer = malloc(i * sizeof(char));
    int bufSize = i/sizeof(fbuffer[0]);
    //printf("bufSize = %d\n", bufSize);
    if (fOut == NULL) {
        printf("Could not open output file\n");
        return -1;
    }
    char * buffer = malloc(100 * sizeof(char));
    for (i = 0; i < fd_in_num; i++) {
        //printf("p_fd_in[%d] = %d\n", i, p_fd_in[i]);
        snprintf(fbuffer, bufSize, "mr-%d.itm", i);
        p_fd_in[i] = open(fbuffer, O_RDWR, 0777);
        lseek(p_fd_in[i], 0, SEEK_SET);
        working_file = fdopen(p_fd_in[i], "r");
        if (working_file == NULL) {
            printf("Error opening file p_fd_in[%d]: errno = %d\n", i, errno);
            return -1;
        }
        //Spit out the contents of p_fd_in[i] to output file in order
        isGood = fgets(buffer, 100, working_file);
        while (isGood != NULL) {
            //printf("buffer: %s", buffer);
            //write buffer to file
            fprintf(fOut, "%s", buffer);
            isGood = fgets(buffer, 100, working_file);
        }
            if (fclose(working_file) != 0) {
        printf("Error closing current file\n");
        return -1;
    }
    }
    
    if (fclose(fOut) != 0) {
        printf("Error closing output file\n");
        return -1;
    }
    free(buffer);
    
    return 0;
}