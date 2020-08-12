/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

TwoWindingsTransformerAdder::TwoWindingsTransformerAdder(Substation& substation) :
    m_substation(substation) {
}

TwoWindingsTransformer& TwoWindingsTransformerAdder::add() {
    VoltageLevel& voltageLevel1 = checkAndGetVoltageLevel1();
    VoltageLevel& voltageLevel2 = checkAndGetVoltageLevel2();
    if (!stdcxx::areSame(voltageLevel1.getSubstation(), m_substation) || !stdcxx::areSame(voltageLevel2.getSubstation(), m_substation)) {
        throw ValidationException(*this, stdcxx::format("the 2 windings of the transformer shall belong to the substation '%1%' ('%2%', '%3%')",
                                                         m_substation.getId(), voltageLevel1.getSubstation().getId(), voltageLevel2.getSubstation().getId()));
    }
    std::unique_ptr<Terminal> ptrTerminal1 = checkAndGetTerminal1(voltageLevel1);
    std::unique_ptr<Terminal> ptrTerminal2 = checkAndGetTerminal2(voltageLevel2);

    checkR(*this, m_r);
    checkX(*this, m_x);
    checkG(*this, m_g);
    checkB(*this, m_b);
    checkRatedU1(*this, m_ratedU1);
    checkRatedU2(*this, m_ratedU2);
    checkRatedS(*this, m_ratedS);

    // check that the two windings transformer is attachable on both side
    voltageLevel1.attach(*ptrTerminal1, true);
    voltageLevel2.attach(*ptrTerminal2, true);

    std::unique_ptr<TwoWindingsTransformer> ptrTransformer = stdcxx::make_unique<TwoWindingsTransformer>(checkAndGetUniqueId(), getName(), isFictitious(), m_substation, m_r, m_x, m_g, m_b, m_ratedU1, m_ratedU2, m_ratedS);
    auto& transformer = getNetwork().checkAndAdd<TwoWindingsTransformer>(std::move(ptrTransformer));

    Terminal& terminal1 = transformer.addTerminal(std::move(ptrTerminal1));
    Terminal& terminal2 = transformer.addTerminal(std::move(ptrTerminal2));
    voltageLevel1.attach(terminal1, false);
    voltageLevel2.attach(terminal2, false);

    return transformer;
}

const Network& TwoWindingsTransformerAdder::getNetwork() const {
    return m_substation.getNetwork();
}

Network& TwoWindingsTransformerAdder::getNetwork() {
    return m_substation.getNetwork();
}

const std::string& TwoWindingsTransformerAdder::getTypeDescription() const {
    static std::string s_typeDescription = "2 windings transformer";

    return s_typeDescription;
}

TwoWindingsTransformerAdder& TwoWindingsTransformerAdder::setB(double b) {
    m_b = b;
    return *this;
}

TwoWindingsTransformerAdder& TwoWindingsTransformerAdder::setG(double g) {
    m_g = g;
    return *this;
}

TwoWindingsTransformerAdder& TwoWindingsTransformerAdder::setR(double r) {
    m_r = r;
    return *this;
}

TwoWindingsTransformerAdder& TwoWindingsTransformerAdder::setRatedS(double ratedS) {
    m_ratedS = ratedS;
    return *this;
}

TwoWindingsTransformerAdder& TwoWindingsTransformerAdder::setRatedU1(double ratedU1) {
    m_ratedU1 = ratedU1;
    return *this;
}

TwoWindingsTransformerAdder& TwoWindingsTransformerAdder::setRatedU2(double ratedU2) {
    m_ratedU2 = ratedU2;
    return *this;
}

TwoWindingsTransformerAdder& TwoWindingsTransformerAdder::setX(double x) {
    m_x = x;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
