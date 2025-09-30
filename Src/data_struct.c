/*
 * @file dataStruct.c
 * @brief 数据结构实现文件 
 * @author ringtree
 * @date 2025-8-18
 * @version 1.0
 *
 * 本文件包含了数据结构的具体实现

 * 单向链表部分：
 * - 实现了单向链表的创建、初始化、插入、删除、查找、遍历等基本操作。
 * - 单向链表节点存储32位整型数据，支持多种插入和删除方式。
 *
 * 所有函数实现均遵循dataStruct.h头文件中声明的接口规范。
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

#include "data_struct.h"

/**
 * @addtogroup 单向链表模块
 * @{ 
 */

/**
 * @defgroup 单向链表初始化操作
 * @brief 单向链表初始化相关函数
 * @{
 */

/**
 * @brief 创建并初始化一个单向链表结构
 *
 * 该函数创建一个新的单向链表，初始化为空单向链表。
 * 单向链表结构中的headIndex和endIndex初始化为NULL，length初始化为0。
 *
 * @return SL_link* 返回指向新创建的单向链表结构的指针，若内存分配失败则返回NULL
 * @note 分配的内存空间需要自己释放free()。
 */
inline SL_link* SL_inifLink (
    void
) {
	SL_link* cur = (SL_link*)malloc(sizeof(SL_link));  // 动态内存分配 单向链表结构体
	if (!cur) {
		printf("内存分配失败 可能内存不足");
		return NULL;
	}

	cur->headIndex = NULL;  // 设置单向链表头节点
	cur->endIndex = NULL;  // 设置单向链表尾节点
	cur->length = 0;  // 初始化单向链表数量 0

	return cur;  
}


/**
 * @brief 创建并初始化一个新的单向链表节点
 *
 * 该函数创建一个新的单向链表节点，将节点数据设置为inputData，
 * next指针初始化为NULL。
 *
 * @param inputData 要存储在节点中的数据
 * @return SL_node* 返回指向新创建的节点的指针，若内存分配失败则返回NULL
 */
static inline SL_node* SL_inifNode (
	const Elemtype inputData
) {
	SL_node* head = (SL_node*)malloc(sizeof(SL_node));  // 动态内存分配 节点结构体
	if (!head) {
		printf("内存分配失败 可能内存不足");
		return NULL;
	}
  
	head->data = inputData;  
	head->next = NULL;

	return head;  
}

/** @} */ // 单向链表初始化操作

/**
 * @defgroup 单向链表插入操作
 * @brief 单向链表插入相关函数
 * @{
 */

/**
 * @brief 在单向链表头部插入一个新节点
 *
 * 该函数创建一个新的节点，将其插入到单向链表的头部，
 * 并更新单向链表的相关属性（头指针和长度）。
 *
 * @param linkedList 指向要操作的单向链表的指针
 * @param inputData 要插入的新节点数据
 * @return void 无返回值
 */
void SL_insertHead (
	SL_link* const linkedList,
	const Elemtype inputData
) {
	SL_node* newNode = SL_inifNode(inputData);  // 调用inifNode()函数 创建新节点

	// 节点连接更新
	newNode->next = linkedList->headIndex;
	linkedList->headIndex = newNode;

	// 单向链表长度更新
	linkedList->length++;
}


/**
 * @brief 在单向链表尾部插入一个新节点
 *
 * 该函数创建一个新的节点，将其插入到单向链表的尾部，
 * 并更新单向链表的相关属性（尾指针和长度）。
 * 如果单向链表为空，则新节点同时成为头节点和尾节点。
 *
 * @param linkedList 指向要操作的单向链表的指针
 * @param inputData 要插入的新节点数据
 * @return void 无返回值
 * @note 该函数实现中存在一个潜在问题：当单向链表为空时（headIndex为NULL），
 *       直接访问temNode->next会导致空指针解引用错误。
 */
void SL_add (
	SL_link* const linkedList,
	const Elemtype inputData
) {
    if (linkedList->headIndex == NULL) {
        SL_insertHead(linkedList, inputData);
    }
    else {
	    SL_node* newNode = SL_inifNode(inputData);  // 调用inifNode()函数 创建新节点
	    SL_node* temNode = linkedList->headIndex;  // 创建临时节点代替单向链表头节点

	    // 节点连接更新
	    linkedList->endIndex = newNode;
	    while (temNode->next) {  // 遍历寻找尾节点上一节点（单单向链表缺点）
	    	temNode = temNode->next;
	    }
	    temNode->next = newNode;

	    // 单向链表长度更新
	    linkedList->length++;
    }
}

