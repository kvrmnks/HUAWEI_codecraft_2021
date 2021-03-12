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

struct PurchaseInfo {
    vector<pair<string, unsigned> > serverNumPerType; // type name, quantity

    friend ostream &operator << (ostream &output, PurchaseInfo& purchaseInfo) {
        output << "(purchase," << purchaseInfo.serverNumPerType.size() << ")" << endl;
        for(auto x : purchaseInfo.serverNumPerType) {
            output << '(' << x.first << ',' << x.second << ')' << endl;
        }
        return output;
    }
};

struct Migration {
    vector<pair<pair<unsigned, unsigned>, char> > migrationInfo; // vm_id, server_id,  (node : 'N', 'A', 'B');

    friend ostream &operator << (ostream &output, Migration& migration) {
        output << "(migration," << migration.migrationInfo.size() << ")" << endl;
        for(auto x : migration.migrationInfo) {
            auto basicInfo = x.first;
            if (x.second == 'N') {
                output << '(' << basicInfo.first << ',' << basicInfo.second << ')' << endl;
            } else {
                output << '(' << basicInfo.first << ',' << basicInfo.second << ',' << x.second << ')' << endl;
            }
        }
        return output;
    }
};

struct DeployRequest {
    vector<pair<unsigned, char> > deployments; // server_id, (node : 'N', 'A', 'B')

    friend ostream &operator << (ostream &output, DeployRequest& deployRequest) {
        for(auto x : deployRequest.deployments) {
            if (x.second == 'N') {
                output << '(' << x.first << ')' << endl;
            } else {
                output << '(' << x.first << ',' << x.second << ')' << endl;
            }
        }
        return output;
    }
};

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

struct Actions {
private:
    vector<DailyAction> actions;
    int bucket_for_server[100 + 1];
    DailyAction dailyAction;
    bool fresh = false;
    bool stored = false;

    void check() {
        if (!fresh) cerr << "oops! You haven't initialise the logger.";
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

    void log_a_server(int type) {
        check();
        bucket_for_server[type] ++;
    }
    void log_a_server(string name) {
        check();
        bucket_for_server[mpSevere[name]] ++;
    }

    void log_a_migration(int vm_id, int target_server_id) {
        dailyAction.insertMigration(make_pair(make_pair(vm_id, target_server_id), 'N'));
    }
    void log_a_migration(int vm_id, int target_server_id, int node) {
        dailyAction.insertMigration(make_pair(make_pair(vm_id, target_server_id), node == 0 ? 'A' : 'B'));
    }

    void log_a_vm_deployment(int vm_rank) {
        VirtualMachine& vm = virtualMachine[vm_rank];
        VirtualMachineInformation& vmInfor= virtualMachineInformation[vm.type];

        if (vmInfor.isDoubleNode) {
            log_a_deploy(vm.serverNum);
        } else {
            log_a_deploy(vm.serverNum, vm.nodeNum);
        }
    }

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

    void print() {
        for (auto x : actions) {
            cout << x;
        }
    }
};

#endif //PROJECT_OUTPUT_H
