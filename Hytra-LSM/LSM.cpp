//
// Created by rauchy on 2022/3/6.
//

#include "LSM.h"

LSM::LSM(std::map<std::string, std::string> merge_maps, std::vector<std::vector<std::string>> keys_per_level,
         std::vector<unsigned int> element_size_threshold_per_level, std::vector<unsigned int> element_length_per_level,
         std::vector<unsigned int> runs_per_level)
        : _merge_maps(merge_maps), _keys_per_level(keys_per_level),
          _element_length_per_level(element_length_per_level),
          _element_size_threshold_per_level(element_size_threshold_per_level),
          _runs_per_level(runs_per_level) {

    // diskrun的层数，包括memrun
    unsigned int level_num = _runs_per_level.size();

    // 不直接初始化，而是等到真实读写时再创建对应的run,避免创建大量空run浪费资源
//    for (unsigned int i = 0; i < level_num; i++) {
//
//        if (i == 0) { // 初始化C_0
//            std::vector<std::string> keys = _keys_per_level[i];
//            unsigned int ele_length = _element_length_per_level[i];
//            unsigned int ele_size_threshold = _element_size_threshold_per_level[i];
//            _mem_level = new MemLevel(ele_length, ele_size_threshold, keys);
//
//        } else {  // 初始化 Disk level
//            // i  = 1, 2, ..., level_num - 1
//            // disklevel: i - 1; _keys_per_level: i
//            std::vector<std::string> keys = _keys_per_level[i];
//            unsigned int ele_length = _element_length_per_level[i];
//            unsigned int ele_size_threshold = _element_size_threshold_per_level[i];
//            unsigned int run_size = _runs_per_level[i];
//            DiskLevel *diskLevel = new DiskLevel(i - 1, run_size, ele_size_threshold, ele_length, keys);
//            _disk_levels.push_back(diskLevel);
//        }
//    }

    // 2023.06.05 modified by wty
    for (unsigned int i = 0; i < level_num; i++) {
        if (i == 0) { // 初始化C_0
            std::vector<std::string> keys = _keys_per_level[i];
            unsigned int ele_length = _element_length_per_level[i];
            unsigned int ele_size_threshold = _element_size_threshold_per_level[i];
            _mem_level = new MemLevel(ele_length, ele_size_threshold, keys);
        }

        std::vector<std::string> keys = _keys_per_level[i];
        unsigned int ele_length = _element_length_per_level[i];
        unsigned int ele_size_threshold = _element_size_threshold_per_level[i];
        unsigned int run_size = _runs_per_level[i];
        DiskLevel *diskLevel = new DiskLevel(i, run_size, ele_size_threshold, ele_length, keys);
        _disk_levels.push_back(diskLevel);
    }

}

LSM::~LSM() {

    for (auto it: _disk_levels) {
        delete it;
    }
    delete _mem_level;
}

// 2023.06.05 created by wty
bool LSM::shouldExpand (DiskRUN *to_merge_run) {
    return (_disk_levels.size() <= 1 || (_merge_maps.count(to_merge_run->get_key()) != 1) ||
    _merge_maps[to_merge_run->get_key()]==to_merge_run->get_key());
}

// 传参 memrun 和 diskrun
void LSM::merge_memruns_to_level(MemRUN *cur_run, DiskRUN *to_merge_run) {
    std::vector<std::string> cur_values = cur_run->get_all_elements();   // 获取当前要合并的 run 的所有值
    std::vector<std::string> values = to_merge_run->get_all_elements();  // 获取将要合并到的 run 的所有值
    unsigned int ele_length = _element_length_per_level[0]; // 获取 memrun 的元素长度
    std::set<std::string> new_value_set;                    // 新的值的集合

    // 把 memrun 和 diskrun 的值都加入到新的集合中去
    for (const auto &s: cur_values) {
        new_value_set.insert(s);
    }

    for (const auto &s: values) {
        new_value_set.insert(s);
    }
    unsigned int new_element_num = new_value_set.size();   // 获取新的集合的 size，即集合中值的数量
    unsigned int new_size = new_element_num * ele_length;  // 集合中值的数量 * 值的长度

    to_merge_run->clear();  // 要合并到的 diskrun 清空

    // 合并后的值写入到下一层中，如果超过设定的阈值则需要临时扩容
    if (new_element_num > to_merge_run->get_vsize_threshold()) {
        to_merge_run->adjust_size(new_size);
    }

    // 按批次写入新的值
    to_merge_run->add_batch(new_value_set);

    //清空当前run
    cur_run->clear();

    printf("merge level 0 mem run :%s to level 0 disk run :%s\n", cur_run->get_key().c_str(),
           to_merge_run->get_key().c_str());

    // 看合并到的 diskrun 是不是也满了，从而决定是否继续合并到下一层
    if (to_merge_run->isfull()) {

        if (shouldExpand(to_merge_run)) {
            to_merge_run -> expand();
        } else {
            // 如果当前 run 在下一层有对应的run进行合并，则合并到下一层
            std::string next_key = _merge_maps[to_merge_run->get_key()];
            DiskRUN *next_run = _disk_levels[1]->get_run(next_key);
            if( next_run == NULL) {
                next_run = _disk_levels[1]->create_run(next_key);
            }
            merge_next_level(to_merge_run, next_run, 1);// cur_run->clear();
        }

//        if ((_merge_maps.count(to_merge_run->get_key()) == 1) && _disk_levels.size() > 1) {
//            if(_merge_maps[to_merge_run->get_key()]==to_merge_run->get_key()){
//                to_merge_run->expand();
//
//            }
//            else {
//                DiskRUN *next_run = _disk_levels[1]->get_run(_merge_maps[to_merge_run->get_key()]);
//
//                merge_next_level(to_merge_run, next_run, 2);// cur_run->clear();
//            }
//        }
//
//            // 否则，当前run当作最后一层直接在当前映射文件基础上扩容并调整阈值
//        else {
//            to_merge_run->expand();
//        }
    }

}