/**
 * @brief 尾插法连续插入多个数据
 *
 * 该函数创建多个新的节点，从可变参数列表中提取数据并添加到节点中，
 * 将其插入到单向链表的尾部，并更新单向链表的相关属性（尾指针和长度）。
 * 内部通过调用add函数实现单个节点的插入。
 *
 * @param linkedList 单向链表结构体指针
 * @param count 插入的节点数量
 * @param ... 可变参数列表，包含要插入的节点的数据（int32类型）
 * @return void 无返回值
 * @note 该函数在每次调用add函数后会手动增加单向链表长度，
 *       但add函数内部已经更新了长度，这可能导致长度被重复增加。
 */
void SL_extind(
	SL_link* const linkedList,
	const uint32 count,
	...
) {
	va_list args;  // 创建可变参数列表容器
	va_start(args, count);  // 初始化可变参数列表
  
  // 循环插入数据
	for (uint16 i = 0; i < count; i++) {
		int32 data = va_arg(args, int32);  // 获取可变参数列表中的下一个参数
		SL_add(linkedList, data);  // 调用add()函数 添加数据
	}

	va_end(args);  // 结束可变参数列表
}


/**
 * @brief 在单向链表的指定位置插入新节点
 *
 * 该函数创建一个新的节点，将其插入到单向链表的指定位置，
 * 并更新单向链表的相关属性（尾指针和长度）。
 * 如果索引为0，则调用insertHead函数在头部插入；
 * 如果索引大于等于单向链表长度，则打印错误信息并返回。
 *
 * @param linkedList 指向单向链表结构的指针
 * @param inputData 要插入的数据
 * @param index 插入位置的索引（从0开始计数）
 * @return void 无返回值
 * @note 当索引值大于等于单向链表长度时，函数仅打印错误信息而不执行插入操作
 */
void SL_insert (
	SL_link* const linkedList,
	const Elemtype inputData,
	const uint32 index
) {
	if (!index) {  // 如果索引值为0 则插入头部
		SL_insertHead(linkedList, inputData);
		return;
	}
	else if (index >= linkedList->length) {  // 如果索引值大于单向链表长度 则返回错误
		printf("Error: Index out of range\n");
		return;
	}

	SL_node* newNode = SL_inifNode(inputData);  // 调用inifNode()函数 创建新节点
	SL_node* cur = linkedList->headIndex;  // 定义指针cur指向单向链表头节点

	// 循环找到索引位置的前一个节点
	for (uint32 i = 0; i < index - 1; i++) {
		cur = cur->next;
	}

	// 节点连接更新
	newNode->next = cur->next;
	cur->next = newNode;
  
	// 更新单向链表长度
	linkedList->length++;
}


/** @} */ // 单向链表插入操作

/**
 * @defgroup 单向链表查找操作
 * @brief 单向链表查找相关函数
 * @{
 */

/**
 * @brief 计算指定值在单向链表中出现的次数
 *
 * 该函数从单向链表头开始，依次访问每个节点，
 * 统计与指定数据相等的节点数量。
 *
 * @param linkedList 单向链表结构体指针
 * @param findData 要查找的数据
 * @return uint32 返回数据在单向链表中出现的次数
 */
uint32 SL_count (
	SL_link* const linkedList,
	const Elemtype findData
) {
    if (linkedList == NULL) {
        return 0;
    }
	SL_node* cur = linkedList->headIndex;  // 定义指针cur指向单向链表头节点 
	uint32 count = 0;  // 定义变量count记录找到的节点数量

	// 循环遍历单向链表，直到单向链表末尾
	while (cur) {
		if (cur->data == findData) 
			count++;
		cur = cur->next;
	}

	return count;  
}


/**
 * @brief 在单向链表中查找所有匹配数据的节点指针，并返回数组
 *
 * 该函数从单向链表头开始遍历，收集所有数据与指定值相等的节点指针，
 * 最终返回动态分配的指针数组（需由调用者释放）。
 * 函数首先调用count函数统计匹配节点数量，然后分配对应大小的内存，
 * 最后再次遍历单向链表收集所有匹配节点的指针。
 *
 * @param linkedList 单向链表结构体指针
 * @param findData 要查找的目标数据
 * @param outCount 输出参数，用于返回匹配的节点数量（若未找到则为0）
 * @return SL_node** 指向匹配节点指针数组的指针（未找到时返回NULL）
 * @note 调用者需使用 free() 释放返回的数组以避免内存泄漏
 * @warning 代码中存在同名但不同签名的find函数，这在C语言中是不允许的，
 *          可能会导致编译错误或运行时问题
 */
SL_node** SL_find (
    SL_link* const linkedList,
    const Elemtype findData,
    uint32* outCount  
) {
  // 统计匹配的节点数量
  *outCount = SL_count(linkedList, findData);
  if (*outCount == 0) {
    return NULL;  // 无匹配节点，返回 NULL
  }
  
  SL_node** nodeList = (SL_node**)malloc(sizeof(SL_node*) * (*outCount));  // 动态分配内存存储所有匹配节点的指针
  SL_node* cur = linkedList->headIndex;  // 遍历单向链表，收集所有匹配的节点指针
	
	// 遍历单向链表，收集所有匹配的节点指针
	for (uint32 i = 0; i < *outCount; i++) {
		for (; cur != NULL; cur = cur->next) {
			if (cur->data == findData) {
				nodeList[i] = cur;
				cur = cur->next;  // 移动到下一个节点
				break;
			}
		}
	}
	
  return nodeList;  
}


