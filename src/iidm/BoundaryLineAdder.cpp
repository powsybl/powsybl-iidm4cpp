/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/BoundaryLineAdder.hpp>

#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

BoundaryLineAdder::BoundaryLineAdder(VoltageLevel& voltageLevel) :
    InjectionAdder(voltageLevel) {
}

BoundaryLine& BoundaryLineAdder::add() {
    Network&  network = getNetwork();
    network.setValidationLevelIfGreaterThan(checkP0(*this, m_p0, network.getMinimumValidationLevel()));
    network.setValidationLevelIfGreaterThan(checkQ0(*this, m_q0, network.getMinimumValidationLevel()));
    checkR(*this, m_r);
    checkX(*this, m_x);
    checkG(*this, m_g);
    checkB(*this, m_b);

    std::unique_ptr<BoundaryLine::Generation> ptrGeneration = m_generationAdder ? m_generationAdder->build() : nullptr;

    std::unique_ptr<BoundaryLine> ptrBoundaryLine = stdcxx::make_unique<BoundaryLine>(network, checkAndGetUniqueId(), getName(), isFictitious(),
                                                                                      m_p0, m_q0, m_r, m_x, m_g, m_b, m_pairingKey, std::move(ptrGeneration));
    auto& boundaryLine = network.checkAndAdd<BoundaryLine>(std::move(ptrBoundaryLine));

    Terminal& terminal = boundaryLine.addTerminal(checkAndGetTerminal());
    getVoltageLevel().getTopologyModel().attach(terminal, false);

    return boundaryLine;
}

const std::string& BoundaryLineAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Boundary line";

    return s_typeDescription;
}

BoundaryLineAdder::GenerationAdder BoundaryLineAdder::newGeneration() {
    return GenerationAdder(*this);
}

BoundaryLineAdder& BoundaryLineAdder::setB(double b) {
    m_b = b;
    return *this;
}

BoundaryLineAdder& BoundaryLineAdder::setG(double g) {
    m_g = g;
    return *this;
}

void BoundaryLineAdder::setGenerationAdder(const GenerationAdder& generationAdder) {
    m_generationAdder.emplace(generationAdder);
}

BoundaryLineAdder& BoundaryLineAdder::setP0(double p0) {
    m_p0 = p0;
    return *this;
}

BoundaryLineAdder& BoundaryLineAdder::setQ0(double q0) {
    m_q0 = q0;
    return *this;
}

BoundaryLineAdder& BoundaryLineAdder::setR(double r) {
    m_r = r;
    return *this;
}

BoundaryLineAdder& BoundaryLineAdder::setPairingKey(const std::string& pairingKey) {
    m_pairingKey = pairingKey;
    return *this;
}

BoundaryLineAdder& BoundaryLineAdder::setX(double x) {
    m_x = x;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
