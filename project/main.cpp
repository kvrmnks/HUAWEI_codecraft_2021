#include "input.h"
#include "DataStructure.h"
#include <cstdlib>
#include "output.h"

#define DEBUG

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
    std::ios::sync_with_stdio(false);

#ifdef DEBUG

    auto p = freopen("training-1.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
//    std::cout << p << std::endl;

#endif

    readData();

    init();

    Actions logger;

    for(int i = 0;i < T;++ i)
    {
        logger.start_a_brand_new_day();

        int maxRank = requireRank[i] + requireNum[i];
        for(int j = requireRank[i];j < maxRank; ++ j)
        {
            const Require& req = require[j];
            int vmType = mpVirtualMachine[string(req.virtualMachineName)];
            int vmRank = virtualMachineNum;
            addVirtualMachine(vmType, req.id);

            bool hasServerUse = false;
            for(int k = 0;k < serverNum;++ k)
            {
                if(server[k].canAddVirtualMachine(vmRank, 0))
                {
                    server[k].addVirtualMachine(vmRank, 0);
                    hasServerUse = true;
                }
                else if(server[k].canAddVirtualMachine(vmRank, 1))
                {
                    server[k].addVirtualMachine(vmRank, 1);
                    hasServerUse = true;
                }
            }
            if(!hasServerUse)
            {
                int server_type;
                srand(19260817);
                do
                {
                    server_type = rand() % N;
                    addServer(server_type);

                    logger.log_a_server(server_type);
                }while(!server[serverNum-1].canAddVirtualMachine(vmRank, 0) && !server[serverNum-1].canAddVirtualMachine(vmRank, 1));
                if(server[serverNum-1].canAddVirtualMachine(vmRank, 0))
                {
                    server[serverNum-1].addVirtualMachine(vmRank, 0);
                }
                else
                {
                    server[serverNum-1].addVirtualMachine(vmRank, 1);
                }
            }

            logger.log_a_vm_deployment(vmRank);
        }

        logger.call_an_end_to_this_day();
    }

    logger.print();

    return 0;
}
