//
// Created by Furyton on 2021/3/16.
//

#ifndef PROJECT_BASELINESOLVER_H
#define PROJECT_BASELINESOLVER_H

#include "output.h"
#include "DataStructure.h"

long long base_solver(int seed, Actions &logger) {
    init();

    srand(seed);

    for(int i = 0;i < T;++ i)
    {
        logger.start_a_brand_new_day();

        int maxRank = requireRank[i] + requireNum[i];
        for(int j = requireRank[i];j < maxRank; ++ j)
        {

            const Require& req = require[j];
            int vmType = mpVirtualMachine[string(req.virtualMachineName)];

            if(req.type == 0)
            {
                int vmRank = virtualMachineNum;
                vmIdToRank[req.id] = vmRank;
                addVirtualMachine(vmType, req.id);

                bool hasServerUse = false;
                for(int k = 0;k < serverNum;++ k)
                {
                    if(server[k].canAddVirtualMachine(vmRank, 0))
                    {
                        server[k].addVirtualMachine(vmRank, 0);
                        hasServerUse = true;
                        break;
                    }
                    else if(server[k].canAddVirtualMachine(vmRank, 1))
                    {
                        server[k].addVirtualMachine(vmRank, 1);
                        hasServerUse = true;
                        break;
                    }
                }
                if(!hasServerUse)
                {
                    int server_type;

                    do
                    {
                        server_type = rand() % N;

                    }while(!serverInformation[server_type].canAddVirtualMachine(vmRank, 0) && !serverInformation[server_type].canAddVirtualMachine(vmRank, 1));

                    addServer(server_type);
                    logger.log_a_server(serverNum - 1, server_type);

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
            else if(req.type == 1)
            {
                if(vmIdToRank.count(req.id) == 0) continue;
                int vmRank = vmIdToRank[req.id];
                VirtualMachine vm = virtualMachine[vmRank];
                server[vm.serverNum].delVirtualMachine(req.id);
            }
        }
        for(int j = 0;j < serverNum;++ j)
        {
            if(server[j].open)
            {
                server[j].cost += server[j].dayCost;
            }
        }
        logger.call_an_end_to_this_day();
    }

    long long sumCost = 0;
    for(int i = 0;i < serverNum;++ i)
    {
        sumCost += server[i].cost;
    }

    return sumCost;
}

/*int base_migration(int seed, Actions &logger) {

    int maxRemainCore = 0, maxRemainMemory = 0;
    int maxRemainCoreDouble = 0, maxRemainMemoryDouble = 0;
    for(int i = 0;i < serverNum;++ i) {
        maxRemainCore = std::max(maxRemainCore, std::max(server[i].remainCoreNodeA, server[i].remainCoreNodeB));
        maxRemainMemory = std::max(maxRemainMemory, std::max())


    }

    for(int i = 0;i < serverNum;++ i) {
        for(auto seNowTmp = server[serverNum].vmList.begin();seNowTmp != server[serverNum].vmList.end();++ seNowTmp) {
            auto seNow =*seNowTmp;

        }

    }
};*/

long long base_solver_with_migration(int seed, Actions &logger) {
    init();

    srand(seed);

    for(int i = 0;i < T;++ i)
    {
        logger.start_a_brand_new_day();

        int maxRank = requireRank[i] + requireNum[i];
        for(int j = requireRank[i];j < maxRank; ++ j)
        {

            const Require& req = require[j];
            int vmType = mpVirtualMachine[string(req.virtualMachineName)];

            if(req.type == 0)
            {
                int vmRank = virtualMachineNum;
                vmIdToRank[req.id] = vmRank;
                addVirtualMachine(vmType, req.id);
                VirtualMachine& vm = virtualMachine[vmRank];
                VirtualMachineInformation& vmInfor = virtualMachineInformation[vmType];

                bool hasServerUse = false;
                for(int k = 0;k < serverNum;++ k)
                {
                    bool canA = server[k].canAddVirtualMachine(vmRank, 0), canB = server[k].canAddVirtualMachine(vmRank, 1);
                    if(!canA && !canB) continue;
                    else if(canA && canB) {
                        if(vmInfor.isDoubleNode) {
                            server[k].addVirtualMachine(vmRank, 0);
                            hasServerUse = true;
                            break;
                        }
                        else {
                            if(server[k].remainCoreNodeA > server[k].remainCoreNodeB) {
                                server[k].addVirtualMachine(vmRank, 0);
                                hasServerUse = true;
                                break;
                            }
                            else {
                                server[k].addVirtualMachine(vmRank, 1);
                                hasServerUse = true;
                            }

                        }
                    }
                    else if(canA)
                    {
                        server[k].addVirtualMachine(vmRank, 0);
                        hasServerUse = true;
                        break;
                    }
                    else if(canB)
                    {
                        server[k].addVirtualMachine(vmRank, 1);
                        hasServerUse = true;
                        break;
                    }
                }
                if(!hasServerUse)
                {
                    int server_type;

                    do
                    {
                        server_type = rand() % N;

                    }while(!serverInformation[server_type].canAddVirtualMachine(vmRank, 0) && !serverInformation[server_type].canAddVirtualMachine(vmRank, 1));

                    addServer(server_type);
                    logger.log_a_server(serverNum - 1, server_type);

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
            else if(req.type == 1)
            {
                if(vmIdToRank.count(req.id) == 0) continue;
                int vmRank = vmIdToRank[req.id];
                VirtualMachine vm = virtualMachine[vmRank];
                server[vm.serverNum].delVirtualMachine(req.id);
            }
        }
        for(int j = 0;j < serverNum;++ j)
        {
            if(server[j].open)
            {
                server[j].cost += server[j].dayCost;
            }
        }
        logger.call_an_end_to_this_day();
    }

    long long sumCost = 0;
    for(int i = 0;i < serverNum;++ i)
    {
        sumCost += server[i].cost;
    }

    return sumCost;
}



#endif //PROJECT_BASELINESOLVER_H
