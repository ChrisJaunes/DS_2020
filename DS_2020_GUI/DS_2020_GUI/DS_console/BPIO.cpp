#include "BPIO.h"

size_t BPIO::db_read(FILE* file, off_t off, void* buf, size_t size, size_t count) {
	fseek(file, off, SEEK_SET);
	return fread(buf, size, count, file);
}

size_t BPIO::db_write(FILE* file, off_t off, const void* buf, size_t size, size_t count) {
	fseek(file, off, SEEK_SET);
	int res = fwrite(buf, size, count, file);
	fflush(file);
	return res;
}
