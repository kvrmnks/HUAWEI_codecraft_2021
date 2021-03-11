//
// Created by kvrmnks on 2021/3/10.
//

#ifndef PROJECT_DATASTRUCTURE_H
#define PROJECT_DATASTRUCTURE_H


#include <map>
#include <string>
using namespace std

int N;										//可以采购服务器的数量,[1,100]
struct ServerInformation					//服务器信息
{
    char typeName[21];						//服务器型号
    int coreNum;							//CPU核数，不超过1024
    int memorySize;							//内存大小,单位G，不超过1024G
    int hardwareCost;						//硬件成本，不超过5e5
    int dayCost;							//每日耗能成本，不超过5000
}serverInformation[100];					//可以采购的服务器类型信息
map<string, int> mpSevere;                  //服务器型号名到编号（下标）的映射

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
map<string, int> mpVirtualMachine;          //虚拟机型号名到编号（下标）的映射


int T;										//T天用户请求序列数据,[1,1000]
struct Require
{
    int type;								//请求类型。0为add操作，1位del操作
    char virtualMachineName[21];            //虚拟机名称
    int virtualMachineNum;					//请求的虚拟机类型编号
    int id;									//虚拟机ID
}require[100000];							//用户请求
int requireNum[1000];						//第i+1天用户请求数目
int requireRank[1000];						//第i+1天用户请求的起始地址

struct VirtualMachine						//请求使用的虚拟机
{
    int id;									//虚拟机ID
    int type;								//虚拟机型号

    int nodeNum;                            //如果虚拟机在服务器中存储，nodeNum表示被部署到了哪个节点。双节点部署模式无效。
    VirtualMachine* next = nullptr;         //如果虚拟机在服务器中存储，next表征链表
}virtualMachine[100000 + 10];





struct Server								//已经购买的、使用中的服务器
{
    int type;								//服务器型号编号

    int remainCoreNodeA;					//服务器A结点剩余内核
    int remainCoreNodeB;					//服务器B结点剩余内核
    int remainMemoryNodeA;					//服务器A结点剩余内存
    int remainMemoryNodeB;					//服务器B结点剩余内存

    bool open;								//服务器是否工作中
    int cost;								//该服务器的累计成本

    VirtualMachine* virtualMachineLinkHead = nullptr;	//该服务器中当前存在的虚拟机

    static int severNum;					//已经购买的服务器数量

//    //检查type类型虚拟机是否能添加到当前服务器
//    //core=0表示添加到A核，core=1表示添加到B核。若虚拟机为双节点部署,core无效
//    void canAddVirtualMachine(int type, int id, int core = 0)
//    {
//
//    }

    //添加虚拟机到服务器;
    //core=0表示添加到A核，core=1表示添加到B核。若虚拟机为双节点部署,core无效
    //返回是否添加成功
    bool addVirtualMachine(const VirtualMachineInformation& vmInfor, int type, int id, int core = 0)
    {
        if(vmInfor.isDoubleNode)
        {
            if(remainCoreNodeA < vmInfor.coreNumNode || remainCoreNodeB < vmInfor.coreNumNode)
                return false;
            if(remainMemoryNodeA < vmInfor.memorySizeNode || remainMemoryNodeB < vmInfor.memorySizeNode)
                return false;

            remainCoreNodeA -= vmInfor.coreNumNode, remainCoreNodeB -= vmInfor.coreNumNode;
            remainMemoryNodeA -= vmInfor.memorySizeNode, remainMemoryNodeB -= vmInfor.memorySizeNode;

            open = true;
        }
        else
        {
            if(core == 0)
            {
                if(remainCoreNodeA < vmInfor.coreNumNode)
                    return false;
                if(remainMemoryNodeA < vmInfor.memorySizeNode)
                    return false;

                remainCoreNodeA -= vmInfor.coreNumNode;
                remainMemoryNodeA -= vmInfor.memorySizeNode;

                open = true;
            }
            else if(core == 1)
            {
                if(remainCoreNodeB < vmInfor.coreNumNode)
                    return false;
                if(remainMemoryNodeB < vmInfor.memorySizeNode)
                    return false;

                remainCoreNodeB -= vmInfor.coreNumNode;
                remainMemoryNodeB -= vmInfor.memorySizeNode;

                open = true;
            }
            else return false;
        }

        VirtualMachine* now = new VirtualMachine();
        now->id = id;
        now->type = type;
        now->next = virtualMachineLinkHead;
        virtualMachineLinkHead = now;

        return true;
    }

    //删除虚拟机
    //若不存在ID为id的虚拟机，返回false
    bool delVirtualMachine(const VirtualMachineInformation& vmInfor, int id)
    {
        VirtualMachine* now = virtualMachineLinkHead;
        while(now != nullptr)
        {
            if(now->id == id)
            {
                if(vmInfor.isDoubleNode)
                {
                    remainCoreNodeA += vmInfor.coreNumNode, remainCoreNodeB += vmInfor.coreNumNode;
                    remainMemoryNodeA += vmInfor.memorySizeNode, remainMemoryNodeB += vmInfor.memorySizeNode;
                }
                else
                {
                    if(now->nodeNum == 1)
                    {
                        if(remainCoreNodeA < vmInfor.coreNumNode)
                            return false;
                        if(remainMemoryNodeA < vmInfor.memorySizeNode)
                            return false;

                        remainCoreNodeA -= vmInfor.coreNumNode;
                        remainMemoryNodeA -= vmInfor.memorySizeNode;

                        open = true;
                    }
                    else if(now->nodeNum == 2)
                    {
                        if(remainCoreNodeB < vmInfor.coreNumNode)
                            return false;
                        if(remainMemoryNodeB < vmInfor.memorySizeNode)
                            return false;

                        remainCoreNodeB -= vmInfor.coreNumNode;
                        remainMemoryNodeB -= vmInfor.memorySizeNode;

                        open = true;
                    }
                }


                if(virtualMachineLinkHead == nullptr)
                    open = false;
                return true;
            }
            else now = now->next;
        }
        return false;
    }
}sever[100000 + 10];						//sever[i] 编号为i的服务器
int severIDVM[1000000 + 10];


#endif //PROJECT_DATASTRUCTURE_H
