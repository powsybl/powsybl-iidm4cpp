/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TERMINALTOPOLOGYTRAVERSER_HPP
#define POWSYBL_IIDM_TERMINALTOPOLOGYTRAVERSER_HPP

#include <powsybl/math/TraverseResult.hpp>

namespace powsybl {

namespace iidm {

class Switch;
class Terminal;

namespace terminal {

class TopologyTraverser {
public:
    TopologyTraverser() = default;

    TopologyTraverser(const TopologyTraverser&) = default;

    TopologyTraverser(TopologyTraverser&&) noexcept = default;

    virtual ~TopologyTraverser() noexcept = default;

    TopologyTraverser& operator=(const TopologyTraverser&) = default;

    TopologyTraverser& operator=(TopologyTraverser&&) noexcept = default;

    virtual math::TraverseResult traverse(Terminal& terminal, bool connected) = 0;

    virtual math::TraverseResult traverse(Switch& aSwitch) = 0;
};

}  // namespace terminal

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TERMINALTOPOLOGYTRAVERSER_HPP
