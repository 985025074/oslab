#define __NR_whoami		72
#define __NR_iam		73
#include <linux/kernel.h>
char store_name[100];
extern inline unsigned char get_fs_byte(const char * addr)
{
    unsigned register char _v;
    __asm__ ("movb %%fs:%1,%0":"=r" (_v):"m" (*addr));
    return _v;
}
extern int printk(const char *fmt, ...);
int sys_iam(const char* name){
    int now_loc =0;
    while(1){
        store_name[now_loc] = get_fs_byte(name+now_loc);
        if(store_name[now_loc]=='\0'){
            break;
        }
        now_loc++;
    }
    return 0;
}
int sys_whoami(const char* name,unsigned int size){
   int now_loc = 0 ;
   if(store_name[0]=='\0'){
    printk("You haven't set your name yet!\n");
    return 0;
   }
    printk("Your name is %s\n",store_name);
    return 0;
}
