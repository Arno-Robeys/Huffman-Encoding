#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <cstdint>
#include <cassert>
#include <memory>
#include <functional>

namespace data {

		template<typename T>
		class Node {
		public:
			virtual ~Node() {}
		};

		template<typename T>
		class Branch : public Node<T> {
		public:
			Branch(std::unique_ptr<Node<T>> left, std::unique_ptr<Node<T>> right) : left_child(std::move(left)), right_child(std::move(right)) {
				//Both children must be non-null because a branch must have two children
				assert(left_child != nullptr && right_child != nullptr);
			}

			const Node<T>& get_left_child() const {
				return *left_child;
			}

			const Node<T>& get_right_child() const {
				return *right_child;
			}

		private:
			std::unique_ptr<Node<T>> left_child;
			std::unique_ptr<Node<T>> right_child;
		};

		template<typename T>
		class Leaf : public Node<T> {

		public:
			Leaf(const T& value) : value(value) {}


			T get_value() const {
				return value;
			}

		private:
			T value;
		};

		template<typename IN, typename OUT>
		std::unique_ptr<Node<OUT>> map(const Node<IN>& tree, std::function<OUT(const IN&)> function) {
			if (const Leaf<IN>* leaf = dynamic_cast<const Leaf<IN>*>(&tree)) {
				//If tree is a leaf, apply function to its value and return a new leaf with the result
				return std::make_unique<Leaf<OUT>>(function(leaf->get_value()));
			}
			else if (const Branch<IN>* branch = dynamic_cast<const Branch<IN>*>(&tree)) {
				//If tree is a branch, recursively map its children and return a new branch with the results
				auto new_left = map(branch->get_left_child(), function);
				auto new_right = map(branch->get_right_child(), function);
				//Create a new branch with the new children
				return std::make_unique<Branch<OUT>>(std::move(new_left), std::move(new_right));
			}
			return nullptr;
		}
}

#endif