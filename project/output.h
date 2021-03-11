//
// Created by Lenovo on 2021/3/11.
//

#ifndef PROJECT_OUTPUT_H
#define PROJECT_OUTPUT_H

const int MAX_DAY = 1000;

struct PurchaseInfo {
    unsigned Q; // type number of server, smaller than N

    std::vector<std::pair<string, unsigned> > serverNumPerType;
}Purchase[MAX_DAY];

struct Migration {
    unsigned W; // vm number about to migrate

    std::vector<std::pair<unsigned, unsigned> > migrationInfo;
};

#endif //PROJECT_OUTPUT_H