/**
 * @brief 在单向链表中查找指定数据并返回第一个匹配节点的索引
 *
 * 该函数从单向链表头开始遍历，查找与指定数据相等的第一个节点，
 * 返回其索引值（从0开始计数）。
 * 如果遍历完整个单向链表都没有找到匹配的节点，则返回UINT32_MAX。
 *
 * @param linkedList 单向链表结构体指针
 * @param findData 要查找的目标数据
 * @return uint32 返回找到的节点的索引（从0开始计数），未找到则返回UINT32_MAX
 * @note 函数返回UINT32_MAX表示未找到匹配节点，需包含<stdint.h>头文件
 */
uint32 SL_getIndex (
	SL_link* const linkedList,
	const Elemtype findData
) {
    if (linkedList == NULL) {
        printf("Error: linkedList is NULL\n");
        return UINT32_MAX;
    }
	SL_node* cur = linkedList->headIndex;
	uint32 index = 0;
	while (cur) {
		if (cur->data == findData) 
		    return index;
		cur = cur->next;
		index++;
	}
	return UINT32_MAX;
}   

/**
 * @brief 获取链表中绝对值的集合
 *
 * 对链表中节点数据的绝对值进行去重处理，并将去重后的结果按降序排列输出。
 * 同时创建一个新链表保存去重后的绝对值。
 * 
 * @param linked 链表
 */
void* SL_get_set(
  SL_link* const linked
) { 
    // 检查链表是否为空
    if (linked == NULL || linked->headIndex == NULL) {
        printf("警告：传入的链表为空\n");
        return NULL;
    }
    
    SL_node* temNode = linked->headIndex;
    Elemtype max = abs(temNode->data);
    Elemtype min = abs(temNode->data);
    
    // 找出链表中绝对值的最大值和最小值
    for (; temNode; temNode = temNode->next) {
      Elemtype absVal = abs(temNode->data);
      max = (max < absVal) ? absVal : max;
      min = (min > absVal) ? absVal : min;
    }
    
    // 分配适当大小的数组
    Elemtype* list = (Elemtype*)malloc(sizeof(Elemtype) * (max - min + 1));
    if (list == NULL) {
        printf("内存分配失败\n");
        return NULL;
    }
    
    // 初始化数组，注意循环范围
    for (int i = 0; i < (max - min + 1); i++) {
        list[i] = 0;
    }

    // 去重并创建新链表
    temNode = linked->headIndex;
    SL_link* outLint = SL_inifLink();
    if (outLint == NULL) {
        free(list);
        printf("创建链表失败\n");
        return NULL;
    }
    
    for (; temNode; temNode = temNode->next) {
      Elemtype absVal = abs(temNode->data);
      // 计算索引时确保在有效范围内
      size_t index = absVal - min;
      if (index < (max - min + 1) && !list[index]) {
        list[index] = 1;
        SL_add(outLint, absVal);
      }
    }

    // 排序并遍历输出结果
    SL_traverseLink(outLint);

    // 释放资源
    free(list);
    SL_freeLinks(&outLint);
    
    return NULL;
}

/**
 * @brief 求解约瑟夫环问题：在一个循环链表中，从头部开始，每数到第 n 个节点就删除该节点，
 *        直到链表中只剩下一个节点，返回该剩余节点的数据。
 *
 * @param link 指向循环链表结构的常量指针，包含头节点指针、尾节点指针和链表长度等信息。
 *             假设该结构中至少包含以下成员：
 *             - SL_node* headIndex;  // 指向链表第一个节点
 *             - SL_node* endIndex;   // 指向链表最后一个节点
 *             - uint32 length;       // 链表当前节点总数
 *             并且存在一个外部函数 SL_deleteNode(link, node) 用于删除指定节点。
 * @param n    每数到第 n 个节点时，删除该节点。n 必须 >= 1。
 * 
 * @return uint32 最后剩下的那个节点的数据值（data）。
 * 
 * @note 该函数会不断删除链表中的节点，直到链表中只剩一个节点，时间复杂度为 O(m*n)，其中 m 为初始节点个数。
 *       假设链表是循环的，且 SL_deleteNode 函数能正确处理节点删除和链表结构更新。
 */
