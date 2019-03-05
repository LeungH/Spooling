#include"spooling.h"
void request(int i)
{
	int j, sum = 0;
	if (c3 == 0)//没有输出请求块，相应进程等待
	{
		p[i].status = 1;
		scheduling();
	}
	else
	{
		int name;
		for (j = 0; j < i; j++)
		{
			sum += File[j];
		}
		name = File[i] - T[i] + sum;//文件编号等于前面进程所占文件编号总数加上对应进程已进入输出井文件数量
		int j = rand() % 10;//产生0~9随机数
		int length = 0;//长度计数器
		while (j > 0 && length<20)//不是结束符并且长度小于20
		{
			buffer[n_in][length] = j;//一个输出请求块对应一行buffer数组
			length++;
			j = rand() % 10;
		}
		req[n_in].filename = name;
		req[n_in].reqname = i;
		req[n_in].length = length;
		printf("写入输出井\n");
		printf("输出请求块的文件名为：%d 输出请求块的进程名为：%d 输出请求块的长度为：%d\n", name, i, length);
		if (p[pcb_num].status == 2)//唤醒spooling进程
			p[pcb_num].status = 0;
		T[i]--;//对应进程未进入输出井文件减一
		c3--;//输出请求块减一
		n_in = (n_in + 1) % reqblock_num;//输出请求块送指针加一
		scheduling();//转到进程调度
	}
}
void spooling()
{
	int i;
	if (c3 == reqblock_num)//输出请求块未被占用
	{
		int flag = 0;
		for (i = 0; i < pcb_num; i++)//是否还有进程未结束，是的话flag=1
		{
			if (p[i].status != 3)
			{
				flag = 1;
				break;
			}
		}
		if (flag == 1)
		{
			p[pcb_num].status = 2;
		}//进入等待状态
		else
		{
			printf("spooling进程结束\n");
			p[pcb_num].status = 3;
		}//spooling进程结束
		scheduling();
	}
	else
	{
		fopen_s(&f, "output.txt", "a");
		if (f == NULL)
		{
			printf("文件打开失败!\n");
			return;
		}
		else//把reqblock对应的输出井的内容输出到文件
		{
			printf("进行打印\n");
			printf("文件名：%d\n", req[n_out].filename);
			fprintf(f, "文件名：%d\n", req[n_out].filename);
			printf("内容为：");
			fprintf(f, "内容为：");
			for (i = 0; i < req[n_out].length; i++)
			{
				printf("%d ", buffer[n_out][i]);
				fprintf(f, "%d ", buffer[n_out][i]);
			}
			printf("\n");
			fprintf(f, "\n");
		}
		fclose(f);
		i = req[n_out].reqname;
		t[i]++;
		if (t[i] == File[i])//对应进程输出文件数等于需要输出文件数
		{
			p[i].status = 3;
			printf("用户进程%d打印完毕\n", i);
		}
		req[n_out].filename = NULL;
		req[n_out].length = NULL;
		req[n_out].reqname = NULL;//释放输出请求块
		c3++;//剩余输出请求块加一
		n_out = (n_out + 1) % reqblock_num;
		for (i = 0; i < pcb_num; i++)//唤醒等待的用户进程
		{
			if (p[i].status == 1)
			{
				p[i].status = 0;
			}
		}
		scheduling();//转到进程调度

	}
}
void scheduling()
{
	int i = rand() % 1000;//产生随机数
	int jiange = 800 / pcb_num;//用户进程的概率等于80%除以用户进程数量
	int arer = 0;
	int s = i;
	while (s - jiange > 0)//计算出随机数在哪一个范围，arer表示区域
	{
		s = s - jiange;
		arer++;
	}
	if (i>800 && p[pcb_num].status == 0)//调用spooling输出程序
	{
		printf("调用spooling输出程序\n");
		spooling();
	}
	else if (i<800 && T[arer] != 0 && p[arer].status == 0)//调用输出服务程序
	{
		printf("调用用户进程%d\n", arer);
		request(arer);
	}
	else if (p[pcb_num].status != 3)//spooling进程未结束，继续进程调度
	{
		scheduling();
	}
	else//结束
	{
		system("PAUSE");
	}
}
void main()
{
	int i;
	srand((int)time(NULL));
	printf("请输入用户进程数：");
	scanf_s("%d", &pcb_num);
	printf("请输入输出请求块的数量：");
	scanf_s("%d", &reqblock_num);
	c3 = reqblock_num;
	t = new int[pcb_num];
	T = new int[pcb_num];
	File = new int[pcb_num];
	p = new pcb[pcb_num];
	req = new reqblock[reqblock_num];
	for (i = 0; i < pcb_num; i++)//用户进程初始化
	{
		t[i] = 0;
		File[i] = rand() % 20;//需要输出的文件数范围是0~19
		T[i] = File[i];
		p[i].id = i;
		p[i].status = 0;
		printf("进程%d需要输出的文件数为：%d\n", i, File[i]);
	}
	p[pcb_num].id = i;
	p[pcb_num].status = 0;//spooling进程初始化
	buffer = new int *[reqblock_num];     //二维数组动态分配内存
	for (int i = 0; i <reqblock_num; i++)
	{
		buffer[i] = new int[30];
	}
	n_in = 0;
	n_out = 0;
	fopen_s(&f, "output.txt", "w");//创建或清空输出文件
	fclose(f);
	scheduling();//转到进程调度
}
