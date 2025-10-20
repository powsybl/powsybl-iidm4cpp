/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCEPRIORITYADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCEPRIORITYADDER_HPP

#include <powsybl/iidm/extensions/iidm/ReferencePriority.hpp>

namespace powsybl {

namespace iidm {

class Terminal;

namespace extensions {

namespace iidm {

class ReferencePriorities;

class ReferencePriorityAdder {
public:
    explicit ReferencePriorityAdder(ReferencePriorities& referencePriorities);

    ~ReferencePriorityAdder() noexcept = default;
    ReferencePriorityAdder(const ReferencePriorityAdder&) = default;
    ReferencePriorityAdder(ReferencePriorityAdder&&) = default;
    ReferencePriorityAdder& operator=(const ReferencePriorityAdder&) = delete;
    ReferencePriorityAdder& operator=(ReferencePriorityAdder&&) = delete;


    ReferencePriorities& add();

    ReferencePriorityAdder& setTerminal(Terminal& terminal);

    ReferencePriorityAdder& setPriority(unsigned long priority);

private:
    ReferencePriorities& m_referencePriorities;

    stdcxx::Reference<Terminal> m_terminal;

    unsigned long m_priority = 0;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCEPRIORITYADDER_HPP