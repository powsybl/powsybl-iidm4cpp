/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/AreaBoundary.hpp>

#include <powsybl/iidm/Area.hpp>
#include <powsybl/iidm/Boundary.hpp>
#include <powsybl/iidm/Terminal.hpp>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

AreaBoundary::AreaBoundary(Area& area, Terminal& terminal, bool ac) :
    m_area(area),
    m_terminal(terminal),
    m_ac(ac) {
}

AreaBoundary::AreaBoundary(Area& area, Boundary& boundary, bool ac) :
    m_area(area),
    m_boundary(boundary),
    m_ac(ac) {
}

const Area& AreaBoundary::getArea() const {
    return m_area;
}

Area& AreaBoundary::getArea() {
    return m_area;
}

stdcxx::CReference<Terminal> AreaBoundary::getTerminal() const {
    return static_cast<bool>(m_terminal) ? stdcxx::cref(m_terminal) : stdcxx::cref<Terminal>();
}
stdcxx::Reference<Terminal> AreaBoundary::getTerminal() {
    return m_terminal;
}

stdcxx::CReference<Boundary> AreaBoundary::getBoundary() const {
    return static_cast<bool>(m_boundary) ? stdcxx::cref(m_boundary) : stdcxx::cref<Boundary>();
}
stdcxx::Reference<Boundary> AreaBoundary::getBoundary() {
    return m_boundary;
}

bool AreaBoundary::isAc() const {
    return m_ac;
}

double AreaBoundary::getP() const {
    if (static_cast<bool>(m_boundary)) {
        return m_boundary.get().getP();
    } else if (static_cast<bool>(m_terminal)) {
        return m_terminal.get().getP();
    }
    return stdcxx::nan();
}
double AreaBoundary::getQ() const {
    if (static_cast<bool>(m_boundary)) {
        return m_boundary.get().getQ();
    } else if (static_cast<bool>(m_terminal)) {
        return m_terminal.get().getQ();
    }
    return stdcxx::nan();
}


}  // namespace iidm

}  // namespace powsybl
