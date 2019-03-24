#ifndef SEARCH_DIR_H
#define SEARCH_DIR_H

#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "doublelinklist.h"
//#include "linklist.h"

//设计一个栈节点
struct Stack
{
	char *path;
	struct Stack* next;
};

//create_node_stack
//创建节点  
struct Stack* create_node_stack(const char *fp);
//struct Stack* create_node(const char *fp);

//销毁节点  
bool destroy_node(struct Stack* node);
//入栈 
bool push_node(struct Stack* top, const char *fp);
//出栈
bool pop_node(struct Stack* top, struct Stack** node);

void search_dir(const char *dirp, const char *type, double_list head, int *count);



#endif 
