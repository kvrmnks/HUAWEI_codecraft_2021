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
/*
int base_migration(int seed, Actions &logger) {

    int maxRemainCore = 0, maxRemainMemory = 0;
    int maxRemainCoreDouble = 0, maxRemainMemoryDouble = 0;
    for(int i = 0;i < serverNum;++ i) {
        maxRemainCore = std::max(maxRemainCore, std::max(server[i].remainCoreNodeA, server[i].remainCoreNodeB));
        maxRemainMemory = std::max(maxRemainMemory, std::max());


    }

    for(int i = 0;i < serverNum;++ i) {
        for(auto seNowTmp = server[serverNum].vmList.begin();seNowTmp != server[serverNum].vmList.end();++ seNowTmp) {
            auto seNow =*seNowTmp;

        }

    }
};*/

long long base_solver_with_choice_max_remain(int seed, Actions &logger) {
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


int canAddServer[100010];
long long base_solver_with_rand_all(int seed, Actions &logger) {
    init();

    srand(seed);

    for(int i = 0;i < T;++ i)
    {
        logger.start_a_brand_new_day();

        int maxRank = requireRank[i] + requireNum[i];
        int tot;
        for(int j = requireRank[i];j < maxRank; ++ j)
        {

            tot = 0;
            const Require& req = require[j];
            int vmType = mpVirtualMachine[string(req.virtualMachineName)];

            if(req.type == 0)
            {
                int vmRank = virtualMachineNum;
                addVirtualMachine(vmType, req.id);

                for(int k = 0;k < serverNum;++ k)
                {
                    if(server[k].canAddVirtualMachine(vmRank, 0))
                    {
                        canAddServer[tot] = k;
                        ++ tot;
                    }
                    else if(server[k].canAddVirtualMachine(vmRank, 1))
                    {
                        canAddServer[tot] = k;
                        ++ tot;
                    }
                }
                if(tot)
                {
                    int tmp = canAddServer[rand() % tot];
                    if(server[tmp].canAddVirtualMachine(vmRank, 0))
                    {
                        server[tmp].addVirtualMachine(vmRank, 0);
                    }
                    else if(server[tmp].canAddVirtualMachine(vmRank, 1))
                    {
                        server[tmp].addVirtualMachine(vmRank, 1);
                    }
                }
                else
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

int canAddServerCore[100010];
long long base_solver_with_select_samll(int seed, Actions &logger) {
    init();

    srand(seed);

    for(int i = 0;i < T;++ i)
    {
        logger.start_a_brand_new_day();

        int maxRank = requireRank[i] + requireNum[i];
        int tot;
        for(int j = requireRank[i];j < maxRank; ++ j)
        {

            tot = 0;
            const Require& req = require[j];
            int vmType = mpVirtualMachine[string(req.virtualMachineName)];

            if(req.type == 0)
            {
                int vmRank = virtualMachineNum;
                addVirtualMachine(vmType, req.id);

                for(int k = 0;k < serverNum;++ k)
                {
                    if(server[k].canAddVirtualMachine(vmRank, 0))
                    {
                        canAddServer[tot] = k;
                        canAddServerCore[tot] = 0;
                        ++ tot;
                    }
                    else if(server[k].canAddVirtualMachine(vmRank, 1))
                    {
                        canAddServer[tot] = k;
                        canAddServerCore[tot] = 1;
                        ++ tot;
                    }
                }
                if(tot)
                {
                    int miRank = canAddServer[0], miCore = canAddServerCore[0];
                    for(int k = 1;k < tot;++ k)
                    {
                        int tmpRank = canAddServer[k];
                        int tmpCore = canAddServerCore[k];
                        int miRemain = miCore == 0 ? server[miRank].remainCoreNodeA + server[miRank].remainMemoryNodeA : server[miRank].remainCoreNodeB + server[miRank].remainMemoryNodeB;
                        int tmpRemain = tmpCore == 0 ? server[tmpRank].remainMemoryNodeA + server[tmpRank].remainMemoryNodeA : server[tmpRank].remainCoreNodeB + server[tmpRank].remainMemoryNodeB;

                        if(miRemain > tmpRemain)
                            miRemain = tmpRemain;
                    }

                    server[miRank].addVirtualMachine(vmRank, miCore);
                }
                else
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


//==========
//文章中第一个方法
int cntVmDuration[100010];

std::pair<bool, bool> canAddVmToServerById(const int vmId, const int serverId){

    VirtualMachine& vm = virtualMachine[vmId];
    Server& se = server[serverId];
    ServerInformation& serInfor = serverInformation[se.type];

    // 初始化每个节点的剩余资源
    int remainCoreA = (serInfor.coreNum >> 1), remainCoreB = (serInfor.coreNum >> 1);
    int remainMemoryA = (serInfor.memorySize >> 1), remainMemoryB = (serInfor.memorySize >> 1);
    int presumCoreA = 0, presumCoreB = 0;
    int presumMA = 0, presumMB = 0;
    if(serverId == 0 && se.vmList.size() == 13){
       int a = 2;
    }

    // 计算现有的资源占用量
    for(auto i : se.vmList)
    {
        // 获得虚拟机
        VirtualMachine& vmTmp = virtualMachine[i];
        /*
        if(vmTmp.beginTime > vm.endTime)
            continue;
        if(vmTmp.beginTime == vm.endTime  && vmTmp.inAddReqRank > vm.inDelReqRank)
            continue;
        if(vmTmp.endTime < vm.beginTime)
            continue;
        if(vmTmp.endTime == vm.beginTime  && vmTmp.inDelReqRank < vm.inAddReqRank)
            continue;
        */
        VirtualMachineInformation& vmInfor = virtualMachineInformation[vmTmp.type];
        if(vmInfor.isDoubleNode)
        {
            // 双节点部署的情况下
            remainCoreA -= vmInfor.coreNumNode;
            remainMemoryA -= vmInfor.memorySizeNode;
            remainCoreB -= vmInfor.coreNumNode;
            remainMemoryB -= vmInfor.memorySizeNode;

            presumCoreA += vmInfor.coreNumNode;
            presumCoreB += vmInfor.coreNumNode;
            presumMA += vmInfor.memorySizeNode;
            presumMB +=  vmInfor.memorySizeNode;
        }
        else if(vmTmp.nodeNum == 0)
        {
            // 单节点部署的情况下 部署在0号节点
            remainCoreA -= vmInfor.coreNumNode;
            remainMemoryA -= vmInfor.memorySizeNode;

            presumCoreA += vmInfor.coreNumNode;
//            presumCoreB += vmInfor.coreNumNode;
            presumMA += vmInfor.memorySizeNode;
//            presumMB +=  vmInfor.memorySizeNode
        }
        else if(vmTmp.nodeNum == 1)
        {
            // 单节点部署的情况下 部署在1号节点
            remainCoreB -= vmInfor.coreNumNode;
            remainMemoryB -= vmInfor.memorySizeNode;

//            presumCoreA += vmInfor.coreNumNode;
            presumCoreB += vmInfor.coreNumNode;
//            presumMA += vmInfor.memorySizeNode;
            presumMB +=  vmInfor.memorySizeNode;
        }
    }

    VirtualMachineInformation& vmInfor = virtualMachineInformation[vm.type];
    std::pair<bool, bool> re;
    // 尝试将新的节点加入虚拟机中
    if(vmInfor.isDoubleNode)
    {
        // 计算双节点下的情况
        remainCoreA -= vmInfor.coreNumNode;
        remainMemoryA -= vmInfor.memorySizeNode;
        remainCoreB -= vmInfor.coreNumNode;
        remainMemoryB -= vmInfor.memorySizeNode;

        if(remainCoreA < 0 || remainCoreB < 0 ||
           remainMemoryA < 0 || remainMemoryB < 0)
            re.first = re.second = false;
        else
            re.first = re.second = true;
    }
    else
    {
        int r1 = remainCoreA - vmInfor.coreNumNode;
        int r2 = remainMemoryA - vmInfor.memorySizeNode;

        int p1 = remainCoreB - vmInfor.coreNumNode;
        int p2 = remainMemoryB - vmInfor.memorySizeNode;

        if(r1 >= 0 && r2 >= 0)
        {
            re.first = true;
        }
        else
        {
            re.first = false;
        }
        if(p1 >= 0 && p2 >= 0)
        {
            re.second = true;
        }
        else
        {
            re.second = false;
        }
    }

    if(serverId == 0){
        cerr << remainCoreA << " " << remainCoreB << " " << remainMemoryA << " " << remainMemoryB << endl;
    }
    return re;
}
/*
std::map<int, int> vmToServer;              //记录下标为i的虚拟机加入到了服务器vmToServerpi[中

///=======
void tmpAdd(int num, int& sumCoreA, int& sumCoreB, int& sumMA, int& sumMB)
{
    VirtualMachineInformation& vmInfor = virtualMachineInformation[virtualMachine[num].type];
    printf("vm%d:Type %d, Core %d, M %d, Double %d\n", num, virtualMachine[num].type, vmInfor.coreNumNode, vmInfor.memorySizeNode, vmInfor.isDoubleNode ? 1 : 0);

}

//=======
*/
long long first_solver(int seed, Actions &logger)
{

    init(); // 排序与初始化

    srand(seed);

    for(int t = 0;t < T;++ t)
    {
        int maxRank = requireRank[t] + requireNum[t];
        for(int j = requireRank[t];j < maxRank; ++ j) // 枚举一天中的事情
        {
            Require& req = require[j];
            if(req.type == 0)
            {//加入虚拟机的操作
                addVirtualMachine(mpVirtualMachine[string(req.virtualMachineName)], req.id, j, 200000-1, t, T);
            }
            else if(req.type == 1)
            {//删除虚拟机的操作
                int vmRank = vmIdToRank[req.id];
                virtualMachine[vmRank].endTime = t;
                virtualMachine[vmRank].duration = t - virtualMachine[vmRank].beginTime;
                virtualMachine[vmRank].inDelReqRank = j;
            }
        }
    }
    for(int i = 0;i < virtualMachineNum;++ i) {
        cntVmDuration[i] = i;
    }
    std::sort(cntVmDuration, cntVmDuration + virtualMachineNum, [](int x, int y) {
        return virtualMachine[x].duration > virtualMachine[y].duration;
    });

    for(int p = 0;p < virtualMachineNum;++ p)
    {
        bool hasSever = false;
        int i = cntVmDuration[p];
        for(int j = 0;j < serverNum;++ j)
        {
            auto tmp = canAddVmToServerById(i, j);
            if(tmp.first || tmp.second){
                server[j].addVirtualMachineForFirst(i, (tmp.first) ? 0 : 1);
                if(j == 0){
                    canAddVmToServerById(i, j);    auto p = server[0].vmList;
                    for(auto x : p){
                        cerr << virtualMachine[x] << virtualMachineInformation[virtualMachine[x].type] << endl;
                    }
                }
                hasSever = true;
                break;
            }
        }
        if(!hasSever)
        {
            int tmp;
            do {
                tmp = rand() % N;
            } while((!serverInformation[tmp].canAddVirtualMachine(i, 0)) &&
            (!serverInformation[tmp].canAddVirtualMachine(i, 1)));
            addServer(tmp);
            std::pair<bool, bool> canTmp = canAddVmToServerById(i, serverNum - 1);
            if(canTmp.first){
                server[serverNum-1].addVirtualMachineForFirst(i, 0);
            }
            else if(canTmp.second){
                server[serverNum-1].addVirtualMachineForFirst(i, 1);
            }
        }
    }
    for(int i = 0;i < T;++ i)
    {
        logger.start_a_brand_new_day();
        if(i == 0)
        {
            for(int j = 1;j <= serverNum;++ j)
            {
                logger.log_a_server(j - 1, server[j].type);
            }
        }

        int maxRank = requireNum[i] + requireRank[i];
        for(int j = requireRank[i];j < maxRank;++ j)
        {
            if(require[j].type == 0)
            {
                int vmRank = vmIdToRank[require[j].id];
                logger.log_a_vm_deployment(vmRank);
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
