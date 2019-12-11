#include <gtest/gtest.h>

#include <ringqueue.h>

TEST(QueueDequeueTest, ringqueue) {
	size_t i, len;
	char const *data[] = {"first", "second", "third", "fourth",
	                      "fifth", "sixth",  "seven", "eight"};
	char *str;
	ringqueue_t queue = ringqueue_new();
	len = 5;
	ringqueue_init(queue, len);

	for (i = 0; i < len; i++) {
		//fprintf(stderr, "push: %s\n", data[i]);
		ASSERT_NE(ringqueue_push(queue, (void *) data[i]), -1);
	}
	ASSERT_EQ(ringqueue_size(queue), len);

	/* check overflow */
	ASSERT_EQ(ringqueue_push(queue, (void *) data[i]), -1);

	/* popfront test */
	str = (char *) ringqueue_popfront(queue);
	EXPECT_STREQ(str, data[0]);
	ASSERT_EQ(ringqueue_size(queue), len - 1);
	//fprintf(stderr, "popfront: %s\n", str);

	/* popback test */
	str = (char *) ringqueue_popback(queue);
	EXPECT_STREQ(str, data[len - 1]);
	ASSERT_EQ(ringqueue_size(queue), len - 2);
	//fprintf(stderr, "popback: %s\n", str);

	//fprintf(stderr, "push: %s\n", data[i]);
	ASSERT_NE(ringqueue_push(queue, (void *) data[i++]), -1);
	//fprintf(stderr, "push: %s\n", data[i]);
	ASSERT_NE(ringqueue_push(queue, (void *) data[i++]), -1);

	i = 0;
	char const *verify[] = {"second", "third", "fourth", "sixth",  "seven", "eight"};
	while ((str = (char *) ringqueue_popfront(queue)) != NULL) {
		//fprintf(stderr, "popfront: %s\n", str);
		EXPECT_STREQ(str, verify[i++]);
	}
	ASSERT_EQ(i, len);
	ASSERT_EQ(ringqueue_size(queue), 0);
	ASSERT_EQ(ringqueue_empty(queue), 1);
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
