//
// Created by jayz on 10.01.20.
//

#ifndef PQUANTUM_SUPPORT_TREE_HPP
#define PQUANTUM_SUPPORT_TREE_HPP

namespace support {
template<class NodeData> struct tree_node {
	NodeData data;
	std::vector<tree_node<NodeData>> children;
};
}

#endif //PQUANTUM_SUPPORT_TREE_HPP
