/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGELEVELTOPOLOGYTRAVERSER_HPP
#define POWSYBL_IIDM_VOLTAGELEVELTOPOLOGYTRAVERSER_HPP

namespace powsybl {

namespace iidm {

class Switch;
class Terminal;

namespace voltage_level {

class TopologyTraverser {
public:
    TopologyTraverser() = default;

    TopologyTraverser(const TopologyTraverser&) = default;

    TopologyTraverser(TopologyTraverser&&) noexcept = default;

    virtual ~TopologyTraverser() noexcept = default;

    TopologyTraverser& operator=(const TopologyTraverser&) = default;

    TopologyTraverser& operator=(TopologyTraverser&&) noexcept = default;

    virtual bool traverse(Terminal& terminal, bool connected) = 0;

    virtual bool traverse(Switch& aSwitch) = 0;
};

}  // namespace voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VOLTAGELEVELTOPOLOGYTRAVERSER_HPP
