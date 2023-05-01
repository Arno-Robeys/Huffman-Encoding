#ifdef TEST_BUILD

#include "catch.hpp"
#include "data/binary-tree.h"

TEST_CASE("Binary Tree Map function with strings") {
	auto leaf1 = std::make_unique<data::Leaf<std::string>>("test1");
	auto leaf2 = std::make_unique<data::Leaf<std::string>>("test2");
	auto leaf3 = std::make_unique<data::Leaf<std::string>>("test3");
	auto branch = std::make_unique<data::Branch<std::string>>(std::move(leaf1), std::move(leaf2));
	auto root = std::make_unique<data::Branch<std::string>>(std::move(branch), std::move(leaf3));

	auto mapped_tree = data::map<std::string, std::string>(*root, [](const std::string& value) { return value + "!!!"; });


    auto mapped_root = dynamic_cast<const data::Branch<std::string>*>(mapped_tree.get());
    REQUIRE(mapped_root != nullptr);

    auto mapped_leaf = dynamic_cast<const data::Leaf<std::string>*>(&mapped_root->get_right_child());
    REQUIRE(mapped_leaf != nullptr);
    REQUIRE(mapped_leaf->get_value() == "test3!!!");

	auto mapped_branch = dynamic_cast<const data::Branch<std::string>*>(&mapped_root->get_left_child());
	auto mapped_left_leaf = dynamic_cast<const data::Leaf<std::string>*>(&mapped_branch->get_left_child());
	auto mapped_right_leaf = dynamic_cast<const data::Leaf<std::string>*>(&mapped_branch->get_right_child());
	REQUIRE(mapped_branch != nullptr);
	REQUIRE(mapped_left_leaf != nullptr);
	REQUIRE(mapped_right_leaf != nullptr);
	REQUIRE(mapped_left_leaf->get_value() == "test1!!!");
	REQUIRE(mapped_right_leaf->get_value() == "test2!!!");

}

TEST_CASE("Binary Tree Map function with integers") {
	auto leaf1 = std::make_unique<data::Leaf<int>>(1);
	auto leaf2 = std::make_unique<data::Leaf<int>>(2);
	auto leaf3 = std::make_unique<data::Leaf<int>>(3);
	auto branch = std::make_unique<data::Branch<int>>(std::move(leaf1), std::move(leaf2));
	auto root = std::make_unique<data::Branch<int>>(std::move(branch), std::move(leaf3));
	auto mapped_tree = data::map<int, int>(*root, [](const int& value) { return value * 2; });

	auto mapped_root = dynamic_cast<const data::Branch<int>*>(mapped_tree.get());
	REQUIRE(mapped_root != nullptr);

	auto mapped_leaf = dynamic_cast<const data::Leaf<int>*>(&mapped_root->get_right_child());
	REQUIRE(mapped_leaf != nullptr);
	REQUIRE(mapped_leaf->get_value() == 6);

	auto mapped_branch = dynamic_cast<const data::Branch<int>*>(&mapped_root->get_left_child());
	auto mapped_left_leaf = dynamic_cast<const data::Leaf<int>*>(&mapped_branch->get_left_child());
	auto mapped_right_leaf = dynamic_cast<const data::Leaf<int>*>(&mapped_branch->get_right_child());
	REQUIRE(mapped_branch != nullptr);
	REQUIRE(mapped_left_leaf != nullptr);
	REQUIRE(mapped_right_leaf != nullptr);
	REQUIRE(mapped_left_leaf->get_value() == 2);
	REQUIRE(mapped_right_leaf->get_value() == 4);
}

#endif
