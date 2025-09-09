/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/LoadAsymmetrical.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

LoadAsymmetrical::LoadAsymmetrical(Load& load, LoadConnectionType connectionType, double deltaPa, double deltaQa, double deltaPb, double deltaQb, double deltaPc, double deltaQc) :
    Extension(load),
    m_connectionType(connectionType),
    m_deltaPa(deltaPa),
    m_deltaQa(deltaQa),
    m_deltaPb(deltaPb),
    m_deltaQb(deltaQb),
    m_deltaPc(deltaPc),
    m_deltaQc(deltaQc) {
}

void LoadAsymmetrical::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Load>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Load>()));
    }
}

const std::string& LoadAsymmetrical::getName() const {
    static std::string s_name = "loadAsymmetrical";
    return s_name;
}

const std::type_index& LoadAsymmetrical::getType() const {
    static std::type_index s_type = typeid(LoadAsymmetrical);
    return s_type;
}

double LoadAsymmetrical::getDeltaPa() const {
    return m_deltaPa;
}
double LoadAsymmetrical::getDeltaQa() const {
    return m_deltaQa;
}
double LoadAsymmetrical::getDeltaPb() const {
    return m_deltaPb;
}
double LoadAsymmetrical::getDeltaQb() const {
    return m_deltaQb;
}
double LoadAsymmetrical::getDeltaPc() const {
    return m_deltaPc;
}
double LoadAsymmetrical::getDeltaQc() const {
    return m_deltaQc;
}
LoadConnectionType LoadAsymmetrical::getConnectionType() const {
    return m_connectionType;
}

LoadAsymmetrical& LoadAsymmetrical::setDeltaPa(double deltaPa) {
    m_deltaPa = deltaPa;
    return *this;
}
LoadAsymmetrical& LoadAsymmetrical::setDeltaQa(double deltaQa) {
    m_deltaQa = deltaQa;
    return *this;
}
LoadAsymmetrical& LoadAsymmetrical::setDeltaPb(double deltaPb) {
    m_deltaPb = deltaPb;
    return *this;
}
LoadAsymmetrical& LoadAsymmetrical::setDeltaQb(double deltaQb) {
    m_deltaQb = deltaQb;
    return *this;
}
LoadAsymmetrical& LoadAsymmetrical::setDeltaPc(double deltaPc) {
    m_deltaPc = deltaPc;
    return *this;
}
LoadAsymmetrical& LoadAsymmetrical::setDeltaQc(double deltaQc) {
    m_deltaQc = deltaQc;
    return *this;
}
LoadAsymmetrical& LoadAsymmetrical::setConnectionType(LoadConnectionType connectionType) {
    m_connectionType = connectionType;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
