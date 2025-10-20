/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ReferencePriorityAdder.hpp>

#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePriorities.hpp>


namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ReferencePriorityAdder::ReferencePriorityAdder(ReferencePriorities& referencePriorities) :
    m_referencePriorities(referencePriorities) {
}

ReferencePriorities& ReferencePriorityAdder::add() {
    if(!static_cast<bool>(m_terminal)) {
        throw PowsyblException("Undefined terminal for reference priority.");
    }
    std::shared_ptr<ReferencePriority> referencePriority = std::make_shared<ReferencePriority>(m_terminal.get(), m_priority);
    m_referencePriorities.add(referencePriority);
    return m_referencePriorities;
}

ReferencePriorityAdder& ReferencePriorityAdder::setTerminal(Terminal& terminal) {
    m_terminal = terminal;
    return *this;
}

ReferencePriorityAdder& ReferencePriorityAdder::setPriority(unsigned long priority) {
    m_priority = priority;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
