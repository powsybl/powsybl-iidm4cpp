/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/GeneratorFortescue.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorFortescue::GeneratorFortescue(Generator& generator, bool grounded, double rz, double xz, double rn, double xn, double groundingR, double groundingX) :
    Extension(generator),
    m_grounded(grounded),
    m_rz(rz),
    m_xz(xz),
    m_rn(rn),
    m_xn(xn),
    m_groundingR(groundingR),
    m_groundingX(groundingX) {
}

void GeneratorFortescue::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Generator>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Generator>()));
    }
}

const std::string& GeneratorFortescue::getName() const {
    static std::string s_name = "generatorFortescue";
    return s_name;
}

const std::type_index& GeneratorFortescue::getType() const {
    static std::type_index s_type = typeid(GeneratorFortescue);
    return s_type;
}

bool GeneratorFortescue::isGrounded() const {
    return m_grounded;
}
GeneratorFortescue& GeneratorFortescue::setGrounded(bool grounded) {
    m_grounded = grounded;
    return *this;
}

double GeneratorFortescue::getRz() const {
    return m_rz;
}
double GeneratorFortescue::getXz() const {
    return m_xz;
}
double GeneratorFortescue::getRn() const {
    return m_rn;
}
double GeneratorFortescue::getXn() const {
    return m_xn;
}
double GeneratorFortescue::getGroundingR() const {
    return m_groundingR;
}
double GeneratorFortescue::getGroundingX() const {
    return m_groundingX;
}

GeneratorFortescue& GeneratorFortescue::setRz(double rz) {
    m_rz = rz;
    return *this;
}
GeneratorFortescue& GeneratorFortescue::setXz(double xz) {
    m_xz = xz;
    return *this;
}
GeneratorFortescue& GeneratorFortescue::setRn(double rn) {
    m_rn = rn;
    return *this;
}
GeneratorFortescue& GeneratorFortescue::setXn(double xn) {
    m_xn = xn;
    return *this;
}
GeneratorFortescue& GeneratorFortescue::setGroundingR(double groundingR) {
    m_groundingR = groundingR;
    return *this;
}
GeneratorFortescue& GeneratorFortescue::setGroundingX(double groundingX) {
    m_groundingX = groundingX;
    return *this;
}


}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

