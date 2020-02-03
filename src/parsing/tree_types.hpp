//
// Created by jayz on 03.02.20.
//

#ifndef PQUANTUM_PARSING_TREE_TYPES_HPP
#define PQUANTUM_PARSING_TREE_TYPES_HPP

#define PQUANTUM_PARSING_TREE_TAG(/*traits*/...) \
PQuantum::support::tree::tree_tag<__VA_ARGS__>

#define PQUANTUM_PARSING_TREE_NODE_INCARNATION(tree_tag,node_type) \
PQuantum::support::tree::node_incarnation<node_type, tree_tag>

#endif //PQUANTUM_PARSING_TREE_TYPES_HPP
