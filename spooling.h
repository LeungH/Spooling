#include<stdio.h>
#include<stdlib.h>
#include<time.h>
typedef struct pcb
{
	int id;
	int status;
}pcb;//进程控制块
typedef struct reqblock
{
	int filename;
	int reqname;
	int length;
}reqblock;//输出请求块
int *t, n_in, n_out, c3, *T, *File,pcb_num,reqblock_num;
int **buffer;//输出井
FILE *f;
pcb *p;
reqblock *req;
void request(int i);//输出服务程序request
void spooling(pcb p[]);//spooling输出程序
void scheduling();//进程调度程序