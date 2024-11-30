//
// Created by rauchy on 2022/3/6.
//

#ifndef INVERTED_LSM_LSMUTILS_H
#define INVERTED_LSM_LSMUTILS_H

#include<string>
#include <fstream>
#include <iostream>
#include <cstring>
#include "LSM.h"


LSM *load_lsm_config(std::string config_file) {
//    std::ifstream  put_file("./config")
    std::ifstream config(config_file);
    if(!config) std::cerr << "Error: " << strerror(errno);

    std::string line;
    std::map<std::string, std::string> merge_map;
    std::vector<std::vector<std::string>> keys_per_level;
    std::vector<unsigned int> element_size_threshold_per_level;

    std::vector<unsigned int> runs_per_level;

    unsigned int mask = 0;
    unsigned int element_length = 0;


    if (config) {
        while (getline(config, line)) {
            if(!line.empty() && line.find('\r')==line.length()-1){
                line.erase(line.length()-1);
            }

            if (line == "merge_map") {
                mask = 0;
            } else if (line == "keys_per_level") {
                mask = 1;
            } else if (line == "element_size_threshold_per_level") {
                mask = 2;
            } else if (line == "element_length_per_level") {
                mask = 3;
            } else if (line == "") {
                continue;
            } else {
                unsigned int l = line.find(":");
                std::string key = line.substr(0, l);
                std::string value = line.substr(l + 1, line.size() - l - 1);
                switch (mask) {
                    case 0: {
                        // 记录上一层中的 run 与下一层哪一个 run 合并；一个 run 对应一个 key（cubeid）
                        merge_map[key] = value;
                    }
                        break;

                    case 1: {
                        // 记录每一层的 run 对应的 key 值；相当于每个 level 有哪些 cubeid
                        unsigned int i = std::atoi(key.c_str());
                        while (keys_per_level.size() <= i) {
                            keys_per_level.push_back(std::vector<std::string>());
                        }
                        keys_per_level[i].push_back(value);
                    }
                        break;

                    case 2: {
                        // 记录每一层的 run 对应的元素数量阈值
                        unsigned int j = std::atoi(value.c_str());
                        element_size_threshold_per_level.push_back(j);
                    }
                        break;
                    case 3: {
                        // 所有的 run 的 element_size 设为相同
                        unsigned int k = std::atoi(value.c_str());
                        element_length = k;
                    }
                        break;
                }
            }
        }
    }
    std::vector<unsigned int> element_length_per_level(element_size_threshold_per_level.size(), element_length);
    runs_per_level.reserve(element_size_threshold_per_level.size());
    for (unsigned int i = 0; i < keys_per_level.size(); i++) {
        runs_per_level.push_back(keys_per_level[i].size());
    }
    LSM *l = new LSM(merge_map, keys_per_level, element_size_threshold_per_level, element_length_per_level,
                     runs_per_level);
    printf("Load lsm config\n");
    return l;
}

LSM *load_default_lsm_config() {
    return load_lsm_config("./config/cubetrip.config");
}

#endif //INVERTED_LSM_LSMUTILS_H
