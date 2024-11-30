//
// Created by rauchy on 2022/3/11.
//

#include "Server.h"
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

extern LSM *load_lsm_config(std::string path);


const std::string RES_OK = "Insert OK!\n";
const std::string RES_ERR = "Insert error\n";

Server::Server(LSM *l) : _l(l) {
    _listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9201);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(_listenfd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("Bind error!\n");
        return;
    }

    if (listen(_listenfd, 5) == -1) {
        perror("Listen error!\n");
        return;
    }
}

Server::~Server() {
    shutdown(_listenfd, SHUT_RDWR);
    delete _l;
}

void Server::newConfig(std::string date, std::string path) {
    LSM *l = load_lsm_config(path);
    _l = l;
    // 如果这个 _map 里面已经包含了日期 date，那就释放这片内存
    if(_map.count(date) == 1) {
        free(_map[date]);
    }
    printf("Add new tree for %s\n", date.c_str());
    // _map 里再加入这个 date 对应的 lsmtree
    _map[date] = l;
}

void Server::start() {
    int conn;
    char clientIP[INET_ADDRSTRLEN] = "";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    while (true) {
        conn = accept(_listenfd, (struct sockaddr *) &clientAddr, &clientAddrLen);
        if (conn < 0) {
            perror("Accept error!\n");
            continue;
        }
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        printf("Connect from %s\n", clientIP);
        char buf[255];
        while (true) {
            memset(buf, 0, sizeof(buf));
            int len = recv(conn, buf, sizeof(buf), 0);
            buf[len] = '\0';
            if (strncasecmp(buf, "exit", 4) == 0) { // 断开连接
                printf("Disonnect from %s\n", clientIP);
                break;
            } else if (strncasecmp(buf, "std", 3) == 0) { // 关闭服务端程序
                printf("Close socket\n");
                return;
            } else if (strncasecmp(buf, "put", 3) == 0) {// 添加kv对
                handlePut(buf, conn, len);
            } else if (strncasecmp(buf, "get", 3) == 0) {// 根据k查找
                handleGet(buf, conn, len);
            } else if (strncasecmp(buf, "status", 6) == 0) {// 返回存储状态
                handleStatus(buf, conn, len);
            } else if (strncasecmp(buf, "config", 6) == 0) {// 配置新配置数据
                handleConfig(buf, conn, len);
            } else { // 忽略
//                send(conn,res_err.c_str(),res_err.length(),0);
//                printf("Error input : %s \n",buf);
            }

        }

    }
}

void Server::handlePut(char buf[], int conn, int len) {
    std::string tmp = buf;
    unsigned int pos1 = tmp.find(":");
    unsigned int pos2 = tmp.find(",");
    std::string key = tmp.substr(pos1 + 1, pos2 - pos1 - 1);
    std::string value = tmp.substr(pos2 + 1, len - pos2);
    printf("Insert %s : %s\n", key.c_str(), value.c_str());
    _l->insert_kv(key, value);
    const char *a = RES_OK.c_str();
//    write (conn, RES_OK.c_str(), RES_OK.length()) ;
    send(conn, RES_OK.c_str(), RES_OK.length(), 0);
}

void Server::handleGet(char buf[], int conn, int len) {
    std::string tmp = buf;
    unsigned int pos1 = tmp.find(":");
    unsigned int pos2 = tmp.find(",");
    std::string key = tmp.substr(pos1 + 1, pos2 - pos1 - 1);
    std::string res_str;

    auto res = _l->get_items_for_key(key);
    for (auto s: res) {
        res_str.append(s);
        res_str.append(",");
    }
    res_str = res_str.substr(0, res_str.length() - 1);

    res_str.append("\n");
    send(conn, res_str.c_str(), res_str.length(), 0);
}

//void Server::handleGet(char buf[], int conn, int len) {
//    std::string tmp = buf;
//    unsigned int pos1 = tmp.find(":");
//    unsigned int pos2 = tmp.find(",");
//    std::string date = tmp.substr(pos1 + 1, pos2 - pos1 - 1);
//    std::string key = tmp.substr(pos2 + 1, len - pos2);
//    std::string res_str;
//    if(_map.count(date) == 1) {
//        auto res = _map[date]->get_items_for_key(key);
//        for (auto s: res) {
//            res_str.append(s);
//            res_str.append(",");
//        }
//        res_str = res_str.substr(0, res_str.length() - 1);
//    }
//    res_str.append("\n");
//    send(conn, res_str.c_str(), res_str.length(), 0);
//}

void Server::handleStatus(char buf[], int conn, int len) {
    std::string res_str;
    if (_l->_mem_level == NULL) {
        res_str.append("empty");
        res_str.append("\n");
        send(conn, res_str.c_str(), res_str.length(), 0);
        return;
    }
    auto mem_size = _l->_mem_level->get_size();
    res_str.append("mem:");
    res_str.append(std::to_string(mem_size));
    res_str.append(",");
    auto level_num = _l->_disk_levels.size();
    for (int i = 0; i < level_num; i++) {
        res_str.append("disk");
        res_str.append(std::to_string(i));
        res_str.append(":");
        res_str.append(std::to_string(_l->_disk_levels[i]->get_size()));
        res_str.append(",");
    }
    res_str = res_str.substr(0, res_str.length() - 1);
    res_str.append("\n");
    send(conn, res_str.c_str(), res_str.length(), 0);
}

void Server::handleConfig(char buf[], int conn, int len) {
    std::string tmp = buf;
    unsigned int pos1 = tmp.find(":");
    unsigned int pos2 = tmp.find(",");
    std::string date = tmp.substr(pos1 + 1, pos2 - pos1 - 1);
    std::string path = tmp.substr(pos2 + 1, len - pos2);
    try {
        newConfig(date, path);
        send(conn, RES_OK.c_str(), RES_OK.length(), 0);
    } catch (const char *s) {
        std::string msg = s;
        send(conn, msg.c_str(), msg.length(), 0);
        printf("config error: %s", s);
    } catch (const std::exception &e1) {
        std::string msg = e1.what();
        send(conn, msg.c_str(), msg.length(), 0);
        printf("config error %s", e1.what());
    }
}
