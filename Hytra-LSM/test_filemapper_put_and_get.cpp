//
// Created by Ria on 2023/6/6.
//

#include <iostream>
#include "DiskRUN.h"

//extern DiskRUN* diskRun;
//extern void DiskRUN::add_batch(std::set<std::string> s);
//extern std::vector<std::string> DiskRUN::get_all_elements();


int main() {
    unsigned int _run_element_length = 10;
    std::string key = "2023";
    unsigned int _level = 0;
    unsigned int _run_size_threshold =  3;

    DiskRUN* diskrun = new DiskRUN(_run_element_length,key,_level,_run_size_threshold);

    std::set<std::string> s;
    s.insert("22");
    s.insert("33");
    s.insert("44");

    diskrun->add_batch(s);

    std::vector<std::string> result = diskrun->get_all_elements();

    for (std::string s: result) {
        std::cout << std::endl;
    }
}