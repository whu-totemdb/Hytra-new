//
// Created by rauchy on 2022/3/6.
//

#include "FileMapper.h"
#include<sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include<cstring>
#include <stdio.h>
#include <unistd.h>


FileMapper::FileMapper(std::string file_name,int element_size_threshold,int element_length)
:_file_name(file_name),_element_size_threshold(element_size_threshold),_element_length(element_length) {

    file_size = element_length * element_size_threshold;
    _cur_size = 0;
    open_file();
    ftruncate(_fd,file_size);
    int len = lseek(_fd, 0, SEEK_END);
    close_file();
}

FileMapper::~FileMapper(){

//    remove(("index_file/"+_file_name).c_str());

}

// 多次单个元素读取效率低，弃用
std::string FileMapper::read_element(unsigned int ele_index){

    open_file();
    do_map();
    size_t offset = ele_index*_element_length;
    char* val = new char[_element_length];
    memcpy(val,_fmap+offset,_element_length);
    std::string s = val;
    delete[] val;
    un_map();
    close_file();
    return s;

}

// 删除文件并将文件内的元素数量置为0
void FileMapper::clear(){
    remove(("index_file/"+_file_name).c_str());
    _cur_size = 0;
}

// 按批次读取文件，加快读取速度
std::vector<std::string> FileMapper::read_all_element(){
    std::vector<std::string> s;
    if (_fd < 0) {
        return s;
    }

    open_file();
//    ftruncate(_fd,file_size); // 读取文件为什么还分配大小？？？
    do_map();
    size_t offset=0;
    char* val = new char[_element_length];
    for(unsigned int i=0;i<_cur_size;i++){
        offset = i*_element_length;
        memcpy(val,_fmap+offset,_element_length);
        std::string tmp = val;
        s.push_back(tmp.substr(0,_element_length));
    }
    delete[] val;
    un_map();
    close_file();
    return s;
}

void FileMapper::expand_size(){
    open_file();

    file_size = _cur_size*_element_length*2;
    ftruncate(_fd,file_size);
    close_file();
    _element_size_threshold = _cur_size*2;
}

void FileMapper::adjust_size(unsigned int new_size) {
    open_file();
    file_size = new_size;
    ftruncate(_fd,file_size);
    close_file();

}

void FileMapper::do_map() {
    // 获得映射区的首地址
    _fmap = (char*) mmap(NULL, file_size,PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0);
}

void FileMapper::un_map() {
//    int ret = munmap(_fmap,file_size); // 释放内存映射
//    if(ret==-1){
//        perror("munmap error\n");
//    }
    fsync(_fd);
    if(munmap(_fmap,file_size)==-1){
        perror("unmap error\n");
    }
}

void FileMapper::open_file() {

//    _fd = open(("index_file/"+_file_name).c_str(),O_RDWR | O_CREAT | O_TRUNC ,(mode_t) 0600);
    // 2023.06.08 使用 O_TRUNC 时，每次打开文件文件内容会清空
//    _fd = open(("index_file/"+_file_name).c_str(),O_RDWR | O_CREAT | O_TRUNC ,S_IRWXU|S_IRWXG);
    _fd = open(("index_file/"+_file_name).c_str(),O_RDWR | O_CREAT,S_IRWXU|S_IRWXG);
    if(_fd == -1){
        perror(("Create file failed " + _file_name).c_str());
    }
}

void FileMapper::close_file() {
    close(_fd);
}

// 多次单个元素写入效率较低，弃用
void FileMapper::write_element(std::string element){
    open_file();
    do_map();
    size_t offset = _cur_size*_element_length;
    const char* ele = element.c_str();
    memcpy(_fmap+offset,ele,_element_length);
    _cur_size++;
    un_map();
    close_file();
}

// 按批次进行写入，提高写入效率
void FileMapper::write_batch(std::set<std::string> values) {
//    open_file(); // 打开文件，并生成文件标识符 _fd；mmap创建映射区完成后即可关闭，后续访问文件用指针/地址
//    ftruncate(_fd, file_size); // 扩展文件大小
//    do_map(); // mmap
//    size_t offset;
//    for(const auto& s:values){
//        offset = _cur_size * _element_length;
//        const char* tmp = s.c_str();
//        memcpy(_fmap, tmp, sizeof(tmp));
//        _cur_size++;
//    }
//    un_map();
//    close_file();

    open_file(); // 打开文件，并生成文件标识符 _fd；mmap创建映射区完成后即可关闭，后续访问文件用指针/地址
    ftruncate(_fd, file_size); // 扩展文件大小

    int len = lseek(_fd, 0, SEEK_END); // 获取文件大小，作为 mmap() 的第二个参数
    printf("%d   ", len);

    do_map(); // mmap

    if(_fmap == MAP_FAILED) // 出错判断
    {
        perror("mmap error");
        exit(1);
    }
//    close_file();

    int offset = 0;
    for(std::string s:values){
        offset = _cur_size * _element_length;
        const char* tmp = s.c_str();
        memcpy(_fmap+offset, tmp, strlen(tmp));
        _cur_size++;
    }

//    printf("%s   ", _fmap); // 用首地址获取这块区域的内容
//    printf("%s   ", _fmap+_element_length);

    un_map();
    close_file();

}
