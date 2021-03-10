//
// Created by kvrmnks on 2021/3/10.
//

#ifndef PROJECT_INPUT_H
#define PROJECT_INPUT_H

#include "DataStructure.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
// 读取一条服务器的配置信息
// 返回一个ServerInformation结构体 表示这个服务器的信息
ServerInformation readOneServer()
{
    ServerInformation serverType;
    static char buf[30];
    static int cnt = 0;
    cnt = 0;

    // 读取服务器的类型名
    char ch = getchar();
    while(ch != '('){ch = getchar();}
    ch = getchar();
    while(ch != ','){buf[cnt++] = ch; ch = getchar();}
    for(int i = 0; i < cnt; i++){serverType.typeName[i] = buf[i];}
    serverType.typeName[cnt] = 0;
    puts(serverType.typeName);

    // 读取服务器的核心数
    int number = 0;
    while(ch < '0' || ch > '9'){ch = getchar();}
    while(ch != ','){number = number * 10 + ch - '0'; ch = getchar();}
    serverType.coreNum = number;

    // 读取服务器的内存大小
    number = 0;
    while(ch < '0' || ch > '9'){ch = getchar();}
    while(ch != ','){number = number * 10 + ch - '0'; ch = getchar();}
    serverType.memorySize = number;

    // 读取服务器的硬件设施成本
    number = 0;
    while(ch < '0' || ch > '9'){ch = getchar();}
    while(ch != ','){number = number * 10 + ch - '0'; ch = getchar();}
    serverType.hardwareCost = number;

    // 读取服务器的每日花费
    number = 0;
    while(ch < '0' || ch > '9'){ch = getchar();}
    while(ch != ')'){number = number * 10 + ch - '0'; ch = getchar();}
    serverType.dayCost = number;

    return serverType;
}

// 读取一条虚拟机的配置信息
// 返回一个VirtualMachineInformation结构体 表示这个虚拟机的信息
VirtualMachineInformation readOneVirtualMachine()
{
    VirtualMachineInformation vm;
    static char buf[30];
    static int cnt = 0;
    cnt = 0;

    // 读取虚拟机的类型名
    char ch = getchar();
    while(ch != '('){ch = getchar();}
    ch = getchar();
    while(ch != ','){buf[cnt++] = ch; ch = getchar();}
    for(int i = 0; i < cnt; i++){vm.typeName[i] = buf[i];}
    vm.typeName[cnt] = 0;

    // 读取虚拟机的核心数
    int number = 0;
    while(ch < '0' || ch > '9'){ch = getchar();}
    while(ch != ','){number = number * 10 + ch - '0'; ch = getchar();}
    vm.coreNum = number;

    // 读取虚拟机的内存大小
    number = 0;
    while(ch < '0' || ch > '9'){ch = getchar();}
    while(ch != ','){number = number * 10 + ch - '0'; ch = getchar();}
    vm.memorySize = number;

    // 读取虚拟机是否需要双节点部署
    number = 0;
    while(ch < '0' || ch > '9'){ch = getchar();}
    while(ch != ')'){number = number * 10 + ch - '0'; ch = getchar();}
    vm.isDoubleNode = number;

    return vm;
}

Require readOneRequire()
{
    Require re;
    static char buf[30];
    static int cnt = 0;
    cnt = 0;

    // 读取虚拟机的类型名
    char ch = getchar();
    while(ch != '('){ch = getchar();}
    ch = getchar();
    while(ch != ','){buf[cnt++] = ch; ch = getchar();}
    buf[cnt] = 0;
    auto s = std::string(buf);

    re.type = s == "add" ? 0 : 1;
    if(re.type == 0){
        cnt = 0;
        ch = getchar();
        while(ch == ' ')ch = getchar();
        while(ch != ','){buf[cnt++] = ch; ch = getchar();}
        for(int i = 0; i < cnt; i ++)
        {
            re.virtualMachineName[i] = buf[i];
        }
        re.virtualMachineName[cnt] = 0;

        int number = 0;
        while(ch < '0' || ch > '9'){ch = getchar();}
        while(ch != ')'){number = number * 10 + ch - '0'; ch = getchar();}
        re.id = number;
    }else{
        re.virtualMachineName[0] = 0;
        int number = 0;
        while(ch < '0' || ch > '9'){ch = getchar();}
        while(ch != ')'){number = number * 10 + ch - '0'; ch = getchar();}
        re.id = number;
    }
    return re;
}
#endif //PROJECT_INPUT_H