uint32 SL_josephusSurvivor (
  SL_link* const link,
  uint32 n
) {
  uint32 counter = 1;       // 计数器
  SL_node* temNode = link->headIndex;   // 临时节点
  SL_node* delNode = NULL;    // 待删除节点

  for (; link->length != 1; counter++) {  // 循环直到链表中剩一个节点
    if (counter == n) {
      delNode = temNode;
      temNode = (temNode == link->endIndex) ? link->headIndex : temNode->next;  // 单向链表模拟循环链表
      SL_deleteNode(link, delNode);
      counter = 0;  // 重置计数器
    } 
    else 
      temNode = (temNode == link->endIndex) ? link->headIndex : temNode->next;
  }

  return temNode->data;
}

/** @} */ // 单向链表查找操作

/**
 * @defgroup 单向链表修改操作
 * @brief 单向链表修改相关函数
 * @{
 */

/**
 * @brief 排序单向链表
 * 
 * 该函数对单向链表进行冒泡排序，排序方式由参数way指定。
 * 参数way参考data_struct.h文件的 @brief 单向链表排序方式宏定义
 * 
 * @param linkedList 单向链表指针（需保证单向链表结构有效且不为空）
 * @param way 排序方式，ASC为升序，DESC为降序
 */
void SL_sort (
	SL_link* const linkedList,
	enum sort way
) {
    // 检查链表是否为空或只有一个节点
    if (linkedList == NULL || linkedList->headIndex == NULL || linkedList->length <= 1) {
        return;
    }
    
    SL_node* outer = linkedList->headIndex;
    
    // 外层循环
    for (size_t i = 0; i < linkedList->length - 1; i++) {
        SL_node* inner = linkedList->headIndex;
        
        // 内层循环，每次将当前最大/小值冒泡到末尾
        for (size_t j = 0; j < linkedList->length - i - 1; j++) {
            // 根据排序方式比较并交换节点数据
            if ((!way && inner->data > inner->next->data) || (way && inner->data < inner->next->data)) {
                Elemtype temp = inner->data;
                inner->data = inner->next->data;
                inner->next->data = temp;
            }
            inner = inner->next;
        }
        outer = outer->next;
    }
}

/**
 * @brief 反转单向链表
 * 
 * 该函数将单向链表反转，即将链表头节点变为链表尾节点，链表尾节点变为链表头节点。
 * 
 * @param linked 单向链表指针（需保证单向链表结构有效且不为空）
 * @return uint16 函数执行成功返回1，否则返回0

 */
uint16 SL_reverse (
  SL_link* const linked
) {
  if (linked == NULL || linked->length == 0 || linked->length == 1) {
    return 0;
  } else if (linked->length == 2) {
    SL_node* temp = linked->headIndex;
    linked->headIndex = linked->endIndex;
    linked->endIndex = temp;
    linked->headIndex->next = linked->endIndex;
    linked->endIndex->next = NULL;
    return 1;
  }

  SL_node* pre = NULL;                ///< 前驱节点
  SL_node* cur = linked->headIndex;   ///< 中心节点
  SL_node* next = cur->next;          ///< 后继节点
  
  // 更新链表头尾指针
  linked->headIndex = linked->endIndex;
  linked->endIndex = cur;

  // 反转核心
  while (cur) {
    cur->next = pre;
    pre = cur;
    cur = next;
    next = cur ? cur->next : NULL;
  }
}

/** @} */ // 单向链表修改操作

/**
 * @defgroup 单向链表删除操作
 * @brief 单向链表删除相关函数
 * @{
 */

/**
 * @brief 删除单向链表的头节点
 *
 * 该函数删除单向链表的头节点，释放其内存，并更新单向链表的头指针和长度。
 * 如果单向链表为空，则打印错误信息并返回UINT32_MAX。
 * 如果删除后单向链表为空，同步更新尾指针为NULL。
 *
 * @param linkedList 单向链表指针（需保证单向链表结构有效且不为空）
 * @return Elemtype 被删除头节点的数据；若失败（如单向链表为空），返回 UINT32_MAX
 */
Elemtype SL_delHead(
	SL_link* const linkedList
) {
    // 检查单向链表是否为空
    if (linkedList->headIndex == NULL || linkedList->length == 0) {
        printf("错误：单向链表为空，无法删除头节点\n");
        return UINT32_MAX;
    }

    SL_node* deletedNode = linkedList->headIndex;          // 保存头节点
    Elemtype outData = deletedNode->data;

    linkedList->headIndex = deletedNode->next;          // 更新头指针

    // 若删除后单向链表为空，同步更新尾指针
    if (linkedList->headIndex == NULL) {
        linkedList->endIndex = NULL;
    }

    linkedList->length--;                               // 更新单向链表长度
    free(deletedNode);                                  // 释放被删除节点的内存
    return outData;                                     // 返回被删除节点的数据
}


