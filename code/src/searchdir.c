#include "searchdir.h"
//#include "linklist.h"
#include "doublelinklist.h"

//创建节点  
struct Stack* create_node_stack(const char *fp)
{
	struct Stack *node = malloc(sizeof(struct Stack));
	node->next = NULL;
	node->path = malloc(strlen(fp)+1);
	strcpy(node->path, fp);
	return node;
}

//销毁节点  
bool destroy_node(struct Stack* node)
{
	if(node == NULL) return false;
	free(node->path);
	free(node);
	return true;
}  

//入栈 
bool push_node(struct Stack* top, const char *fp)
{
	if(top == NULL) return false;
	struct Stack* node = create_node_stack(fp);
	node->next = top->next;
	top->next  = node;
	return true;
}

//出栈
bool pop_node(struct Stack* top, struct Stack** node)
{
	if(top == NULL) return false;
	if(top->next == NULL) return false;
	*node = top->next;
	top->next = (*node)->next;
	(*node)->next = NULL;
	return true;
}

/*
      dirp----目录路径
	  type----文件类型 如".jpg"
	  head----双向链表头结点
	  count---统计对应类型type文件个数
*/
void search_dir(const char *dirp, const char *type, double_list head, int *count)
{
	 double_list pos_list=head;
	//创建栈
	struct Stack* top = create_node_stack(" ");
	//把目录压入栈中
	push_node(top, dirp);
	
	struct Stack* node=NULL;
	while(pop_node(top, &node))
	{
		DIR *dir = opendir(node->path);
		if(dir == NULL) 
		{
			//释放节点
			destroy_node(node);
			node = NULL;
			continue;
		}
		
		struct dirent *dird = NULL;
		while( (dird =readdir(dir)) )
		{
			if(dird->d_type == DT_DIR && strcmp(dird->d_name, ".") !=0 && strcmp(dird->d_name, "..") !=0 )
			{
				char buffer[strlen(node->path)+strlen(dird->d_name)+2];
				sprintf(buffer, "%s/%s", node->path, dird->d_name);
				//压栈
				push_node(top, buffer);
				
				
			}
			else
			{
				//判断是否是要找的文件
				int La = strlen(dird->d_name);
				int Lb = strlen(type);
				
				if(strcmp(dird->d_name+(La-Lb), type) == 0)
				{
					 //printf("%s/%s\n", node->path, dird->d_name);
									 
					 char buffer[strlen(node->path)+strlen(dird->d_name)+2];
					 sprintf(buffer, "%s/%s", node->path, dird->d_name);
					 
					 (*count)++;
					 if(*count==1)//第一次给head
					 {
						 strcpy(head->data, buffer);
						 printf("%s\n", head->data);
					 }
					 else
					 {
						pos_list=insert_node(pos_list,buffer);
						printf("%s\n", pos_list->data);
					 }	 
				}
			}
		}
		//关闭目录
		closedir(dir);
		
		//释放节点
		destroy_node(node);
		node = NULL;
	}
	
}

