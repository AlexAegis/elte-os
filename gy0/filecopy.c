//make a copy of file given in argv[1] to file given in argv[2]
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>     //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

struct id
{
    char *name;
    int number;
};

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        perror("You have to use program with one argument, the file names write_to");
        exit(1);
    }
    int f;

    if (f < 0)
    {
        perror("Error at opening the file\n");
        exit(1);
    }
    //There is errno variable in which there is the number of error --
    //if (errno!=0) there is an error

    f = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    id toBeWritten;
    toBeWritten.name =

        char c;

    //read gives back the number of bytes
    //1. parameter the file descriptor
    //2. parameter the address of variable, we read into
    //3. parameter the number of bytes we want to read in
    printf("%c", c); //we prints out the content of the file on the screen
    if (write(f, &c, sizeof(c)) != sizeof(c))
    {
        perror("There is a mistake in writing\n");
        exit(1);
    }
    //write gives back the number of written bytes
    //1. parameter the file descriptor
    //2. parameter the address of variable we want to write out
    //3. parameter the number of bytes we want to write out
    close(f);
    return 0;
}
