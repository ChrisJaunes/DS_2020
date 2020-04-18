#pragma once
#include "pch.h"
namespace BPlusTreeUtils {
	extern int bptcnt;
	size_t db_read(FILE* , off_t , void* , size_t , size_t );
	size_t db_write(FILE* , off_t , const void* , size_t, size_t );
};

