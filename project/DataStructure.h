//
// Created by kvrmnks on 2021/3/10.
//

#ifndef PROJECT_DATASTRUCTURE_H
#define PROJECT_DATASTRUCTURE_H

struct ServerInformation					//服务器信息
{
    char typeName[21];						//服务器型号
    int coreNum;							//CPU核数，不超过1024
    int memorySize;							//内存大小,单位G，不超过1024G
    int hardwareCost;						//硬件成本，不超过5e5
    int dayCost;							//每日耗能成本，不超过5000
};

struct VirtualMachineInformation
{
    char typeName[21];						//虚拟机型号
    int coreNum;							//CPU核数
    int memorySize;							//内存大小
    int isDoubleNode;						//是否双节点部属，0为单节点部署，1为双节点部属

    int coreNumNode;						//单节点CPU核数
    int memorySizeNode;						//单节点CPU内存占用
};

struct Require
{
    int type;								//请求类型。0为add操作，1位del操作
    char virtualMachineName[21];
    int virtualMachineNum;					//请求的虚拟机类型编号
    int id;									//虚拟机ID
};

struct VirtualMachine						//请求使用的虚拟机
{
    int id;									//虚拟机ID
    int type;								//虚拟机型号
};

struct Server								//已经购买的、使用中的服务器
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
};
#endif //PROJECT_DATASTRUCTURE_H
