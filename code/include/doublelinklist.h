#ifndef DOUBLE_LINK_LIST_H
#define DOUBLE_LINK_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

//设计节点 双向链表
typedef struct node
{
	char   data[256];
	struct node *prev;
	struct node *next;
}double_listnode,*double_list;  //double_listnode = struct node  ,double_list = struct node *

//2、初始化空链表
double_list init_list(void);


//3、插入节点
double_list insert_node(double_list p,char* data);


//3、显示链表操作
void show_double_list(double_list head);

//销毁链表
void destroy_double_list(double_list head);
#endif