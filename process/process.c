#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/wait.h>
#define HZ	100

void cpuio_bound(int last, int cpu_time, int io_time);



/*
 * 此函数按照参数占用CPU和I/O时间
 * last: 函数实际占用CPU和I/O的总时间，不含在就绪队列中的时间，>=0是必须的
 * cpu_time: 一次连续占用CPU的时间，>=0是必须的
 * io_time: 一次I/O消耗的时间，>=0是必须的
 * 如果last > cpu_time + io_time，则往复多次占用CPU和I/O
 * 所有时间的单位为秒
 */
void cpuio_bound(int last, int cpu_time, int io_time)
{
	struct tms start_time, current_time;
	clock_t utime, stime;
	int sleep_time;

	while (last > 0)
	{
		/* CPU Burst */
		times(&start_time);
		/* 其实只有t.tms_utime才是真正的CPU时间。但我们是在模拟一个
		 * 只在用户状态运行的CPU大户，就像“for(;;);”。所以把t.tms_stime
		 * 加上很合理。*/
		do
		{
			times(&current_time);
			utime = current_time.tms_utime - start_time.tms_utime;
			stime = current_time.tms_stime - start_time.tms_stime;
		} while ( ( (utime + stime) / HZ )  < cpu_time );
		last -= cpu_time;

		if (last <= 0 )
			break;

		/* IO Burst */
		/* 用sleep(1)模拟1秒钟的I/O操作 */
		sleep_time=0;
		while (sleep_time < io_time)
		{
			sleep(1);
			sleep_time++;
		}
		last -= sleep_time;
	}
}
void make_some_forks(){
    int p1 = fork();
    if(p1 == 0){
        cpuio_bound(10,1,1);
        return;
    }
    int p2 = fork();
    if(p2 == 0){
        cpuio_bound(10,2,1);
        return ;
    }
    int p3 = fork();
    if(p3 == 0){
        cpuio_bound(10,1,2);
        return ;
    }
    for(int i = 0;i<3;i++){
        wait(NULL);
        printf("finish %d\n",i);
    }
    return ;
}
int main(){
    make_some_forks();
    return 0;
};
//使用top观察发现：
//  40392 root      20   0    2648     92      0 R  66.8   0.0   0:24.44 ./a.out                                                                          
//   40391 root      20   0    2648     92      0 R  47.8   0.0   0:18.44 ./a.out                                                                          
//   40393 root      20   0    2648     92      0 R  33.6   0.0   0:12.43 ./a.out   
// 其中浮点是cpu占用率？可以看到基本符合我们的设置
//完毕