#include "BPlusTreeUtils.h"
double  DS_BPlusTree::bptcntR = 0;
double  DS_BPlusTree::bptcntW = 0;
size_t DS_BPlusTree::db_EOF(FILE* file) {
	_fseeki64(file, 0, SEEK_END);
	return (size_t)_ftelli64(file);
}
size_t DS_BPlusTree::db_read(FILE* file, size_t off, void* buf, size_t size, size_t count) {
	bptcntR += size/1024.0;
	_fseeki64(file, off, SEEK_SET);
	size_t res = fread(buf, size, count, file);
	return res;
}

size_t DS_BPlusTree::db_write(FILE* file, size_t off, const void* buf, size_t size, size_t count) {
	bptcntW += size/1024.0;
	_fseeki64(file, off, SEEK_SET);
	size_t res = fwrite(buf, size, count, file);
	fflush(file);
	return res;
}
