//
// Created by Furyton on 2021/3/11.
//

#ifndef PROJECT_OUTPUT_H
#define PROJECT_OUTPUT_H

#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#define lowbit(x) ((x)&(-(x)))

#include "DataStructure.h"

using namespace std;

// 记录购买服务器的信息
struct PurchaseInfo {
    vector<pair<string, unsigned> > serverNumPerType; // type name, quantity

    friend ostream &operator << (ostream &output, PurchaseInfo& purchaseInfo) {
        output << "(purchase, " << purchaseInfo.serverNumPerType.size() << ")" << endl;
        for(auto x : purchaseInfo.serverNumPerType) {
            output << '(' << x.first << ", " << x.second << ')' << endl;
        }
        return output;
    }
};

// 记录迁移的信息
struct Migration {
    vector<pair<pair<unsigned, unsigned>, char> > migrationInfo; // vm_id, server_id,  (node : 'N', 'A', 'B');

    friend ostream &operator << (ostream &output, Migration& migration) {
        output << "(migration, " << migration.migrationInfo.size() << ")" << endl;
        for(auto x : migration.migrationInfo) {
            auto basicInfo = x.first;
            if (x.second == 'N') {
                output << '(' << basicInfo.first << ", " << basicInfo.second << ')' << endl;
            } else {
                output << '(' << basicInfo.first << ", " << basicInfo.second << ", " << x.second << ')' << endl;
            }
        }
        return output;
    }
};

// 记录虚拟机调度的信息
struct DeployRequest {
    vector<pair<unsigned, char> > deployments; // server_id, (node : 'N', 'A', 'B')

    friend ostream &operator << (ostream &output, DeployRequest& deployRequest) {
        for(auto x : deployRequest.deployments) {
            if (x.second == 'N') {
                output << '(' << x.first << ')' << endl;
            } else {
                output << '(' << x.first << ", " << x.second << ')' << endl;
            }
        }
        return output;
    }
};

// 记录每天的决策信息，包括购买信息，迁移信息，调度信息
struct DailyAction {
    PurchaseInfo purchase;
    Migration migration;
    DeployRequest deployment;

    void insertPurchase (pair<string, unsigned> p) {
        purchase.serverNumPerType.push_back(p);
    }
    void insertMigration (pair<pair<unsigned, unsigned>, char> m) {
        migration.migrationInfo.push_back(m);
    }
    void insertDeployment (pair<unsigned, char> d) {
        deployment.deployments.push_back(d);
    }
    friend ostream &operator << (ostream &output, DailyAction dailyAction) {
        output << dailyAction.purchase << dailyAction.migration << dailyAction.deployment;
        return output;
    }
};

const int MAX_TYPE = 100;

// 记录所有天的决策信息
struct Actions {
private:
    vector<DailyAction> actions;
    int bucket_for_server[200];
    DailyAction dailyAction;
    bool fresh = false;
    bool stored = false;
    map<int, int> server_rank_id_map;
    void check() {
        if (!fresh) {
            cerr << "oops! You haven't initialise the logger.";
            exit(250);
        }
    }
    void log_a_deploy(int target_server_id) {
        dailyAction.insertDeployment(make_pair(target_server_id, 'N'));
    }
    void log_a_deploy(int target_server_id, int node) {
        dailyAction.insertDeployment(make_pair(target_server_id, node == 0 ? 'A' : 'B'));
    }


    void increase_bucket(int type) {
        type ++;
        while (type <= MAX_TYPE) {
            bucket_for_server[type] ++;
            type += lowbit(type);
        }
    }

    int get_server_id(int type) {
        type ++;
        int res = 0;

        while(type > 0) {
            res += bucket_for_server[type];
            type -= lowbit(type);
        }
        return res;
    }
public:
    void init() {
        for (int i = 0; i <= MAX_TYPE; i++) {
            bucket_for_server[i] = 0;
        }
    }

    // 初始化今天的信息
    void start_a_brand_new_day() {
        dailyAction = DailyAction();
        fresh = true;
        stored = false;
    }

//    买了下标为 rank, 类型为type的服务器
    void log_a_server(int rank, int type) {
        check();
        server_rank_id_map[rank] = get_server_id(type);
        increase_bucket(type);
    }

//    买了一个名字为 name 的服务器
    void log_a_server(string name) {
        check();
        bucket_for_server[mpSevere[name]] ++;
    }

//    做了一次双节点的迁移，将id为 vm_id 的虚拟机迁移到 下标为 rank 的服务器
//    必须在所有log_a_server之后执行
    void log_a_migration(int vm_id, int rank) {
        if (server_rank_id_map.count(rank) == 0) {
            cerr<<"vm_id"<<vm_id<<endl;
            exit(234);
        }
        dailyAction.insertMigration(make_pair(make_pair(vm_id, server_rank_id_map[rank]), 'N'));
    }

//    做了一次单节点的迁移，将id为 vm_id 的虚拟机迁移到 下标为 rank 的服务器，节点为 node (0 \ 1)
    void log_a_migration(int vm_id, int rank, int node) {
        if (server_rank_id_map.count(rank) == 0) {
            cerr<<"vm_id"<<vm_id<<endl;
            exit(235);
        }
        dailyAction.insertMigration(make_pair(make_pair(vm_id, server_rank_id_map[rank]), node == 0 ? 'A' : 'B'));
    }

//    给下标为vm_rank的虚拟机做了一次调度
//    必须在所有log_a_server之后执行
    void log_a_vm_deployment(int vm_rank) {
        VirtualMachine& vm = virtualMachine[vm_rank];
        VirtualMachineInformation& vmInfor= virtualMachineInformation[vm.type];

        if (server_rank_id_map.count(vm.serverNum) == 0) {
            exit(233);
        }

        if (vmInfor.isDoubleNode) {
            log_a_deploy(server_rank_id_map[vm.serverNum]);
        } else {
            log_a_deploy(server_rank_id_map[vm.serverNum], vm.nodeNum);
        }
    }

//    一天结束，保存购买信息等
    void call_an_end_to_this_day() {
        fresh = false;
        stored = true;

        for (int i = 0; i < N; i++) {
            if (bucket_for_server[i] != 0) {
                dailyAction.insertPurchase(make_pair(string(serverInformation[i].typeName), bucket_for_server[i]));
            }
        }

        actions.push_back(dailyAction);
    }

//    输出
    void print() {
        for (auto x : actions) {
            cout << x;
        }
    }
};

#endif //PROJECT_OUTPUT_H
