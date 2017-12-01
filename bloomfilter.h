
#ifndef _BLOOM_H
#define _BLOOM_H

#define GROUPNUM 23              // 哈希函数的个数 (错误率取 0.00001%）
#define VECTORSIZE 6709540     // 每个空间的大小


//填满字节的位域结构
struct bloomsingle
{
	unsigned char  a : 1;
	unsigned char  b : 1;
	unsigned char  c : 1;
	unsigned char  d : 1;
	unsigned char  e : 1;
	unsigned char  f : 1;
	unsigned char  g : 1;
	unsigned char  h : 1;
};
typedef struct bloomsingle bit;
typedef struct bloomsingle * pbit;

int getBitNumber(bit b, int pos);//获取bit结构中的pos位的值
bit setBitNumber(bit b, int num, int pos);//将bit中pos位的值设为num


struct HashTable
{
	pbit hash[GROUPNUM];  //定义一个长度为GROUPNUM=17的w位数组，数组的每个元素是一个pbit指针类型，也就是一个bit的地址
	unsigned int size;
};

typedef struct HashTable BF;

void bf_add(BF bf, char * line);//将字符串line加入哈希表对应关键码位置中
BF bf_create(unsigned int size);//创建一个大小为size的空间，返回携带空间指针的bf
int bf_search(BF bf, char line[]);//在bf中查找字符串line，存在返回1，否则返回0

								  //bloom－filter主功能
//int bf_check(FILE * fp_strpool, FILE * fp_checkedstr, FILE * result);
//超级哈希，参数seed确定具体哈希函数
unsigned int MurmurHash2(const void *key, int len, unsigned int seed);


#endif
