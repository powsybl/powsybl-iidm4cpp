/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/LoadAsymmetricalAdder.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/extensions/iidm/LoadAsymmetrical.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

LoadAsymmetricalAdder::LoadAsymmetricalAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> LoadAsymmetricalAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Load>(extendable)) {
        return stdcxx::make_unique<LoadAsymmetrical>(dynamic_cast<Load&>(extendable), m_connectionType, m_deltaPa, m_deltaQa, m_deltaPb, m_deltaQb, m_deltaPc, m_deltaQc);
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Load>()));
}

LoadAsymmetricalAdder& LoadAsymmetricalAdder::withConnectionType(LoadConnectionType connectionType) {
    m_connectionType = connectionType;
    return *this;
}
LoadAsymmetricalAdder& LoadAsymmetricalAdder::withDeltaPa(double deltaPa) {
    m_deltaPa = deltaPa;
    return *this;
}
LoadAsymmetricalAdder& LoadAsymmetricalAdder::withDeltaQa(double deltaQa) {
    m_deltaQa = deltaQa;
    return *this;
}
LoadAsymmetricalAdder& LoadAsymmetricalAdder::withDeltaPb(double deltaPb) {
    m_deltaPb = deltaPb;
    return *this;
}
LoadAsymmetricalAdder& LoadAsymmetricalAdder::withDeltaQb(double deltaQb) {
    m_deltaQb = deltaQb;
    return *this;
}
LoadAsymmetricalAdder& LoadAsymmetricalAdder::withDeltaPc(double deltaPc) {
    m_deltaPc = deltaPc;
    return *this;
}
LoadAsymmetricalAdder& LoadAsymmetricalAdder::withDeltaQc(double deltaQc) {
    m_deltaQc = deltaQc;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