/**
 * @brief 删除单向链表的尾节点
 *
 * 该函数删除单向链表的尾节点，释放其内存，并更新单向链表的尾指针和长度。
 * 如果单向链表为空，则打印错误信息并返回UINT32_MAX。
 * 如果单向链表只有一个节点（头节点也是尾节点），删除后将头指针和尾指针都设置为NULL。
 * 否则，需要遍历单向链表找到倒数第二个节点，将其next指针设置为NULL，并更新尾指针。
 *
 * @param linkedList 单向链表指针（需保证单向链表结构有效且不为空）
 * @return Elemtype 被删除尾节点的数据；若失败（如单向链表为空），返回 UINT32_MAX
 * @note 由于单单向链表的限制，删除尾节点需要遍历整个单向链表，时间复杂度为O(n)
 */
Elemtype SL_delEnd(
	SL_link* const linkedList
) {
    // 检查单向链表是否为空
    if (linkedList->endIndex == NULL || linkedList->length == 0) {
        printf("错误：单向链表为空，无法删除尾节点\n");
        return UINT32_MAX;
    }

    SL_node* deletedNode = linkedList->endIndex;           // 保存尾节点
    Elemtype outData = deletedNode->data;

    if (linkedList->headIndex == linkedList->endIndex) {
        // 单向链表只有一个节点，即头节点也是尾节点
        linkedList->headIndex = NULL;
        linkedList->endIndex = NULL;
    } else {
        // 找到倒数第二个节点
        SL_node* cur = linkedList->headIndex;
        while (cur->next != linkedList->endIndex) {
            cur = cur->next;
            // 防御性检查：防止单向链表结构损坏
            if (cur == NULL) {
                printf("错误：单向链表结构损坏，无法找到尾节点前驱\n");
                return UINT32_MAX;
            }
        }
        // 更新尾指针
        linkedList->endIndex = cur;
        cur->next = NULL;                               // 断开与原尾节点的链接
    }

    linkedList->length--;                               // 更新单向链表长度
    free(deletedNode);                                  // 释放被删除节点的内存
    return outData;                                     // 返回被删除节点的数据
}


/**
 * @brief 从单向链表中删除指定节点，返回被删除节点的数据
 *
 * 该函数删除单向链表中的指定节点，释放其内存，并更新单向链表的相关属性。
 * 支持删除头节点、尾节点和中间节点三种情况：
 * - 如果删除的是头节点，调用delHead函数处理；
 * - 如果删除的是尾节点，调用delEnd函数处理；
 * - 如果删除的是中间节点，找到其前驱节点，更新前驱节点的next指针。
 *
 * @param linkedList 单向链表指针（需保证单向链表结构有效）
 * @param node 待删除的节点指针（需保证是单向链表中的有效节点）
 * @return Elemtype 被删除节点的数据；若失败（如单向链表为空、节点无效），返回 UINT32_MAX
 * @warning 传入的节点指针必须是单向链表中的有效节点，否则可能导致错误
 */
Elemtype SL_deleteNode(
    SL_link* const linkedList, 
    SL_node* const node
) {
    // 检查单向链表是否为空
    if (linkedList->headIndex == NULL || linkedList->length == 0) {
        printf("错误：单向链表为空，无法删除节点\n");
        return UINT32_MAX;
    }

    // 检查节点是否为NULL
    if (node == NULL) {
        printf("传入的节点指针为NULL\n");
        return UINT32_MAX;
    }

    // 删除头节点
    if (node == linkedList->headIndex) {
        return SL_delHead(linkedList);
    }
    // 删除尾节点
    else if (node == linkedList->endIndex) {
        return SL_delEnd(linkedList);
    }
    // 删除中间节点
    else {
        SL_node* prev = linkedList->headIndex;
        // 找到node的前驱节点
        while (prev != NULL && prev->next != node) {
            prev = prev->next;
        }

        // 如果未找到前驱节点，说明node不在单向链表中
        if (prev == NULL) {
            printf("未找到目标节点，无法删除\n");
            return UINT32_MAX;
        }

        // 更新前驱节点的next指针，跳过node
        prev->next = node->next;

        // 如果删除的是尾节点，更新尾指针
        if (node == linkedList->endIndex) {
            linkedList->endIndex = prev;
        }

        Elemtype outData = node->data;
        free(node);  // 释放节点内存
        linkedList->length--;  // 更新单向链表长度
        return outData;
    }
}


/**
 * @brief 删除单向链表中指定索引的节点，返回被删除节点的数据
 *
 * 该函数删除单向链表中指定索引位置的节点，释放其内存，并更新单向链表的相关属性。
 * 支持删除头节点（索引0）、尾节点（索引length-1）和中间节点三种情况：
 * - 如果删除的是头节点，调用delHead函数处理；
 * - 如果删除的是尾节点，调用delEnd函数处理；
 * - 如果删除的是中间节点，找到其前驱节点，更新前驱节点的next指针。
 * 如果索引越界或单向链表为空，则打印错误信息并返回UINT32_MAX。
 *
 * @param linkedList 单向链表指针（需保证单向链表结构有效）
 * @param index 待删除节点的索引（从0开始计数）
 * @return Elemtype 被删除节点的数据；若失败（如索引越界、单向链表为空），返回 UINT32_MAX
 * @note 索引需满足 0 <= index < length，否则函数将返回错误
 */