void LSM::merge_next_level(DiskRUN *cur_run, DiskRUN *to_merge_run, unsigned int next_level) {
    std::vector<std::string> cur_values = cur_run->get_all_elements();
    std::vector<std::string> values = to_merge_run->get_all_elements();
    unsigned int ele_length = _element_length_per_level[0];

    std::set<std::string> new_value_set;

    for (const auto &s: cur_values) {
        new_value_set.insert(s);
    }

    for (const auto &s: values) {
        new_value_set.insert(s);
    }

    unsigned int new_element_num = new_value_set.size();
    unsigned int new_size = new_element_num * ele_length;

    to_merge_run->clear();

    // 合并后的值写入到下一层中，如果超过设定的阈值则需要临时扩容
    if (new_element_num > to_merge_run->get_vsize_threshold()) {
        to_merge_run->adjust_size(new_size);
        printf("resize diskrun key :%s level: %d\n", to_merge_run->get_key().c_str(), to_merge_run->get_level());
    }

    to_merge_run->add_batch(new_value_set);

    //清空当前run
    cur_run->clear();

    printf("merge level %d disk run :%s to level %d disk run :%s\n",
           cur_run->get_level(), cur_run->get_key().c_str(), to_merge_run->get_level(),
           to_merge_run->get_key().c_str());

    if (to_merge_run->isfull()) { // 决定是否合并到下一层
        // 如果当前是最后一层或者当前run没有下一层run与之合并，
        // 当前run直接扩容
        if ((next_level > _disk_levels.size() - 1)
            || (_merge_maps.count(to_merge_run->get_key()) == 0)) {
            to_merge_run->expand();
            printf("expand diskrun key :%s level: %d\n", to_merge_run->get_key().c_str(), to_merge_run->get_level());

        }
            // 否则，需要合并到下一层对应的run
        else {
            std::string next_key = _merge_maps[to_merge_run->get_key()];
            // 2023.06.10 写错了，是 next_level + 1；原来写的是 next_level
            DiskRUN *next_run = _disk_levels[next_level + 1]->get_run(next_key);

            if( next_run == NULL) {
                next_run = _disk_levels[next_level + 1]->create_run(next_key);
            }

            merge_next_level(to_merge_run, next_run, next_level + 1);
        }

    }

}


std::set<std::string> LSM::get_items_for_key(std::string key) {
    // 对应于lazy模式，需要全局寻找然后返回对应的全局索引项
    std::set<std::string> result;
    // 如果C_0包含待查询的key，则先从C_0层开查找，并向下查找与C_0相关联的diskrun，直到全部返回
    if (_mem_level->contains_key(key)) {
        std::vector<std::string> temp = _mem_level->get_memrun(key)->get_all_elements();
        for (std::string s: temp) {
            result.insert(s);
        }

        // 向下一层的disklevel寻找，如果当前key在下一层有对应的diskrun与之对应，
        // 则将下一层diskrun的全部元素加入到结果集中，并继续向下一层寻找，直至到达最后一层或者
        // 下一层没有上一层对应的diskrun
        unsigned int level = 0;
        while ((_merge_maps.count(key) == 1) && (level <= _disk_levels.size() - 1)) {
            if (level != 0) {
                key = _merge_maps[key];
            }
            DiskRUN *diskRun = _disk_levels[level]->get_run(key);
            if (diskRun == NULL) {
                level++;
                continue;
            }

            std::vector<std::string> tmp = diskRun->get_all_elements();
            for (std::string s: tmp) {
                result.insert(s);
            }
            level++;
        }

    }
    return result;
}


void LSM::insert_kv(const std::string &k, std::string v) {
    // 采用lazy模式进行更新，直接插入m记录
    MemRUN *memrun = _mem_level->get_memrun(k);
    memrun->add_v(v);

    if (memrun->is_full()) {
        if (_merge_maps.count(k) == 0) {
            printf("No diskrun for memrun key : %s\n", k.c_str());
        } else {
            // 取出磁盘第 0 层中 key = 2023-05-20@894@1 的 run
//            DiskRUN *diskrun = _disk_levels[0]->get_run(_merge_maps[k]);

            // 2023.06.05 modified by wty
            // 一个持久化的过程，先放到第 0 层的 diskrun 里面，要合并后面再操作
            DiskRUN *diskrun = _disk_levels[0]->get_run(k);

            if( diskrun == NULL) {
                diskrun = _disk_levels[0]->create_run(k);
            }

            merge_memruns_to_level(memrun, diskrun);
        }
    }

}

void LSM::delete_kv(std::string k, std::string v) {

}
