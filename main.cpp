#include <iostream>
#include <cstdio>
#include <cstdlib>

int N;										//可以采购服务器的数量,[1,100]
struct ServerInformation					//服务器信息
{
	char typeName[21];						//服务器型号
	int coreNum;							//CPU核数，不超过1024
	int memorySize;							//内存大小,单位G，不超过1024G
	int hardwareCost;						//硬件成本，不超过5e5
	int dayCost;							//每日耗能成本，不超过5000
}severInformation[100];						//可以采购的服务器类型信息

int M;										//售买虚拟机的数量,[1,1000]
struct VirtualMachineInformation
{
	char typeName[21];						//虚拟机型号
	int coreNum;							//CPU核数
	int memorySize;							//内存大小
	int isDoubleNode;						//是否双节点部属，0为单节点部署，1为双节点部属

	int coreNumNode;						//单节点CPU核数
	int memorySizeNode;						//单节点CPU内存占用
}virtualMachineInformation[1000];			//虚拟机型号信息

int T;										//T天用户请求序列数据,[1,1000]
int requireNum[1000];						//第i+1天用户请求数目
int requireRank[1000];						//第i+1天用户请求的起始地址
struct Require
{
	int type;								//请求类型。0为add操作，1位del操作
	int virtualMachineNum;					//请求的虚拟机类型编号
	int id;									//虚拟机ID
}require[100000];							//用户请求

struct VirtualMachine						//请求使用的虚拟机
{
	int id;									//虚拟机ID
	int type;								//虚拟机型号
}virtualMachine[100000 + 10];

struct Sever								//已经购买的、使用中的服务器
{
	int type;								//服务器型号编号

	int remainCoreNodeA;					//服务器A结点剩余内核
	int remainCoreNodeB;					//服务器B结点剩余内核
	int remainMemoryNodeA;					//服务器A结点剩余内存
	int remainMemoryNodeB;					//服务器B结点剩余内存

	int open;								//服务器是否工作中
	int cost;								//该服务器的累计成本

	VirtualMachine* vmLinkHead = nullptr;	//该服务器中当前存在的虚拟机

	static int severNum;					//已经购买的服务器数量

	void addVirtualMachine(int type, int id)//添加虚拟机
	{

	}
	void delVirtualMachine(int id)			//删除虚拟机
	{

	}
}sever[100000 + 10];						//sever[i] 编号为i的服务器

int severIDVM[1000000 + 10];

void addSever(int type)
{

}



int main() {


	return 0;
}
