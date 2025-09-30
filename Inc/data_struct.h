/*
 * @file data_struct.h
 * @brief 数据结构模块接口定义头文件
 * @author ringtree
 * @date 2025-08-26
 * @version 1.0
 * @copyright Copyright (c) 2025 ringtree. All rights reserved.
 *
 * 本文件定义了项目中使用的基础数据结构及相关操作函数接口，
 *
 * 使用说明：
 * - 本头文件声明了单向链表模块的全部操作函数接口，具体实现详见 data_struct.c
 * - 当前链表节点存储的数据类型为 Elemtype（默认为 uint32），如需修改数据类型，请调整文件中的 typedef 定义
 * - 所有函数接口均遵循模块化设计原则，参数与返回值定义清晰，详见对应函数声明
 *
 * 版本历史：
 * - 1.0 (2025-08-09)：初始版本，实现单向链表基本功能
 */
#pragma once
#ifndef __DATA_STRUCT_H__
#define __DATA_STRUCT_H__ 

/* include ---------------------------------------------------- */
#include "main.h"

/* define ----------------------------------------------------- */
/**
 * @defgroup 单向链表模块
 * @brief 单向链表排序方式枚举
 * @{
 */
enum sort {
	ASC = 0x00,
	DESC = 0x01
}; 

/**
 * @defgroup 单向链表模块
 * @brief 单向链表数据结构及其操作
 *
 * 本模块实现了单向链表的基本操作，包括创建、初始化、插入、删除、查找和遍历等功能。
 * 链表节点存储整型数据，支持头插法、尾插法、指定位置插入等多种插入方式，
 * 以及按索引删除、按值删除等多种删除方式。
 * @{
 */

/**
 * @brief 链表节点数据类型定义
 *
 * 定义链表中存储的数据类型为32位整型。
 */
typedef int32 Elemtype;

typedef struct SL_node SL_node;

/**
 * @brief 链表节点结构体
 *
 * 定义链表节点的结构，包含数据域和指向下一个节点的指针。
 */
typedef struct SL_node {
	Elemtype data;        ///< 节点数据
	struct SL_node* next;    ///< 指向下一个节点的指针
} SL_node;

/**
 * @brief 链表结构体
 *
 * 定义链表的结构，包含头节点指针、尾节点指针和链表长度。
 */
typedef struct SL_linkedList {
	SL_node* headIndex;      ///< 指向链表头节点的指针
	SL_node* endIndex;       ///< 指向链表尾节点的指针
	uint32 length;        ///< 链表长度（节点数量）
} SL_link;

/**
 * @brief 链表相关操作函数声明
 */

SL_link* SL_inifLink (
	void
);

/** @} */ // 链表初始化操作

/**
 * @defgroup 链表插入操作
 * @brief 链表节点插入相关函数
 * @{
 */

void SL_insertHead (
	SL_link* const linkedList,
	const Elemtype inputData
);

void SL_add (
	SL_link* const linkedList,
	const Elemtype inputData
);

void SL_extind(
	SL_link* const linkedList,
	const uint32 count,
	...
);

void SL_insert (
	SL_link* const linkedList,
	const Elemtype inputData,
	const uint32 index 
);

/** @} */ // 链表插入操作

/**
 * @defgroup 链表查找操作
 * @brief 链表节点查找相关函数
 * @{
 */

uint32 SL_count (
	SL_link* const linkedList,
	const Elemtype findData
);

SL_node** SL_find (
    SL_link* const linkedList,
    const Elemtype findData,
    uint32* outCount  
);

uint32 SL_getIndex (
	SL_link* const linkedList,
	const Elemtype findData
);

void* SL_get_set(
  SL_link* const linked
);

uint32 SL_josephusSurvivor (
  SL_link* const link,
  uint32 n
);

/** @} */ // 链表查找操作

/**
 * @defgroup 单向链表修改操作
 * @brief 单向链表修改相关函数
 * @{
 */

void SL_sort (
	SL_link* const linkedList,
	enum sort const way
);

uint16 SL_reverse (
  SL_link* const linked
);

/** @} */ // 单向链表修改操作

/**
 * @defgroup 链表删除操作
 * @brief 链表节点删除相关函数
 * @{
 */

Elemtype SL_delHead(
	SL_link* const linkedList
);

Elemtype SL_delEnd(
	SL_link* const linkedList
);

Elemtype SL_deleteNode (
	SL_link* const linkedList, 
	SL_node* const node
);

Elemtype SL_deleteIndex(
	SL_link* const linkedList, 
	uint32 index
);

Elemtype SL_deleteData(
    SL_link* const linkedList,
    const Elemtype targetData,
    uint32 deleteCount
);

/** @} */ // 链表删除操作

/**
 * @defgroup 链表遍历操作
 * @brief 链表遍历相关函数
 * @{
 */

uint32 SL_traverseLink (
	SL_link* const linkedList
);

/** @} */ // 链表遍历操作

/** 
 * @defgroup 单向链表释放操作
 * @brief 释放单向链表所占内存
 * @{
 */

void SL_freeNodes(
    SL_link* const linkedList
);

void SL_freeLinks(
    SL_link** linkedListPtr
);

/** @} */ // 单向链表释放操作

/**
 * @defgroup 单向链表其它操作
 * @brief 单向链表其它操作相关函数
 * @{
 */

SL_node* fast_slow_find(
  SL_link* const linkedList,
  const uint32 findIndex
);

SL_node* same_suffix (
  SL_link* const link1,
  SL_link* const link2,
  uint32* const count 
);

/**@} */ // 单向链表其它操作

/** @} */ // 单向链表模块


 
#endif /* !__DATA_STRUCT_H__ */