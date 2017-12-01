#include <stdio.h>
#include "bloomfilter.h"

#define maxq 500000   // 队列最大长度
#define maxu 200000   // 存储的不重复链接最大数量
#define BUF 1000000  // 源码最大长度

//***********网页爬取相关************
char sourceCode[BUF];
FILE *url, *tempurlfile;
char* URLs[maxu];   // 存储所有状态码为200且不重复的链接
int urlsNum;             // 数组URLs中元素的个数

typedef struct
{
	char *urlName;
	int fd;
} Ev_arg;

//************三叉树相关*************
// 树中每个节点的结构体
typedef struct TNode
{
	char data;                               //节点存储的字节数据
	struct TNode *left, *right;     //同级节点TSNode
	struct TNode *next;              //下一级节点
	int ID;                                    //编号id
} TNode, *TNodePtr;

// 三叉树结构体
typedef struct
{
	TNodePtr root;            //三叉树根节点
	int uNum;                   //插入链接的个数
	int uLen;                   //插入链接的长度
	char* searchURL;       //待搜索的链接
	TNodePtr currentNode;   //当前搜索到的节点
} TREE;

TREE *tr_alloc();
int tr_add_string(TREE *tree, char *P, int M, int id);
int tr_search(TREE* tree, int uLen, char* searchURL);

//**************队列相关*************
typedef struct
{
	char* myqueue[maxq];
	int head, tail;
} Queue;
//队列初始化
Queue* que_init();
//入队，成功返回1，否则返回0
int enqueue(Queue* q, char* node);
//出队操作
char* dequeue(Queue* q);
//判空操作
int queue_empty(Queue* q);
// 返回队列长度
int queue_length(const Queue* const q);


// 从该源码中提取的符合条件的链接全部入栈q
int DFA(char* souceCode, Queue *q, BF bf, int id);
//并行爬取网页
int handleURLs(Queue *queue_url, BF bf, char* serverip, int port);
int buildConnect(int *sockfd, char* serverip, int port);
int setnoblocking(int sockfd);
int sendRequest(char* url, int sockfd, char* serverip);
