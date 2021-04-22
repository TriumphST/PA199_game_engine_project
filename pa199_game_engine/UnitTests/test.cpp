#include "pch.h"
#include "../pa199_game_engine/Vector3.h"
TEST(TestVector3, TestElements) {
	Vector3 v = Vector3(1.0f, 1.0f, 1.0f);
	EXPECT_EQ(v.x, 1.0f);
	//EXPECT_EQ(v->y, 0.0f);
	//EXPECT_EQ(v->z, 0.0f);
	//EXPECT_TRUE(true);
}