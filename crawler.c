#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
#include "common.h"
#include "bloomfilter.h"

int main(int argc, char* argv[])
{
    char *serverip = argv[1];
    int port = atoi(argv[2]);
    char *fileName = argv[3];
	Queue *queue_url = que_init();
	// 将首页地址入队
	char* indexUrl = "/";
	enqueue(queue_url, indexUrl);
	// 构建布隆过滤器
	BF bf = bf_create(VECTORSIZE);
	bf_add(bf, indexUrl);
    if((url = fopen(fileName, "w")) == NULL)
    {
    	printf("file url.txt write failed!\n");
    	return -1;
	}
	if((tempurlfile = fopen("tempurlfile.txt", "w")) == NULL)
    {
    	printf("file tempurlfile.txt write failed!\n");
    	return -1;
	}
	// 提取临时的链接关系到数组
	printf("开始爬虫:\n");
    handleURLs(queue_url, bf, serverip, port);
    printf("爬虫完毕!\n");
    fclose(tempurlfile);

    // 提取正确的链接关系到url.txt
    // 对数组URLs,索引从1到urlsNum加入三叉树
	TREE *tree = tr_alloc();
	printf("\n开始建树:\n");
    for(int u = 1; u<urlsNum; u++)
    {
        int len1 = strlen(URLs[u]);
        tr_add_string(tree, URLs[u], len1, u);
    }
    printf("建树完毕!\n");

    printf("\n开始写入:\n");
    if((tempurlfile = fopen("tempurlfile.txt", "r")) == NULL)
    {
    	printf("file tempurlfile.txt read failed!\n");
    	return -1;
	}
	char row[100];
	char tempSourceID[10], tempdestURL[100];
    int sourceID, destID, urlLen;
	int num = 0;
	int p, q, j;
	int buf[500] = {0};
	int camp = 1;
	int bufnum = 0, flagrep;
	while(!feof(tempurlfile))
    {
        fgets(row, 100, tempurlfile);
        //提取该行的源id和目的url
        memset(tempSourceID, 0, 10);
        memset(tempdestURL, 0, 100);
        for (p = 0; row[p] != ' ' ; ++p);
        strncpy(tempSourceID, row, p);
        sourceID = atoi(tempSourceID);
        for (q = p; row[q] != '\n' ; ++q);
        if((q-p) == 1)
        {
            continue;
        }
        strncpy(tempdestURL, row+p+1, q-p-1);
        tempdestURL[q] = '\0';
        urlLen = strlen(tempdestURL);
        destID = tr_search(tree, urlLen, tempdestURL);
        if(sourceID == destID)
            continue;
        if(destID > 0)
        {
            // 去掉重复的sourceID->destID对
            flagrep = 0;    
            if(camp == sourceID)
            {
                for(j = 0; j < bufnum; j++)
                {
                    if(buf[j] != destID)
                        flagrep = 0;
                    else
                    {
                        flagrep = 1;
                        break;
                    }
                }
                if(flagrep == 0)
                {
		    buf[bufnum] = destID;
                    fprintf(url, "\n%d %d",  sourceID, destID);
                    num++;
                    bufnum++;
                }
                else
                    continue;
            }
            else
            {
                buf[0] = destID;
                bufnum = 1;
                camp = sourceID;
		fprintf(url, "\n%d %d",  sourceID, destID);
            }
        }
    }
    printf("写入完毕!\n");
    printf("正确的链接数量共有: %d 个\n", num);
    fclose(tempurlfile);
    fclose(url);
    remove("tempurlfile.txt");
    printf("\n程序结束!\n");
	return 0;
}


/*  调试用
    FILE *matrix;
    if((matrix = fopen("matrix.txt", "w")) == NULL)
    {
        printf("file matrix.txt write failed!\n");
        return -1;
    }

    if((url = fopen("url.txt", "r")) == NULL)
    {
        printf("file url.txt read failed!\n");
        return -1;
    }
    char r[100];
    char _url[100];
    int x, y;
    int _n = 1;
    while(!feof(url))
    {
        fgets(r, 100, url);
        if(*r == '\n')
            break;
        memset(_url, 0, 100);
        for (x = 20; r[x] != ' ' ; ++x);
        strncpy(_url, r + 20, x - 20);
        //printf("%s\n", _url);  // 此处没问题
        URLs[_n] = malloc(sizeof(_url));
        memset(URLs[_n], 0, sizeof(_url));
        for(int t = 0; t < strlen(_url); t++)
        {
            URLs[_n][t] = _url[t];
        }
        _n++;
    }
*/
