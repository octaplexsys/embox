#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

static void *pthread_test(void *opaque) {
	int i = 0;

	usleep(2);
	printf("> exiting from usleep (%d)\n", i++);

	usleep(2);
	printf("> exiting from usleep (%d)\n", i++);

	return NULL;
}

int main(int argc, char **argv) {
	pthread_t thread;

	if (0 != pthread_create(&thread, NULL, pthread_test, NULL)) {
		printf("pthread_create failed\n");
		return -1;
	}

	pthread_join(thread, NULL);

	printf("pthread_test finished!\n");

	return 0;
}