Elemtype SL_deleteIndex(
    SL_link* const linkedList, 
    uint32_t index
) { 
    // 检查单向链表是否为空
    if (linkedList->headIndex == NULL || linkedList->length == 0) {
        printf("单向链表为空，无法删除索引 %u 的节点\n", index);
        return UINT32_MAX;
    }

    // 检查索引是否越界（索引需满足 0 <= index < length）
    if (index >= linkedList->length) {
        printf("错误：索引 %u 越界(单向链表长度为 %u)\n", index, linkedList->length);
        return UINT32_MAX;
    }

    // 删除头节点（索引0）
    if (index == 0) {
        return SL_delHead(linkedList);
    }
    // 删除尾节点（索引 == length - 1）
    else if (index == linkedList->length - 1) {
        return SL_delEnd(linkedList);
    }
    // 删除中间节点
    else {
        SL_node* prev = linkedList->headIndex;
        // 找到前驱节点（索引为 index - 1）
        for (uint32_t i = 0; i < index - 1; i++) {
            prev = prev->next;
            // 防御性检查：防止单向链表结构损坏
            if (prev == NULL) {
                printf("错误：单向链表结构损坏，无法找到索引 %u 的前驱节点\n", index);
                return UINT32_MAX;
            }
        }

        SL_node* deletedNode = prev->next;         // 保存被删除节点
        Elemtype outData = deletedNode->data;

        // 更新前驱节点的next指针，跳过被删除节点
        prev->next = deletedNode->next;

        // 如果删除的是尾节点，更新尾指针
        if (deletedNode == linkedList->endIndex) {
            linkedList->endIndex = prev;
        }

        linkedList->length--;                   // 更新单向链表长度
        free(deletedNode);                      // 释放被删除节点的内存
        return outData;
    }
}


/**
 * @brief 删除单向链表中指定数据的目标匹配项
 *
 * 该函数删除单向链表中与指定数据匹配的节点，释放其内存，并更新单向链表的相关属性。
 * 根据deleteCount参数的值，支持两种删除模式：
 * - deleteCount = 0：删除所有匹配的节点；
 * - deleteCount > 0：删除第deleteCount个匹配的节点（从1开始计数）。
 * 如果未找到匹配项或参数无效，则返回UINT32_MAX。
 *
 * @param linkedList 单向链表指针（需保证单向链表结构有效）
 * @param targetData 待删除的目标数据值
 * @param deleteCount 控制删除逻辑：
 *                   - 0：删除所有匹配项
 *                   - >0：删除第 deleteCount 个匹配项（从1开始计数）
 * @return Elemtype 被删除节点的数据；若失败（如未找到匹配项或参数无效），返回 UINT32_MAX
 * @note 当deleteCount=0时，函数会删除所有匹配的节点，但只返回最后一个被删除节点的数据
 */
