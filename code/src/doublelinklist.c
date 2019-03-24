#include "doublelinklist.h"

//2、初始化空链表
double_list init_list(void)
{
	double_list head = malloc(sizeof(double_listnode)); //开辟节点空间
	if(head == NULL)
	{
		printf("malloc node failed!\n");
		return NULL;
	}

	head->prev = head->next = head;
	
	return head;
}

//3、插入节点
double_list insert_node(double_list p,char* data)
{
	if(p == NULL)
	{
		printf("p is NULL!\n");
		return NULL;
	}

    //double_list---结构体指针   double_listnode---结构体  sizeof(double_list)----用成了指针 会段错误
    double_list newnode = (double_list)malloc(sizeof(double_listnode));
	if(newnode == NULL)
	{
		printf("malloc newnode error\n");
		return NULL;
	}
	
	bzero( newnode, sizeof(double_listnode) );
	
	strcpy(newnode->data, data);
	newnode->prev = p;              //第一步
	newnode->next = p->next;        //第二步
	p->next = newnode;		        //第三步
	newnode->next->prev = newnode;  //第四步
	return newnode;

}

//3、显示链表操作
void show_double_list(double_list head)
{
	int i=0;
	if(head == NULL)
		return;

	double_list p = head;
	printf("\n 遍历链表\n");
	while(p!=head->prev)
	{
		printf("i=%d %s\n", i++, p->data);
		p = p->next;
	}
    printf("i=%d %s\n", i++, p->data);	
}

//销毁链表
void destroy_double_list(double_list head)
{
    if(!head){
        printf("head is NULL!\n");
        return;
    }

    double_list p = head->next,q = NULL;
    head->next = NULL;

    while(p!=head)
	{
        q = p;
        p = p->next;
        free(q);
    }
    free(head);
	head=NULL;
    q = NULL;
}


//链表--------------END--------------------