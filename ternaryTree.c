#include <stdlib.h>
#include <string.h>
#include "common.h"

TREE *tr_alloc()
{
	TREE *tree;
	if ((tree = malloc(sizeof(TREE))) == NULL)
		return NULL;
	memset(tree, 0, sizeof(TREE));
	if ((tree->root = malloc(sizeof(TNode))) == NULL)
    {
		free(tree);
		return NULL;
	}
	memset(tree->root, 0, sizeof(TNode));
	return tree;
}

// 将url添加进三叉树
int tr_add_string(TREE *tree, char *P, int M, int id)
{
    if(M == 0)  // 防止空串插入影响id
    {
        return -1;
    }

	int i = 0, j, flag;
	TNodePtr currentNode, preNode, newNode;
	if (tree->uNum == 0)  //还未添加url
	{
		currentNode = tree->root;
		for (i; i<M; i++)
		{
			if ((newNode = malloc(sizeof(TNode))) == NULL)
				return -1;
			memset(newNode, 0, sizeof(TNode));
			newNode->data = P[i];
			if (i == M - 1)
				newNode->ID = id;
			else
				newNode->ID = -1;
			currentNode->next = newNode;
			currentNode = currentNode->next;
		}
		tree->uNum++;
		return 0;
	}
	currentNode = preNode = tree->root->next;
	while (currentNode != NULL && i<M)
	{
		if (P[i] == currentNode->data)
		{
			i++;
			preNode = currentNode;
			currentNode = currentNode->next;
			flag = 0;
		}
		else if (P[i]<currentNode->data)
		{
			preNode = currentNode;
			currentNode = currentNode->left;
			flag = 1;
		}
		else
		{
			preNode = currentNode;
			currentNode = currentNode->right;
			flag = 2;
		}
	}
	//同样的链接以前一个的编号为准
	if (i == M)
    {
        if(preNode->ID == -1)
        {
            tree->uNum++;
            preNode->ID = id;
            return 0;
        }
        else
            return -1;
    }
	if ((newNode = malloc(sizeof(TNode))) == NULL)
		return -1;
	memset(newNode, 0, sizeof(TNode));
	newNode->data = P[i];
	if (i == M - 1)
		newNode->ID = id;
	else
		newNode->ID = -1;
	currentNode = preNode;
	if (flag == 0)
	{
		currentNode->next = newNode;
		currentNode = currentNode->next;
		i++;
	}
	else if (flag == 1)
	{
		currentNode->left = newNode;
		currentNode = currentNode->left;
		i++;
	}
	else
	{
		currentNode->right = newNode;
		currentNode = currentNode->right;
		i++;
	}
	//插入完毕则返回
	if (i == M)
	{
		tree->uNum++;
		return 0;
	}
	//将剩余字符插入
	for (i; i<M; i++)
	{
		if ((newNode = malloc(sizeof(TNode))) == NULL)
			return -1;
		memset(newNode, 0, sizeof(TNode));
		newNode->data = P[i];
		if (i == M - 1)
			newNode->ID = id;
		else
			newNode->ID = -1;
		currentNode->next = newNode;
		currentNode = currentNode->next;
	}
	tree->uNum++;
	return 0;
}

// 在三叉树中搜索url,若存在返回id,若不存在返回-1
int tr_search(TREE* tree, int uLen, char* searchURL)
{
	int i;
	TNodePtr currentNode, child;
	currentNode = tree->root;
	child = currentNode->next;
	for (i = 0; i<uLen; i++)
	{
	    if(child == NULL)
            return -1;
		if (searchURL[i] == child->data)
        {
            currentNode = child;
            child = currentNode->next;
            if (i == uLen-1)
                return currentNode->ID;
            continue;
        }
        else if (searchURL[i] > child->data)
        {

            child = child->right;
            if(child == NULL)
                return -1;
            i--;
            continue;
        }
        else if (searchURL[i] < child->data)
        {
            child = child->left;
            if(child == NULL)
                return -1;
            i--;
            continue;
        }
	}
}
