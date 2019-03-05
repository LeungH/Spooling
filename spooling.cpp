#include"spooling.h"
void request(int i)
{
	int j, sum = 0;
	if (c3 == 0)//û���������飬��Ӧ���̵ȴ�
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
		name = File[i] - T[i] + sum;//�ļ���ŵ���ǰ�������ռ�ļ�����������϶�Ӧ�����ѽ���������ļ�����
		int j = rand() % 10;//����0~9�����
		int length = 0;//���ȼ�����
		while (j > 0 && length<20)//���ǽ��������ҳ���С��20
		{
			buffer[n_in][length] = j;//һ�����������Ӧһ��buffer����
			length++;
			j = rand() % 10;
		}
		req[n_in].filename = name;
		req[n_in].reqname = i;
		req[n_in].length = length;
		printf("д�������\n");
		printf("����������ļ���Ϊ��%d ��������Ľ�����Ϊ��%d ��������ĳ���Ϊ��%d\n", name, i, length);
		if (p[pcb_num].status == 2)//����spooling����
			p[pcb_num].status = 0;
		T[i]--;//��Ӧ����δ����������ļ���һ
		c3--;//���������һ
		n_in = (n_in + 1) % reqblock_num;//����������ָ���һ
		scheduling();//ת�����̵���
	}
}
void spooling()
{
	int i;
	if (c3 == reqblock_num)//��������δ��ռ��
	{
		int flag = 0;
		for (i = 0; i < pcb_num; i++)//�Ƿ��н���δ�������ǵĻ�flag=1
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
		}//����ȴ�״̬
		else
		{
			printf("spooling���̽���\n");
			p[pcb_num].status = 3;
		}//spooling���̽���
		scheduling();
	}
	else
	{
		fopen_s(&f, "output.txt", "a");
		if (f == NULL)
		{
			printf("�ļ���ʧ��!\n");
			return;
		}
		else//��reqblock��Ӧ�������������������ļ�
		{
			printf("���д�ӡ\n");
			printf("�ļ�����%d\n", req[n_out].filename);
			fprintf(f, "�ļ�����%d\n", req[n_out].filename);
			printf("����Ϊ��");
			fprintf(f, "����Ϊ��");
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
		if (t[i] == File[i])//��Ӧ��������ļ���������Ҫ����ļ���
		{
			p[i].status = 3;
			printf("�û�����%d��ӡ���\n", i);
		}
		req[n_out].filename = NULL;
		req[n_out].length = NULL;
		req[n_out].reqname = NULL;//�ͷ���������
		c3++;//ʣ�����������һ
		n_out = (n_out + 1) % reqblock_num;
		for (i = 0; i < pcb_num; i++)//���ѵȴ����û�����
		{
			if (p[i].status == 1)
			{
				p[i].status = 0;
			}
		}
		scheduling();//ת�����̵���

	}
}
void scheduling()
{
	int i = rand() % 1000;//���������
	int jiange = 800 / pcb_num;//�û����̵ĸ��ʵ���80%�����û���������
	int arer = 0;
	int s = i;
	while (s - jiange > 0)//��������������һ����Χ��arer��ʾ����
	{
		s = s - jiange;
		arer++;
	}
	if (i>800 && p[pcb_num].status == 0)//����spooling�������
	{
		printf("����spooling�������\n");
		spooling();
	}
	else if (i<800 && T[arer] != 0 && p[arer].status == 0)//��������������
	{
		printf("�����û�����%d\n", arer);
		request(arer);
	}
	else if (p[pcb_num].status != 3)//spooling����δ�������������̵���
	{
		scheduling();
	}
	else//����
	{
		system("PAUSE");
	}
}
void main()
{
	int i;
	srand((int)time(NULL));
	printf("�������û���������");
	scanf_s("%d", &pcb_num);
	printf("���������������������");
	scanf_s("%d", &reqblock_num);
	c3 = reqblock_num;
	t = new int[pcb_num];
	T = new int[pcb_num];
	File = new int[pcb_num];
	p = new pcb[pcb_num];
	req = new reqblock[reqblock_num];
	for (i = 0; i < pcb_num; i++)//�û����̳�ʼ��
	{
		t[i] = 0;
		File[i] = rand() % 20;//��Ҫ������ļ�����Χ��0~19
		T[i] = File[i];
		p[i].id = i;
		p[i].status = 0;
		printf("����%d��Ҫ������ļ���Ϊ��%d\n", i, File[i]);
	}
	p[pcb_num].id = i;
	p[pcb_num].status = 0;//spooling���̳�ʼ��
	buffer = new int *[reqblock_num];     //��ά���鶯̬�����ڴ�
	for (int i = 0; i <reqblock_num; i++)
	{
		buffer[i] = new int[30];
	}
	n_in = 0;
	n_out = 0;
	fopen_s(&f, "output.txt", "w");//�������������ļ�
	fclose(f);
	scheduling();//ת�����̵���
}
