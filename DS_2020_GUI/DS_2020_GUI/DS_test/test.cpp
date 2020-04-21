#include "test.h"
#ifdef TEST_DEBUG
#include "config.h"
#include "qdebug.h"

void test_BPT();

namespace FST {
	void tmain() {
		test_BPT();
	}
}
#endif