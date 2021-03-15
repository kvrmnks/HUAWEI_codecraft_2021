#include "input.h"
#include "DataStructure.h"
#include <cstdlib>
#include "output.h"
#include <algorithm>

#define DEBUG

#define ONLINE_JUDGE
#ifdef ONLINE_JUDGE
#undef DEBUG
#endif

#ifdef DEBUG

//#define INOUT_DEBUG
#define IO_DEBUG
//#define LOGIC_DEBUG
//#define BREAKPOINT_DEBUG
//#define GRAMMAR_DEBUG
#define COST_DEBUG
//#define SEVERERROR_DEBUUG

#endif


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
#ifdef INOUT_DEBUG
    for(int i = 1; i <= N; i++){
        auto p = serverInformation[i];
        std::cout << p.typeName << " " << p.memorySize << " "
        << p.dayCost << " " << p.hardwareCost << " " << p.coreNum << std::endl;
    }
    for(int i = 1; i <= M; i ++){
        auto p = virtualMachineInformation[i];
        std::cout << p.typeName << " " << p.coreNum << " " << p.memorySize << " " << p.isDoubleNode << "\n";
    }
    for(int i = 0; i < T; i ++){
        for(int j = 0; j < requireNum[i]; j ++){
            auto p = require[requireRank[i] + j];
            std::cout << p.virtualMachineNum << " " << p.virtualMachineName << " " << p.id << " " << p.type << std::endl;
        }
    }
#endif
}

void init()
{
    for(int i = 0;i < 100000;++ i)
        server[i].rank = i;
    /*for(int i = 0;i < 100;++ i)
        cntSeverInformation[i] = i;

    std::sort(cntSeverInformation, cntSeverInformation + N, [](int x, int y){
        return (serverInformation[x].coreNum + serverInformation[x].memorySize)/serverInformation[x].hardwareCost > (serverInformation[y].coreNum + serverInformation[y].memorySize)/serverInformation[y].hardwareCost;
    });*/

}


int main() {
    std::ios::sync_with_stdio(false);
//    auto p = freopen("training-2.txt", "r", stdin);

#ifdef IO_DEBUG

    freopen("training-2.txt", "r", stdin);
    freopen("training-2-out.txt", "w", stdout);

#endif

    readData();

    init();

    Actions logger;


    srand(19260817);  //683,830,557 + 702,452,646 = 1,386,283,203
    //srand(996251404); //687,231,808 + 705,445,429
    //srand(2019051301);//679, 094, 368+702,452,646


#ifdef LOGIC_DEBUG
    int tmp = 0;
#endif
    for(int i = 0;i < T;++ i)
    {
#ifdef LOGIC_DEBUG
//        printf("\n%d\n", tmp);
#endif

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
#ifdef LOGIC_DEBUG
                ++ tmp;
#endif

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
                    /*server_type = cntSeverInformation[0];
                    while(!serverInformation[server_type].canAddVirtualMachine(vmRank, 0) && !serverInformation[server_type].canAddVirtualMachine(vmRank, 1))
                    {
                        server_type += 1;
                        server_type %= N;

#ifdef GRAMMAR_DEBUG
                        cout<<server_type<<endl;
#endif

                    }*/
                    do
                    {
                        server_type = rand() % N;

#ifdef GRAMMAR_DEBUG
                        cout<<server_type<<endl;
#endif

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
#ifdef SEVERERROR_DEBUUG
            int tmp = 0;
            for(int k = 0;k < serverNum;++ k)
            {
                if (server[k].remainMemoryNodeA < 0 || server[k].remainMemoryNodeB < 0 || server[k].remainCoreNodeA < 0 || server[k].remainCoreNodeB < 0)
                {
                    tmp = 1;
                }
            }
#endif
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

    logger.print();

#ifdef COST_DEBUG
    long long sumCost = 0;
    for(int i = 0;i < serverNum;++ i)
    {
        sumCost += server[i].cost;
    }
    printf("\n%lld\n", sumCost);
#endif

    return 0;
}
