#include <iostream>
#include <cstdio>

int N;										//���Բɹ�������������,[1,100]
struct ServerInformation					//��������Ϣ
{
	char typeName[21];						//�������ͺ�
	int coreNum;							//CPU������������1024
	int memorySize;							//�ڴ��С,��λG��������1024G
	int hardwareCost;						//Ӳ���ɱ���������5e5
	int dayCost;							//ÿ�պ��ܳɱ���������5000
}severInformation[100];						//���Բɹ��ķ�����������Ϣ

int M;										//���������������,[1,1000]
struct VirtualMachineInformation
{
	char typeName[21];						//������ͺ�
	int coreNum;							//CPU����
	int memorySize;							//�ڴ��С
	int isDoubleNode;						//�Ƿ�˫�ڵ㲿����0Ϊ���ڵ㲿��1Ϊ˫�ڵ㲿��

	int coreNumNode;						//���ڵ�CPU����
	int memorySizeNode;						//���ڵ�CPU�ڴ�ռ��
}virtualMachineInformation[1000];			//������ͺ���Ϣ

int T;										//T���û�������������,[1,1000]
int requireNum[1000];						//��i+1���û�������Ŀ
int requireRank[1000];						//��i+1���û��������ʼ��ַ
struct Require
{
	int type;								//�������͡�0Ϊadd������1λdel����
	int virtualMachineNum;					//�������������ͱ��
	int id;									//�����ID
}require[100000];							//�û�����

struct VirtualMachine						//����ʹ�õ������
{
	int id;									//�����ID
	int type;								//������ͺ�
}virtualMachine[100000 + 10];

struct Sever								//�Ѿ�����ġ�ʹ���еķ�����
{
	int type;								//�������ͺű��

	int remainCoreNodeA;					//������A���ʣ���ں�
	int remainCoreNodeB;					//������B���ʣ���ں�
	int remainMemoryNodeA;					//������A���ʣ���ڴ�
	int remainMemoryNodeB;					//������B���ʣ���ڴ�

	int open;								//�������Ƿ�����
	int cost;								//�÷��������ۼƳɱ�

	VirtualMachine* vmLinkHead = nullptr;	//�÷������е�ǰ���ڵ������

	static int severNum;					//�Ѿ�����ķ���������

	void addVirtualMachine(int type, int id)//��������
	{

	}
	void delVirtualMachine(int id)			//ɾ�������
	{

	}
}sever[100000 + 10];						//sever[i] ���Ϊi�ķ�����

int severIDVM[1000000 + 10];

void addSever(int type)
{

}



int main() {


	return 0;
}
