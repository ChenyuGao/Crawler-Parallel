#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "common.h"
#include "bloomfilter.h"

// 从该源码中提取的符合条件的链接全部入栈q
int DFA(char* sourceCode, Queue *q, BF bf, int id)
{
	char *newUrl;
	int bool;      // 判重
	int flag = 0;  // 判断链接最后是否为换行符
	int length;    // 链接长度
	char urlname[1024];  // 匹配成功的链接
	char *start = 0;
	char *end = 0;
	int state = 0;
	while (*sourceCode != '\0')
	{
		switch (state)
		{
		case 0:
			switch (*sourceCode)
			{
			case '<':
				state = 1;
				sourceCode++;
				break;
			default:
				state = 0;
				sourceCode++;
				break;
			}
			break;

		case 1:
			switch (*sourceCode)
			{
			case 'a':
				state = 2;
				sourceCode++;
				break;
			default:
				state = 0;
				sourceCode++;
				break;
			}
			break;

		case 2:
			switch (*sourceCode)
			{
			case 'h':
				state = 3;
				sourceCode++;
				break;
			case '>':
				state = 0;
				sourceCode++;
				break;
			default:
				state = 2;
				sourceCode++;
				break;
			}
			break;

		case 3:
			switch (*sourceCode)
			{
			case 'r':
				state = 4;
				sourceCode++;
				break;
			case '>':
				state = 0;
				sourceCode++;
				break;
			case 'h':               // 处理 hhref 的情况
				state = 3;
				sourceCode++;
				break;
			default:
				state = 2;
				sourceCode++;
				break;
			}
			break;

		case 4:
			switch (*sourceCode)
			{
			case 'e':
				state = 5;
				sourceCode++;
				break;
			case '>':
				state = 0;
				sourceCode++;
				break;
			default:
				state = 2;
				sourceCode++;
				break;
			}
			break;

		case 5:
			switch (*sourceCode)
			{
			case 'f':
				state = 6;
				sourceCode++;
				break;
			case '>':
				state = 0;
				sourceCode++;
				break;
			default:
				state = 2;
				sourceCode++;
				break;
			}
			break;

		case 6:
			switch (*sourceCode)
			{
			case '=':
				state = 7;
				sourceCode++;
				break;
			case '>':
				state = 0;
				sourceCode++;
				break;
			default:
				state = 2;
				sourceCode++;
				break;
			}
			break;

		case 7:
			switch (*sourceCode)
			{
			case '"':
				state = 8;
				sourceCode++;
				break;
			case '\'':   // 有的网址是单引号
				state = 8;
				sourceCode++;
				break;
			case '>':
				state = 0;
				sourceCode++;
				break;
			default:
				state = 2;
				sourceCode++;
				break;
			}
			break;

		case 8:
			switch (*sourceCode)
			{
			case '/':

				sourceCode += 2;

				char temp[20];
				memcpy(temp, sourceCode, 13);       // 匹配 news.sohu.com
				*(temp + 13) = '\0';
				if (0 == strcmp(temp, "news.sohu.com"))
				{
					state = 9;
					sourceCode = sourceCode + 13;   // 跳过 news.sohu.com
					start = sourceCode;

				}
				else
					state = 0;

				break;
			case '"':
				state = 0;
				sourceCode ++;
				break;
			case '>':
				state = 0;
				sourceCode++;
				break;
			case '#':
				state = 0;
				sourceCode++;
				break;
			default:
				state = 8;
				sourceCode++;
				break;
			}
			break;

		case 9:
			switch (*sourceCode)
			{
			case '\n':     // 如果"前有换行符则需要去掉
				state = 9;
				flag = 1;
				sourceCode++;
				break;
			case '"':
				state = 10;
				end = sourceCode;
				if (flag)
				{
					length = (int)(end - start - 1);
					flag = 0;
				}
				else
				{
					length = (int)(end - start);
				}
				memcpy(urlname, start, length);   // urlname是匹配成功的链接名称
				urlname[length] = 0;
				fprintf(tempurlfile, "%d %s\n",  id, urlname);   // 写入 tempurlfile.txt 文件
				bool = bf_search(bf, urlname);
				if (!bool)  // 不重，则申请空间
				{
					if ((newUrl = malloc(sizeof(urlname))) == NULL)
						return -1;
					memset(newUrl, 0, sizeof(urlname));
					strcpy(newUrl, urlname);
					enqueue(q, newUrl);
					bf_add(bf, newUrl);
				}
				else
				{
					state = 3;
				}
				sourceCode++;
				break;
			case '\'':
				state = 10;
				end = sourceCode;
				if (flag)
				{
					length = (int)(end - start - 1);
					flag = 0;
				}
				else
				{
					length = (int)(end - start);
				}
				memcpy(urlname, start, length);   // urlname是匹配成功的链接名称
				urlname[length] = 0;
				fprintf(tempurlfile, "%d %s\n",  id, urlname);   // 写入 tempurlfile.txt 文件
				bool = bf_search(bf, urlname);
				if (!bool)  // 不重，则申请空间
				{
					if ((newUrl = malloc(sizeof(urlname))) == NULL)
						return -1;
					memset(newUrl, 0, sizeof(urlname));
					strcpy(newUrl, urlname);
					enqueue(q, newUrl);
					bf_add(bf, newUrl);
				}
				else
				{
					state = 3;
				}
				sourceCode++;
				break;
			case '>':
				state = 0;
				sourceCode++;
				break;
			default:
				state = 9;
				sourceCode++;
				break;
			}
			break;

		case 10:
			switch (*sourceCode)
			{
			case 'h':              // 处理一个a标签中有多个链接的情况
				state = 3;
				sourceCode++;
				break;
			case '>':
				state = 0;
				sourceCode++;
				break;
			default:
				state = 10;
				sourceCode++;
				break;
			}
			break;

		default:
			break;
		}
	}
	return 0;
}
