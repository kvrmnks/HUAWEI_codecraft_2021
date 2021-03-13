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

// 记录所有天的决策信息
struct Actions {
private:
    vector<DailyAction> actions;
    int bucket_for_server[100 + 1];
    DailyAction dailyAction;
    bool fresh = false;
    bool stored = false;

    void check() {
#ifdef DEBUG
        if (!fresh) cerr << "oops! You haven't initialise the logger.";
#endif
    }
    void log_a_deploy(int target_server_id) {
        dailyAction.insertDeployment(make_pair(target_server_id, 'N'));
    }
    void log_a_deploy(int target_server_id, int node) {
        dailyAction.insertDeployment(make_pair(target_server_id, node == 0 ? 'A' : 'B'));
    }
public:
    // 初始化今天的信息
    void start_a_brand_new_day() {
        memset(bucket_for_server, 0, sizeof bucket_for_server);
        dailyAction = DailyAction();
        fresh = true;
        stored = false;
    }

//    买了一个类型为 type 的服务器
    void log_a_server(int type) {
        check();
        bucket_for_server[type] ++;
    }

//    买了一个名字为 name 的服务器
    void log_a_server(string name) {
        check();
        bucket_for_server[mpSevere[name]] ++;
    }

//    做了一次双节点的迁移，将id为 vm_id 的虚拟机迁移到 id 为 target_server_id 的服务器
    void log_a_migration(int vm_id, int target_server_id) {
        dailyAction.insertMigration(make_pair(make_pair(vm_id, target_server_id), 'N'));
    }

//    做了一次单节点的迁移，将id为 vm_id 的虚拟机迁移到 id 为 target_server_id 的服务器，节点为 node (0 \ 1)
    void log_a_migration(int vm_id, int target_server_id, int node) {
        dailyAction.insertMigration(make_pair(make_pair(vm_id, target_server_id), node == 0 ? 'A' : 'B'));
    }

//    给下标为vm_rank的虚拟机做了一次调度
    void log_a_vm_deployment(int vm_rank) {
        VirtualMachine& vm = virtualMachine[vm_rank];
        VirtualMachineInformation& vmInfor= virtualMachineInformation[vm.type];

        if (vmInfor.isDoubleNode) {
            log_a_deploy(vm.serverNum);
        } else {
            log_a_deploy(vm.serverNum, vm.nodeNum);
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
