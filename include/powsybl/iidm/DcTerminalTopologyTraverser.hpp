/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCTERMINALTOPOLOGYTRAVERSER_HPP
#define POWSYBL_IIDM_DCTERMINALTOPOLOGYTRAVERSER_HPP

#include <powsybl/math/TraverseResult.hpp>

namespace powsybl {

namespace iidm {

class DcSwitch;
class DcTerminal;

namespace dc_terminal {

class TopologyTraverser {
public:
    TopologyTraverser() = default;

    TopologyTraverser(const TopologyTraverser&) = default;

    TopologyTraverser(TopologyTraverser&&) noexcept = default;

    virtual ~TopologyTraverser() noexcept = default;

    TopologyTraverser& operator=(const TopologyTraverser&) = default;

    TopologyTraverser& operator=(TopologyTraverser&&) noexcept = default;

    /**
     * Called when a DC Terminal is encountered
     */
    virtual math::TraverseResult traverse(DcTerminal& dcTerminal, bool connected) = 0;

    /**
     * Called when a DC Switch is encountered
     */
    virtual math::TraverseResult traverse(DcSwitch& dcSwitch) = 0;
};

}  // namespace dc_terminal

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCTERMINALTOPOLOGYTRAVERSER_HPP
