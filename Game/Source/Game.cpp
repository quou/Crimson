#include <Crimson.h>

int main(void) {
	Crimson::Log(CR_LOGTYPE_WARNING, "%d\n", 10);

	CR_ASSERT(false, "%s\n", "Hello there");

	CR_LOG_ERROR("%d\n", 3);
}