Elemtype SL_deleteData(
    SL_link* const linkedList,
    const Elemtype targetData,
    uint32_t deleteCount
) {
    // 检查单向链表是否为空
    if (linkedList->headIndex == NULL || linkedList->length == 0) {
        printf("单向链表为空，无法删除数据 %u\n", targetData);
        return UINT32_MAX;
    }

    SL_node* current = linkedList->headIndex;  // 当前遍历节点
    SL_node* prev = NULL;                      // 前驱节点（用于维护链接）
    uint32_t matchIndex = 0;                // 当前匹配的序号（从1开始）
    Elemtype deletedData = UINT32_MAX;      // 记录被删除节点的数据

    // 遍历单向链表查找匹配项
    while (current != NULL) {
        if (current->data == targetData) {
            matchIndex++;  // 匹配序号+1

            // 删除第 deleteCount 个匹配项（deleteCount > 0）
            if (deleteCount > 0 && matchIndex == deleteCount) {
                // 保存被删除节点及其数据
                SL_node* toDelete = current;
                deletedData = toDelete->data;

                // 判断是否为头节点
                if (toDelete == linkedList->headIndex) {
                    return SL_delHead(linkedList);
                }
                // 判断是否为尾节点
                else if (toDelete == linkedList->endIndex) {
                    return SL_delEnd(linkedList);
                }
                // 否则为中间节点
                else {
                    // 找到前驱节点
                    prev = linkedList->headIndex;
                    while (prev != NULL && prev->next != toDelete) {
                        prev = prev->next;
                        // 防御性检查：防止单向链表结构损坏
                        if (prev == NULL) {
                            printf("错误：单向链表结构损坏，无法找到匹配节点的前驱\n");
                            return UINT32_MAX;
                        }
                    }
                    if (prev == NULL) {
                        printf("错误：未找到匹配节点的前驱\n");
                        return UINT32_MAX;
                    }
                    // 更新前驱节点的next指针，跳过被删除节点
                    prev->next = toDelete->next;

                    // 如果删除的是尾节点，更新尾指针
                    if (toDelete == linkedList->endIndex) {
                        linkedList->endIndex = prev;
                    }

                    linkedList->length--;                   // 更新单向链表长度
                    free(toDelete);                         // 释放被删除节点的内存
                    return deletedData;                     // 返回被删除的数据
                }

            // 删除所有匹配项（deleteCount == 0）
            } else if (deleteCount == 0) {
                SL_node* toDelete = current;
                Elemtype dataDelete = toDelete->data;

                // 判断是否为头节点
                if (toDelete == linkedList->headIndex) {
                  SL_delHead(linkedList);
                }
                // 判断是否为尾节点
                else if (toDelete == linkedList->endIndex) {
                  SL_delEnd(linkedList);
                }
                // 否则为中间节点
                else {
                  // 找到前驱节点
                  prev = linkedList->headIndex;
                  while (prev != NULL && prev->next != toDelete) {
                      prev = prev->next;
                      // 防御性检查：防止单向链表结构损坏
                      if (prev == NULL) {
                          printf("错误：单向链表结构损坏，无法找到匹配节点的前驱\n");
                          return UINT32_MAX;
                      }
                  }
                  if (prev == NULL) {
                      printf("错误：未找到匹配节点的前驱\n");
                      return UINT32_MAX;
                  }
                  // 更新前驱节点的next指针，跳过被删除节点
                  prev->next = toDelete->next;

                  // 如果删除的是尾节点，更新尾指针
                  if (toDelete == linkedList->endIndex) {
                      linkedList->endIndex = prev;
                  }

                  linkedList->length--;                   // 更新单向链表长度
                  free(toDelete);                         // 释放被删除节点的内存
                  // 继续循环以删除下一个匹配项（不立即返回）
                  current = linkedList->headIndex;
                  matchIndex = 0; // 重置匹配序号
                  continue;
                }

                // 如果是中间节点，删除并继续
                dataDelete = toDelete->data;
                if (toDelete == linkedList->headIndex) {
                  SL_delHead(linkedList);
                }
                else if (toDelete == linkedList->endIndex) {
                  SL_delEnd(linkedList);
                }
                else {
                  prev = linkedList->headIndex;
                  while (prev != NULL && prev->next != toDelete) {
                    prev = prev->next;
                    if (prev == NULL) {
                      printf("错误：单向链表结构损坏，无法找到匹配节点的前驱\n");
                      return UINT32_MAX;
                    }
                  }
                  if (prev == NULL) {
                    printf("错误：未找到匹配节点的前驱\n");
                    return UINT32_MAX;
                  }
                  prev->next = toDelete->next;
                  if (toDelete == linkedList->endIndex) {
                    linkedList->endIndex = prev;
                  }
                  linkedList->length--;
                  free(toDelete);
                }
                // 继续循环以删除下一个匹配项
                current = linkedList->headIndex;
                matchIndex = 0; // 重置匹配序号
                continue;

            // 当前节点不匹配或未找到目标序号（继续遍历）
            } else {
                prev = current;  // 更新前驱节点
                current = current->next;
            }
        } else {
            prev = current;  // 更新前驱节点
            current = current->next;
        }
    }

    // 处理未找到匹配项的情况
    if (deleteCount > 0) {
        printf("错误：未找到第 %u 个数据为 %u 的节点\n", deleteCount, targetData);
    } else {
        printf("错误：单向链表中未找到数据为 %u 的节点\n", targetData);
    }
    return UINT32_MAX;  // 未找到匹配项
}

/** @} */ // 单向链表删除操作

/**
 * @defgroup 单向链表遍历操作
 * @brief 单向链表遍历相关函数
 * @{
 */

/**
 * @brief 遍历单向链表并打印所有节点的数据
 *
 * 该函数从单向链表头开始，依次访问每个节点，
 * 打印出节点中存储的数据，数据之间用制表符分隔，每20个数据换行。
 * 遍历完成后返回单向链表的当前长度。
 *
 * @param linkedList 指向要遍历的单向链表的指针
 * @return uint32 返回单向链表的当前长度
 * @note 函数会打印每个节点的数据，格式为"数据值\t"，每20个数据后换行
 */
uint32 SL_traverseLink (
	SL_link* const linkedList
) {
	  SL_node* temNode = linkedList->headIndex;  // 创建临时节点，指向单向链表头节点
	  uint16 number = 0;  // 创建计数器

    if (temNode != NULL) {
	    // 循环，直到单向链表末尾
	    while (temNode->next) {
	    	printf("%d\t", temNode->data);
	    	temNode = temNode->next;
	    	number++;
	    	if (number == 20) {
	    		number = 0;
	    		printf("\n");
	    	}
	    }
        
	    printf("%d\n", temNode->data);
    }
    else {
        printf("\n");
    }


	  return linkedList->length;  
}

