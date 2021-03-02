#include <gtest/gtest.h>
#include "../server.cpp"
Server* server = new Server();

TEST(HandleFunctionTest, StringWithNumbers) {
	EXPECT_EQ("1 15 20 30\n66", server->handle("20 apples, 30 bananas, 15 peaches and 1 watermelon"));
	EXPECT_EQ("20\n20", server->handle("20 apples"));
}
TEST(HandleFunctionTest, StringWithoutNumbers) {
	EXPECT_EQ("fruits", server->handle("fruits"));
	EXPECT_EQ("", server->handle(""));
}
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}