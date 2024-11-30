//
// Created by rauchy on 2022/3/11.
//

#ifndef INVERTED_LSM_SERVER_H
#define INVERTED_LSM_SERVER_H

#include "LSM.h"
#include <map>

class Server {
public :
    Server(LSM *l);

    ~Server();

    void newConfig(std::string date, std::string path);

    void start();

private:
    LSM *_l;
    std::map<std::string, LSM *> _map;
    int _listenfd;

    void handlePut(char buf[], int conn, int len);

    void handleGet(char buf[], int conn, int len);

    void handleStatus(char buf[], int conn, int len);

    void handleConfig(char buf[], int conn, int len);
};


#endif //INVERTED_LSM_SERVER_H
