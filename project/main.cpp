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
#include "SegmentTree.cpp"
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

typedef long long int (*FUNCTYPE)(int, Actions&);
Actions logger;

Actions& winner_solver(vector<FUNCTYPE>& solvers, int T, bool isDebugging) {
    long long min_cost = INT64_MAX;

    srand(0);
//    srand(19260817);  //683,830,557 + 702,452,646 = 1,386,283,203
    //srand(996251404); //687,231,808 + 705,445,429
    //srand(2019051301);//679, 094, 368+702,452,646

    for(int i = 0; i < T; i++)
        for(auto solver: solvers) {
            Actions logger_i(isDebugging);

            long long cost = solver(rand(), logger_i);

            if (cost < min_cost) {
                logger = logger_i;
                min_cost = cost;
            }
        }
#ifdef COST_DEBUG
    cout << "total cost: " << min_cost << endl;
#endif

    return logger;
}

int main() {
    std::ios::sync_with_stdio(false);

#ifdef IO_DEBUG

    freopen("training-1.txt", "r", stdin);
    freopen("training-1-out.txt", "w", stdout);

#endif

    readData();

    vector<FUNCTYPE> solvers; // 存放所有备用解法

//  push your lovely solver into the solvers vector :)

    solvers.push_back(&first_solver);
//    solvers.push_back(&base_solver);
//    solvers.push_back(&base_solver_with_rand_all);



//    simpler way to print

//    winner_solver(solvers).print();

    Actions& final_answer = winner_solver(solvers, 1, true); // T 指rand几次

    final_answer.print();

//    cout << final_answer.get_rank_of_server_with_id(1257) << endl;

    return 0;
}
