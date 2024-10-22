
#include <unistd.h>

_syscall1(int, iam, const char*, name);

int main(int argc, char *argv[]){
    return iam(argv[0]);

}