/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/AreaBoundaryAdder.hpp>

#include <powsybl/iidm/Area.hpp>
#include <powsybl/iidm/AreaBoundary.hpp>
#include <powsybl/iidm/Boundary.hpp>
#include <powsybl/iidm/Terminal.hpp>

namespace powsybl {

namespace iidm {

AreaBoundaryAdder::AreaBoundaryAdder(Area& area) :
    m_area(area) {
}

Area& AreaBoundaryAdder::add() {
    if (!m_ac.has_value()) {
        throw PowsyblException("AreaBoundary AC flag is not set.");
    }
    //Remove before adding to avoid duplicates and allow update ac flag
    if (static_cast<bool>(m_boundary)) {
        m_area.removeAreaBoundary(m_boundary);
        m_area.addAreaBoundary(std::make_shared<AreaBoundary>(m_area, m_boundary, *m_ac));
    } else if(static_cast<bool>(m_terminal)) {
        m_area.removeAreaBoundary(m_terminal);
        m_area.addAreaBoundary(std::make_shared<AreaBoundary>(m_area, m_terminal, *m_ac));
    } else {
        throw PowsyblException("No AreaBoundary element (terminal or boundary) is set.");
    }
    return m_area;
}

AreaBoundaryAdder& AreaBoundaryAdder::setTerminal(Terminal& terminal) {
    m_boundary = stdcxx::Reference<Boundary>();
    m_terminal = terminal;
    return *this;
}
AreaBoundaryAdder& AreaBoundaryAdder::setBoundary(Boundary& boundary) {
    m_terminal = stdcxx::Reference<Terminal>();
    m_boundary = boundary;
    return *this;
}
AreaBoundaryAdder& AreaBoundaryAdder::setAc(bool ac) {
    m_ac = ac;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
