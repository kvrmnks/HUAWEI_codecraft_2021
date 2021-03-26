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
    vector<pair<string, int> > serverNumPerType; // type name, quantity

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
    vector<pair<pair<int, int>, char> > migrationInfo; // vm_id, server_id,  (node : 'N', 'A', 'B');

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
    bool flag = false;
    DeployRequest(bool output_rank) {
        flag = output_rank;
    }
    DeployRequest(){}

    vector<pair<int, pair<int, char> > > deployments; // server_id, (node : 'N', 'A', 'B')

    friend ostream &operator << (ostream &output, DeployRequest& deployRequest) {
        for(auto x : deployRequest.deployments) {
            if (deployRequest.flag) {
                if (x.second.second == 'N') {
                    output << "(vm_rank: " << x.first << ", server: " << x.second.first << ')' << endl;
                } else {
                    output << "(vm_rank: " << x.first << ", server: " << x.second.first << ", node: " << x.second.second << ')' << endl;
                }
            } else {
                if (x.second.second == 'N') {
                    output << '(' << x.second.first << ')' << endl;
                } else {
                    output << '(' << x.second.first << ", " << x.second.second << ')' << endl;
                }
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

    DailyAction (bool isDebugging) {
        purchase = PurchaseInfo();
        migration = Migration();
        deployment = DeployRequest(isDebugging);
    }
    DailyAction(){}

    void insertPurchase (pair<string, int> p) {
        purchase.serverNumPerType.push_back(p);
    }
    void insertMigration (pair<pair<int, int>, char> m) {
        migration.migrationInfo.push_back(m);
    }
    void insertDeployment (pair<int ,pair<int, char> > d) {
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
    bool flag = false;

    vector<DailyAction> actions;
    DailyAction dailyAction;

    int total_server = 0;

    int bucket_server[MAX_TYPE];
    int prefix_sum[MAX_TYPE];

    vector<pair<int, int> > server_rank_type_vec;
    vector<pair<int, pair<int, char> > > deployInformation;
    vector<pair<pair<int, int>, char> > vm_id_server_rank_for_migration;

    map<int, int> server_rank_id_map;
    map<int, pair<int, int> > server_id_rank_map;
public:
    Actions(bool isDebugging) {
        flag = isDebugging;
    }
    Actions(){}

    void start_a_brand_new_day() {
        dailyAction = DailyAction(flag);

        for (int i = 0; i < N; i++) {
            bucket_server[i] = 0;
            prefix_sum[i] = 0;
        }
        server_rank_type_vec.clear();
        deployInformation.clear();
        vm_id_server_rank_for_migration.clear();
    }

    // 购买了一台服务器，下标为rank，类型为 type
    void log_a_server(int rank, int type) {
        server_rank_type_vec.emplace_back(rank, type);
        bucket_server[type] ++;
    }

//  做了一次双节点迁移
//  虚拟机下标为vm_rank，目标服务器下标server_rank
    void log_a_migration(int vm_rank, int server_rank) {
        VirtualMachine& vm = virtualMachine[vm_rank];

        vm_id_server_rank_for_migration.emplace_back(make_pair(vm.id, server_rank), 'N');
    }

//  做了一次单节点迁移
//  虚拟机下标为vm_rank，目标服务器下标server_rank，节点 node
    void log_a_migration(int vm_rank, int server_rank, int node) {
        VirtualMachine& vm = virtualMachine[vm_rank];

        vm_id_server_rank_for_migration.emplace_back(make_pair(vm.id, server_rank), node == 0 ? 'A' : 'B');
    }

//  做了一次调度，虚拟机的下标为vm_rank
    void log_a_vm_deployment(int vm_rank) {
        VirtualMachine& vm = virtualMachine[vm_rank];
        VirtualMachineInformation& vmInfo= virtualMachineInformation[vm.type];

        if (vmInfo.isDoubleNode) {
            deployInformation.emplace_back(vm_rank, make_pair(vm.serverNum, 'N'));
        } else {
            deployInformation.emplace_back(vm_rank, make_pair(vm.serverNum, vm.nodeNum == 0 ? 'A' : 'B'));
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
            server_id_rank_map[server_rank_id_map[x.first]] = make_pair(x.first, tmp_type);


//            cout << "rank: " << x.first << ",  type: " << x.second << ", id: " << server_rank_id_map[x.first] << endl;
        }

//        cout << "====================================" << endl;

        total_server += prefix_sum[N - 1];

//      计算调度信息
        for (auto deploy : deployInformation) {
            dailyAction.insertDeployment(make_pair(deploy.first, make_pair(server_rank_id_map[deploy.second.first], deploy.second.second)));
        }

//      计算迁移信息
        for (auto migration : vm_id_server_rank_for_migration) {
            dailyAction.insertMigration(make_pair(make_pair(migration.first.first, server_rank_id_map[migration.first.second]), migration.second));
        }

        actions.push_back(dailyAction);
    }

//    输出
    void print() {
        for (auto x : actions) {
            cout << x;
        }
    }

    int get_rank_of_server_with_id(int id) {
        if (server_id_rank_map.count(id) == 0) {
            //cerr << "No such id yet" << endl;
            exit(-999);
        }
        return server_id_rank_map[id].first;
    }

    void output_server_rank_id_map() {
        for (auto iter = server_id_rank_map.rbegin(); iter != server_id_rank_map.rend(); iter++) {
            cout << "server id: " << iter->first << " , server rank: " << iter->second.first << ", server type: " << iter->second.second << endl;
        }
    }

    void output_vmlist_of_server_with_id(int id) {
        auto server_data = server_id_rank_map[id];

        cout << "server id : " << id << ", server rank : " << server_data.first << ", server type: " << server_data.second << endl;

        cout << "server vm list: " << endl;

        auto list = server[server_data.first].vmList;

        for (int vm_rank : list) {
            VirtualMachine& vm = virtualMachine[vm_rank];

            cout << "id: " << vm.id << ", rank: " << vm.rank << ", type: " << vm.type << ", node: " <<vm.nodeNum << endl;
        }
    }
};

#endif //PROJECT_OUTPUT_H
