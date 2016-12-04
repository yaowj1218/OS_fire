//
//  main.c
//  OS_fire
//
//  Created by 姚文杰 on 2016/11/28.
//  Copyright © 2016年 姚文杰. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define  BLOCKSIZE  1024      //磁盘块大小
#define  SIZE         1024000   //虚拟磁盘空间大小
#define  END         65535     //FAT中的文件结束标志
#define  FREE        0         //FAT中盘块空闲标志
#define  ROOTBLOCKNUM  2   //根目录区所占盘块总数
#define  MAXOPENFILE    10   //最多同时打开文件个数

typedef struct FCB //仿照FAT16设置的
{
    char filename[8]; //文件名
    char exname[3];//文件扩展名
    unsigned char attribute;//文件属性字段：为简单起见，我们只为文件设置了两种属性：
    //值为0时表示目录文件，值为1时表示数据文件
    unsigned short time;//文件创建时间
    unsigned short data;//文件创建日期
    unsigned short first;//文件起始盘块号
    unsigned long length;//文件长度（字节数）
    char free;//表示目录项是否为空，若值为0，表示空，值为1，表示已分配
}fcb;

typedef struct FAT
{
    unsigned short id;
}fat;

typedef struct USEROPEN
{
    char filename[8]; //文件名
    char exname[3];//文件扩展名
    unsigned char attribute;//文件属性：值为0时表示目录文件，值为1时表示数据文件
    unsigned short time;//文件创建时间
    unsigned short data;//文件创建日期
    unsigned short first;//文件起始盘块号
    unsigned long length;//文件长度（对数据文件是字节数，对目录文件可以是目录项个数）
    char free;//表示目录项是否为空，若值为0，表示空，值为1，表示已分配
    //前面内容是文件的FCB中的内容。
    // 下面设置的dirno和diroff记录了相应打开文件的目录项在父目录文件中的位置，//这样如果该文件的fcb被修改了，则要写回父目录文件时比较方便
    int dirno;  //相应打开文件的目录项在父目录文件中的盘块号
    int diroff;// 相应打开文件的目录项在父目录文件的dirno盘块中的目录项序号
    char dir[MAXOPENFILE][80]; //相应打开文件所在的目录名，这样方便快速检查出
    //指定文件是否已经打开
    int count; //读写指针在文件中的位置
    char fcbstate; //是否修改了文件的FCB的内容，如果修改了置为1，否则为0
    char topenfile; //表示该用户打开表项是否为空，若值为0，表示为空，否则表示已
    //被某打开文件占据
}useropen;

typedef struct BLOCK0  //引导块内容
{
    //存储一些描述信息，如磁盘块大小、磁盘块数量、最多打开文件数等、
    char information[200];
    unsigned short root; //根目录文件的起始盘块号
    unsigned char *startblock; //虚拟磁盘上数据区开始位置
}block0;

unsigned char *myvhard; //指向虚拟磁盘的起始地址
useropen openfilelist[MAXOPENFILE];//   用户打开文件表数组
useropen *ptrcurdir;// 指向用户打开文件表中的当前目录所在打开文件表项的位置；
char currentdir[80];//     记录当前目录的目录名（包括目录的路径）
unsigned char* startp;//   记录虚拟磁盘上数据区开始位置
unsigned char buffer[SIZE];

void startsys(){
    FILE *fp;
    int i;
    myvhard=(unsigned char *)malloc(SIZE);
    memset(myvhard, 0, SIZE);
    fp=fopen(filename,"r");
    
    if(fp){
        fread(buffer,SIZE,1,fp);
        fclose(fp);
        if(buffer[0]!=0xaa||buffer[1]!=0xaa){
            printf("不存在文件系统，准备创建。。。\n");
            my_format();
        }
    }
}


int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
