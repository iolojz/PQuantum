//
// Created by jayz on 03.02.20.
//

#ifndef PQUANTUM_SUPPORT_BOOL_V_HPP
#define PQUANTUM_SUPPORT_BOOL_V_HPP

namespace PQuantum::support {
template<class ...> static constexpr bool true_v = true;
template<class ...> static constexpr bool false_v = false;
}

#endif //PQUANTUM_SUPPORT_BOOL_V_HPP
