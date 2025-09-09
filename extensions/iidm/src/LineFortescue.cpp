/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/LineFortescue.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

LineFortescue::LineFortescue(Line& line, double rz, double xz, bool openPhaseA, bool openPhaseB, bool openPhaseC) :
    Extension(line),
    m_rz(rz),
    m_xz(xz),
    m_openPhaseA(openPhaseA),
    m_openPhaseB(openPhaseB),
    m_openPhaseC(openPhaseC) {
}

void LineFortescue::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Line>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Line>()));
    }
}

const std::string& LineFortescue::getName() const {
    static std::string s_name = "lineFortescue";
    return s_name;
}

const std::type_index& LineFortescue::getType() const {
    static std::type_index s_type = typeid(LineFortescue);
    return s_type;
}

double LineFortescue::getRz() const {
    return m_rz;
}
double LineFortescue::getXz() const {
    return m_xz;
}

LineFortescue& LineFortescue::setRz(double rz) {
    m_rz = rz;
    return *this;
}
LineFortescue& LineFortescue::setXz(double xz) {
    m_xz = xz;
    return *this;
}

bool LineFortescue::isOpenPhaseA() const {
    return m_openPhaseA;
}
bool LineFortescue::isOpenPhaseB() const {
    return m_openPhaseB;
}
bool LineFortescue::isOpenPhaseC() const {
    return m_openPhaseC;
}

LineFortescue& LineFortescue::setOpenPhaseA(bool openPhaseA) {
    m_openPhaseA = openPhaseA;
    return *this;
}
LineFortescue& LineFortescue::setOpenPhaseB(bool openPhaseB) {
    m_openPhaseB = openPhaseB;
    return *this;
}
LineFortescue& LineFortescue::setOpenPhaseC(bool openPhaseC) {
    m_openPhaseC = openPhaseC;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

