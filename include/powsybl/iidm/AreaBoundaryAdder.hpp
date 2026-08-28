/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_AREABOUNDARYADDER_HPP
#define POWSYBL_IIDM_AREABOUNDARYADDER_HPP

#include <powsybl/iidm/PropertiesHolder.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Area;
class AreaBoundary;
class Boundary;
class Terminal;

class AreaBoundaryAdder : public PropertiesHolder {

public:
    AreaBoundaryAdder(Area& area);
    ~AreaBoundaryAdder() noexcept = default;

    Area& add();

    AreaBoundaryAdder& setTerminal(Terminal& terminal);
    AreaBoundaryAdder& setBoundary(Boundary& boundary);
    AreaBoundaryAdder& setAc(bool ac);


private:
    Area& m_area;
    stdcxx::Reference<Terminal> m_terminal;
    stdcxx::Reference<Boundary> m_boundary;
    stdcxx::optional<bool> m_ac;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_AREABOUNDARYADDER_HPP