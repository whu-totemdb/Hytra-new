//
// Created by Ria on 2023/6/8.
//

#include <iostream>
#include "FileMapper.h"


int main() {
    int size =3;
    int length = 10;

//    std::cout << 3;

    FileMapper* fileMapper = new FileMapper("filemapper", size, length);

    std::set<std::string> values;
    values.insert("1");
    values.insert("2");
    fileMapper->write_batch(values);
    fileMapper->open_file();
    fileMapper->close_file();
//    fileMapper->write_element(s);
}