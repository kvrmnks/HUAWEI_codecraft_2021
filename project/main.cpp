/*运行结果记录
 *  20次循环base_solver:1,385,237,273
 *  50次循环base_solver:1,382,498,784
 *  10次循环base_solver_with_rand_all:1,270,989,790
 *
 * */


#include "input.h"
#include "DataStructure.h"
#include <cstdlib>
#include "output.h"
#include "BaselineSolver.h"

#define DEBUG

//#define ONLINE_JUDGE
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

int main() {
    std::ios::sync_with_stdio(false);
//    auto p = freopen("training-2.txt", "r", stdin);

#ifdef IO_DEBUG

    freopen("training-1.txt", "r", stdin);
   freopen("training-1-out.txt", "w", stdout);

#endif

    readData();

    Actions logger;

    long long min_cost = INT64_MAX;

    srand(0);

//    srand(19260817);  //683,830,557 + 702,452,646 = 1,386,283,203
    //srand(996251404); //687,231,808 + 705,445,429
    //srand(2019051301);//679, 094, 368+702,452,646

    for (int i = 0; i < 1; i++) {
        Actions logger_i;
        long long cost = first_solver(rand(), logger_i);

        if (cost < min_cost) {
            logger = logger_i;
            min_cost = cost;
        }
    }

//    long long cost = base_solver(19260817, logger);

    logger.print();

#ifdef COST_DEBUG
    cout << min_cost << endl;
#endif

    return 0;
}
