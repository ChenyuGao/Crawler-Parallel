#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bloomfilter.h"

//创建一个大小为size的空间，返回携带空间指针的bf
BF bf_create(unsigned int size)   //size=VECTORSIZE=479240000
{
	unsigned int i;
	unsigned int vsize;
	BF table;  //有一个长度为17类型为pbit的数组hash[GROUPNUM=17]
	vsize = size >> 3;  //无符号数右移3位 补零
	table.hash[0] = (pbit)malloc((sizeof(bit)*(vsize)));

	table.size = size;

	for (i = 0; i < vsize; i++)
	{
		(table.hash[0] + i)->a = 0;
		(table.hash[0] + i)->b = 0;
		(table.hash[0] + i)->c = 0;
		(table.hash[0] + i)->d = 0;
		(table.hash[0] + i)->e = 0;
		(table.hash[0] + i)->f = 0;
		(table.hash[0] + i)->g = 0;
		(table.hash[0] + i)->h = 0;
	}

	for (i = 1; i<GROUPNUM; i++)
		table.hash[i] = table.hash[i - 1] + (vsize / GROUPNUM);

	return table;
}

//将字符串line加入哈希表对应关键码位置中
void bf_add(BF bf, char * line)
{
	unsigned int i;
	unsigned int hashResult;
	for (i = 0; i < GROUPNUM; i++)
	{
		hashResult = MurmurHash2(line, strlen(line), i + 1) % (bf.size / GROUPNUM);
		*(bf.hash[i] + (hashResult >> 3)) = setBitNumber(*(bf.hash[i] + (hashResult >> 3)), 1, hashResult % 8 + 1);  //将bit中pos位的值设为num  bit setBitNumber(bit b, int num, int pos)
		
		//printf("hashResult = %d\n", hashResult);
		//printf("(hashResult >> 3) = %d\n", (hashResult >> 3));
		//printf("地址 bf.hash[i] + (hashResult >> 3) = %d\n", (bf.hash[i] + (hashResult >> 3)));
		//printf("hashResult = %d\n", hashResult);
		
		
	}
}

//在bf中查找字符串line，存在返回1，否则返回0
int bf_search(BF bf, char line[])
{
	int i;
	unsigned int hashResult;
	int result = 1;

	for (i = 0; i < GROUPNUM; i++)
	{
		hashResult = MurmurHash2(line, strlen(line), i + 1) % (bf.size / GROUPNUM);

		if (getBitNumber(*(bf.hash[i] + (hashResult >> 3)), hashResult % 8 + 1) == 0)
		{
			//如果不匹配，退出
			result = 0;
			i = GROUPNUM;
		}
	}
	return result;
}

//超级哈希from google，参数seed确定具体哈希函数  hashResult = MurmurHash2(line, strlen(line), i + 1)
unsigned int MurmurHash2(const void * key, int len, unsigned int seed)
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const unsigned int m = 0x5bd1e995; // 0101 1011 1101 0001 1110 1001 1001 0101
	const int r = 24;

	// Initialize the hash to a 'random' value

	unsigned int h = seed ^ len;  //二进制按位异或

								  // Mix 4 bytes at a time into the hash

	const unsigned char * data = (const unsigned char *)key;

	while (len >= 4)
	{
		unsigned int k = *(unsigned int *)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	// Handle the last few bytes of the input array

	switch (len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
		h *= m;
	}

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

//获取bit结构中的pos位的值
int getBitNumber(bit b, int pos)
{
	int result;
	switch (pos)
	{
	case 1:result = b.a; break;
	case 2:result = b.b; break;
	case 3:result = b.c; break;
	case 4:result = b.d; break;
	case 5:result = b.e; break;
	case 6:result = b.f; break;
	case 7:result = b.g; break;
	case 8:result = b.h; break;
	default:result = 0;
	}
	return result;
}
//将bit中pos位的值设为num  bit setBitNumber(bit b, int num, int pos)
bit setBitNumber(bit b, int num, int pos)
{
	switch (pos)
	{
	case 1: b.a = num; break;
	case 2: b.b = num; break;
	case 3: b.c = num; break;
	case 4: b.d = num; break;
	case 5: b.e = num; break;
	case 6: b.f = num; break;
	case 7: b.g = num; break;
	case 8: b.h = num; break;
	default:break;
	}
	return b;
}
