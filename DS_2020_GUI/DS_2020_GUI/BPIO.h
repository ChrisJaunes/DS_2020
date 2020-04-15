#pragma once
#include "pch.h"
namespace BPIO
{
	int db_read(FILE*, off_t, void*, size_t);
	int db_write(FILE*, off_t, const void*, size_t);

};

