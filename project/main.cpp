#include "input.h"

int N;										//可以采购服务器的数量,[1,100]
ServerInformation serverInformation[100];						//可以采购的服务器类型信息
int M;										//售买虚拟机的数量,[1,1000]
VirtualMachineInformation virtualMachineInformation[1000];			//虚拟机型号信息

int T;										//T天用户请求序列数据,[1,1000]
int requireNum[1000];						//第i+1天用户请求数目
int requireRank[1000];						//第i+1天用户请求的起始地址
Require require[100000];							//用户请求

VirtualMachine virtualMachine[100000 + 10];
Server sever[100000 + 10];						//sever[i] 编号为i的服务器
int severIDVM[1000000 + 10];

void addSever(int type)
{

}


void readData()
{
    scanf("%d", &N);
    for(int i = 0; i < N; i ++)
    {
        serverInformation[i] = readOneServer();
    }
    scanf("%d", &M);
    for(int i = 0; i < M; i ++)
    {
        virtualMachineInformation[i] = readOneVirtualMachine();
    }
    scanf("%d", &T);

    int cnt = 0;
    for(int i = 0; i < T; i ++)
    {
        scanf("%d", &requireNum[i]);
        requireRank[i] = cnt;
        for(int j = 0; j < requireNum[i]; j ++){
            require[cnt ++] = readOneRequire();
        }
    }

//     对于输入的测试
//    for(int i = 1; i <= N; i++){
//        auto p = serverInformation[i];
//        std::cout << p.typeName << " " << p.memorySize << " "
//        << p.dayCost << " " << p.hardwareCost << " " << p.coreNum << std::endl;
//    }
//    for(int i = 1; i <= M; i ++){
//        auto p = virtualMachineInformation[i];
//        std::cout << p.typeName << " " << p.coreNum << " " << p.memorySize << " " << p.isDoubleNode << "\n";
//    }
//    for(int i = 0; i < T; i ++){
//        for(int j = 0; j < requireNum[i]; j ++){
//            auto p = require[requireRank[i] + j];
//            std::cout << p.virtualMachineName << " " << p.id << " " << p.type << std::endl;
//        }
//    }
}

int main() {
    auto p = freopen("training-1.txt", "r", stdin);
//    std::cout << p << std::endl;
    readData();
    return 0;
}
