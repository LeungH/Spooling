#include<stdio.h>
#include<stdlib.h>
#include<time.h>
typedef struct pcb
{
	int id;
	int status;
}pcb;//���̿��ƿ�
typedef struct reqblock
{
	int filename;
	int reqname;
	int length;
}reqblock;//��������
int *t, n_in, n_out, c3, *T, *File,pcb_num,reqblock_num;
int **buffer;//�����
FILE *f;
pcb *p;
reqblock *req;
void request(int i);//����������request
void spooling(pcb p[]);//spooling�������
void scheduling();//���̵��ȳ���