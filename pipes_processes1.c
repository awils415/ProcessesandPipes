// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu"; 
    char fixed_str2[] = "gobison.org";
    char input_str[100]; 
    char input_str2[100];
    char concat_str[100];
    pid_t p; 
  
    if (pipe(fd1) == -1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2) == -1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate: ");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
        //char concat_str[100];
 
        close(fd1[0]); // Close reading end of first pipe
 
        // Write input string and close writing end of first
        // pipe.
        write(fd1[1], input_str, strlen(input_str) + 1);
        close(fd1[1]);
 
        // Wait for child to send a string
        wait(NULL);
 
        //close(fd2[1]); // Close writing end of second pipe
 
        // Read string from child, print it and close
        // reading end.
        read(fd2[0], concat_str, 100);
        printf("Concatenated string %s\n", concat_str);
        close(fd2[0]);

        // Close writing end of child
        close(fd2[1]);

        wait(NULL);
        
        // Read second string from child, concatenate second input
        // to string, print, then close reading end
        read(fd2[0], concat_str, 100);

        int x = strlen(concat_str);
        int j;
        for (j = 0; j < strlen(fixed_str2); j++)
            concat_str[x++] = fixed_str2[j];

        concat_str[x] = '\0';

        close(fd1[0]);
        close(fd2[0]);

        write(fd1[1], concat_str, strlen(concat_str) + 1);
        close(fd1[1]);

        printf("New concatenated string: %s\n", concat_str);

        exit(0);
    } 
  
    // child process 
    else
    { 
        close(fd1[1]); // Close writing end of first pipe
 
        // Read a string using first pipe
        //char concat_str[100];
        read(fd1[0], concat_str, 100);
 
        // Concatenate a fixed string with it
        int k = strlen(concat_str);
        int i;
        for (i = 0; i < strlen(fixed_str); i++)
            concat_str[k++] = fixed_str[i];
 
        concat_str[k] = '\0'; // string ends with '\0'
 
        // Close both reading ends
        close(fd1[0]);
        close(fd2[0]);
 
        // Write concatenated string and close writing end
        write(fd2[1], concat_str, strlen(concat_str) + 1);
        close(fd2[1]);

        wait(NULL);

        // Prompt for second string
        printf("Enter a second string to concatenate: ");
        scanf("%s", input_str2); 
        //printf("%s", input_str2);

        // Write second input and close writing end
        write(fd2[1], input_str2, strlen(input_str2) + 1);
        close(fd2[1]);

        //printf("String to be concatenated: %s\n", input_str2);

        exit(0);
    } 
} 