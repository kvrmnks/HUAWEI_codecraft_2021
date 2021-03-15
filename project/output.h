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
    DailyAction dailyAction;

    int total_server = 0;

    int bucket_server[MAX_TYPE];
    int prefix_sum[MAX_TYPE];

    vector<pair<int, int> > server_rank_type_vec;
    vector<pair<int, char> > server_rank_for_deployment;
    vector<pair<pair<int, int>, char> > vm_id_server_rank_for_migration;

    map<int, int> server_rank_id_map;
public:

    void start_a_brand_new_day() {
        dailyAction = DailyAction();

        for (int i = 0; i < N; i++) {
            bucket_server[i] = 0;
            prefix_sum[i] = 0;
        }
        server_rank_type_vec.clear();
        server_rank_for_deployment.clear();
        vm_id_server_rank_for_migration.clear();
    }

    // 购买了一台服务器，下标为rank，类型为 type
    void log_a_server(int rank, int type) {
        server_rank_type_vec.emplace_back(rank, type);
        bucket_server[type] ++;
    }

//  做了一次双节点迁移
//  虚拟机id为vm_id，目标服务器下标rank
    void log_a_migration(int vm_id, int server_rank) {
        vm_id_server_rank_for_migration.emplace_back(make_pair(vm_id, server_rank), 'N');
    }

//  做了一次单节点迁移
//  虚拟机id为vm_id，目标服务器下标rank，节点 node
    void log_a_migration(int vm_id, int server_rank, int node) {
        vm_id_server_rank_for_migration.emplace_back(make_pair(vm_id, server_rank), node == 0 ? 'A' : 'B');
    }

//  做了一次调度，虚拟机的下标为vm_rank
    void log_a_vm_deployment(int vm_rank) {
        VirtualMachine& vm = virtualMachine[vm_rank];
        VirtualMachineInformation& vmInfo= virtualMachineInformation[vm.type];

        if (vmInfo.isDoubleNode) {
            server_rank_for_deployment.emplace_back(vm.serverNum, 'N');
        } else {
            server_rank_for_deployment.emplace_back(vm.serverNum, vm.nodeNum == 0 ? 'A' : 'B');
        }
    }

    void call_an_end_to_this_day() {
        // 存入购买信息

//        cout << "bucket: //////////////////////////////////////" << endl;

        for (int i = 0; i < N; i++) {
            if (bucket_server[i] != 0) {
                dailyAction.insertPurchase(make_pair(string(serverInformation[i].typeName), bucket_server[i]));
            }

        }

        // 计算前缀和
//        cout << "type : " << 0 << " , bucket: " << bucket_server[0] << ", sum: " << prefix_sum[0] <<endl;

        prefix_sum[0] = bucket_server[0];
        for (int i = 1; i < N; i++) {
            prefix_sum[i] = prefix_sum[i - 1] + bucket_server[i];

//            cout << "type : " << i << " , bucket: " << bucket_server[i] << ", sum: " << prefix_sum[i] <<endl;
        }


//        cout << "bucket: //////////////////////////////////////\n" << endl;

//      计算服务器rank到id的映射，同一类型，最先购买的id会更大
//        cout << "====================================" << endl;

        int tmp_type = 0;
        for (auto x : server_rank_type_vec) {
            tmp_type = x.second;
            server_rank_id_map[x.first] = total_server + (tmp_type == 0 ? 0 : prefix_sum[tmp_type - 1]) + bucket_server[tmp_type] - 1;
            bucket_server[tmp_type] --;


//            cout << "rank: " << x.first << ",  type: " << x.second << ", id: " << server_rank_id_map[x.first] << endl;
        }

//        cout << "====================================" << endl;

        total_server += prefix_sum[N - 1];

//      计算调度信息
        for (auto deploy : server_rank_for_deployment) {
            dailyAction.insertDeployment(make_pair(server_rank_id_map[deploy.first], deploy.second));
        }

//      计算迁移信息
        for (auto migration : vm_id_server_rank_for_migration) {
            dailyAction.insertMigration(make_pair(make_pair(migration.first.first, migration.first.second), migration.second));
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
