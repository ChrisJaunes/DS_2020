#include "BPlusTreeUtils.h"
int  BPlusTreeUtils::bptcntR = 0;
int  BPlusTreeUtils::bptcntW = 0;
size_t BPlusTreeUtils::db_EOF(FILE* file) {
	fseek(file, 0, SEEK_END);
	return (size_t)_ftelli64(file);
}
size_t BPlusTreeUtils::db_read(FILE* file, off_t off, void* buf, size_t size, size_t count) {
	bptcntR += size/1024;
	fseek(file, off, SEEK_SET);
	int res = fread(buf, size, count, file);
	return res;
}

size_t BPlusTreeUtils::db_write(FILE* file, off_t off, const void* buf, size_t size, size_t count) {
	bptcntW += size/1024;
	fseek(file, off, SEEK_SET);
	int res = fwrite(buf, size, count, file);
	fflush(file);
	return res;
}
