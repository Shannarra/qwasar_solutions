#include <unistd.h> // read
#include <stdio.h>  // fprintf
#include <fcntl.h>  // open, O_RDONLY
#include <errno.h>  // errno
#include <string.h> // strerror

#define READLINE_READ_SIZE 500

char *my_readline(int fd) {

    if (READLINE_READ_SIZE <= 0)
    {
        fprintf(stderr, "READLINE_READ_SIZE must be bigger than 0!\n");
        return NULL;   
    }

    char buff[READLINE_READ_SIZE] = {0};

    if (read(fd, buff, READLINE_READ_SIZE) < 0) {
        fprintf(stderr, "Could not read file \"%d\": %s\n", fd, strerror(errno));
        return NULL;
    }
    static char rebuff[READLINE_READ_SIZE+1] = {0};

    for (int i = 0; i < READLINE_READ_SIZE; i++)
        if (buff[i] != EOF || buff[i] != '\n')
            rebuff[i] = buff[i];
        else break;

    return rebuff;    
}

int main() {
    char file[100] = {0};
    printf("Enter file name to read: ");
    scanf("%s", file);
    int fd = open(file, O_RDONLY);
    
    if (fd > 0)
        printf("CONTENT: %s\n", my_readline(fd));
    else 
        fprintf(stderr, "Could not open file \"%s\": %s\n", file, strerror(errno));

}