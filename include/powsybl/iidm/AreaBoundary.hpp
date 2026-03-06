/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_AREABOUNDARY_HPP
#define POWSYBL_IIDM_AREABOUNDARY_HPP

#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Area;
class Boundary;
class Terminal;

class AreaBoundary {

public:
    AreaBoundary() = delete;
    AreaBoundary(Area& area, Terminal& terminal, bool ac);
    AreaBoundary(Area& area, Boundary& boundary, bool ac);
    ~AreaBoundary() noexcept = default;

public:

    const Area& getArea() const;
    Area& getArea();

    stdcxx::CReference<Terminal> getTerminal() const;
    stdcxx::Reference<Terminal> getTerminal();

    stdcxx::CReference<Boundary> getBoundary() const;
    stdcxx::Reference<Boundary> getBoundary();

    bool isAc() const;

    double getP() const;
    double getQ() const;

private:
    Area& m_area;

    stdcxx::Reference<Terminal> m_terminal;
    stdcxx::Reference<Boundary> m_boundary;

    bool m_ac;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_AREABOUNDARY_HPP
