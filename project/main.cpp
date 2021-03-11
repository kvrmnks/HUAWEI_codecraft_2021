#include "input.h"
#include "DataStructure.h"

void readData()
{
    scanf("%d", &N);
    for(int i = 0; i < N; i ++)
    {
        serverInformation[i] = readOneServer();
        mpSevere[string(serverInformation[i].typeName)] = i;
    }
    scanf("%d", &M);
    for(int i = 0; i < M; i ++)
    {
        virtualMachineInformation[i] = readOneVirtualMachine();
        mpVirtualMachine[string(virtualMachineInformation[i].typeName)] = i;
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
//            std::cout << p.virtualMachineNum << " " << p.virtualMachineName << " " << p.id << " " << p.type << std::endl;
//        }
//    }
}

void init()
{
    for(int i = 0;i < 100000 + 10;++ i) server[i].rank = i;
}

int main() {
    auto p = freopen("training-1.txt", "r", stdin);
//    std::cout << p << std::endl;
    readData();
    init();
    return 0;
}
