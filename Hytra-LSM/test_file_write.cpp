//
// Created by Ria on 2023/6/7.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <string>
#include <set>

void sys_err(int ret, const char *str)
{
    if(ret == -1)
    {
        perror(str);
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    char *p = NULL;
//    int fd = open("mmaptest", O_RDWR | O_CREAT | O_TRUNC ,S_IRWXU|S_IRWXG); // 打开文件
    int fd = open("mmaptest", O_RDWR | O_CREAT,S_IRWXU|S_IRWXG); // 打开文件
    sys_err(fd, "open error");

    size_t filesize = 30;
//    ftruncate(fd, filesize); // 扩展文件大小

    int len = lseek(fd, 0, SEEK_END); // 获取文件大小，作为 mmap() 的第二个参数
    printf("%d\n", len);

    p = (char*)mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if(p == MAP_FAILED) // 出错判断
    {
        perror("mmap error");
        exit(1);
    }


    std::set<std::string> values;
    values.insert("2");
    values.insert("3");
//    values.insert("4");

    int offset = 0;
    for(std::string s:values){
        const char* c = s.c_str();
        int l = strlen(c);
        printf("%d\n", l);
        memcpy(p+offset, c, strlen(c));
        offset += 20;
    }


    filesize *= 2;
    ftruncate(fd, filesize);
//    memcpy(p, "1", 1); // 用首地址向这块区域中写内容
//    memcpy(p+8, "2", 1);
//    printf("%s", p+4); // 用首地址获取这块区域的内容

    int ret = munmap(p, len); // 释放内存映射
    sys_err(ret, "munmap error");

        close(fd);

    return 0;
}