/** @} */ // 单向链表遍历操作

/** 
 * @defgroup 单向链表释放操作
 * @brief 释放单向链表所占内存
 * @{
 */

/**
 * @brief 释放链表中的所有节点内存（不释放 SL_link 结构体本身）
 * 
 * @param linkedList 指向链表管理结构体的指针（link*）
 * 
 * 说明：该函数遍历链表，从 headIndex 开始，逐个释放所有 SL_node 节点，
 *      并将链表头尾指针置空，长度置零。
 *      但它不会释放 SL_link 结构体本身。
 */
void SL_freeNodes(
    SL_link* const linkedList
) {
    if (linkedList == NULL) {
        printf("警告：传入的链表指针为 NULL，无节点可释放。\n");
        return;
    }

    SL_node* current = linkedList->headIndex;
    SL_node* nextNode = NULL;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    // 清理链表管理结构体中的字段
    linkedList->headIndex = NULL;
    linkedList->endIndex = NULL;
    linkedList->length = 0;

    printf("链表所有节点内存已释放。\n");
}

/**
 * @brief 完全释放整个链表，包括所有节点 + 链表管理结构体，并将外部指针置为 NULL
 * 
 * @param linkedListPtr 指向链表管理结构体指针的指针（link**）
 * 
 * 说明：该函数先调用 freeLinkedListNodes() 释放所有节点，
 *      然后释放 SL_link 结构体本身，并将外部指针置为 NULL，避免野指针。
 */
void SL_freeLinks(
    SL_link** linkedListPtr
) {
    if (linkedListPtr == NULL || *linkedListPtr == NULL) {
        printf("警告：传入的链表指针无效，无内存可释放。\n");
        return;
    }

    SL_link* linkedList = *linkedListPtr;

    // 调用 freeLinkedListNodes() 释放所有节点
    SL_freeNodes(linkedList);

    // 释放链表管理结构体本身（link）
    free(linkedList);

    // 将外部指针置为 NULL，避免野指针
    *linkedListPtr = NULL;

    printf("链表管理结构体和所有节点内存已完全释放。\n");
}

/** @} */ // 单向链表释放操作

/**
 * @defgroup 单向链表其它操作
 * @brief 单向链表其它操作相关函数
 * @{
 */

/**
 * 使用快慢指针方法在单链表中查找指定倒数索引位置的节点
 * 
 * @param linkedList 指向链表结构的指针，包含链表的头节点和长度信息
 * @param findIndex 要查找的节点倒数索引位置（从1开始计数）
 *                  例：1 返回倒数第一个节点，即最后一个节点
 * 
 * @return 返回指向找到的节点的指针，如果索引无效则返回NULL
 *         特殊情况：索引为链表长度时返回头节点，索引为1时返回尾节点
 */
SL_node* fast_slow_find(
  SL_link* const linkedList,
  const uint32 findIndex
) {
  if (findIndex > linkedList->length || findIndex == 0) return NULL;
  else if (findIndex == linkedList->length) return linkedList->headIndex;
  else if (findIndex == 1) return linkedList->endIndex;
  else {
    SL_node* slow = linkedList->headIndex;
    SL_node* fast = linkedList->headIndex;
    for (uint32 i = 0; i < findIndex; i++) 
      fast = fast->next;
    while (fast) {
      slow = slow->next;
      fast = fast->next;
    }
    return slow;
  }
}

/**
 * 使用快慢指针方法在两个单链表中查找相同后缀的第一个节点
 * 
 * @param link1 指向第一个链表结构的指针，包含链表的头节点和长度信息
 * @param link2 指向第二个链表结构的指针，包含链表的头节点和长度信息
 * @param count 存储相同后缀的长度
 * 
 * @return 返回指向相同后缀的第一个节点的指针，如果两个链表没有相同后缀则返回NULL
 * 
 * @note 此函数一般用于字符类型的单向链表
 * 
 */
SL_node* same_suffix (
  SL_link* const link1,
  SL_link* const link2,
  uint32* const count 
) {
  uint32 ca = link1->length - link2->length;
  ca = ((ca > 0) ? ca : -ca);

  SL_node* fast = link1->length > link2->length ? link1->headIndex : link2->headIndex;
  SL_node* slow = link1->length < link2->length ? link1->headIndex : link2->headIndex;

  for (uint32 i = 0; i < ca; i++) {fast = fast->next;}

  while (fast->data != slow->data) {
    fast = fast->next;
    slow = slow->next;
  }

  *count = 0;
  while (fast) {
    (*count)++;
    fast = fast->next;
  }

  return slow;
}

/**@} */ // 单向链表其它操作

/** @} */ // 单向链表模块

