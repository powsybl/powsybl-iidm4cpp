/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

ThreeWindingsTransformerAdder::ThreeWindingsTransformerAdder(Substation& substation) :
    m_substation(substation) {
}

ThreeWindingsTransformer& ThreeWindingsTransformerAdder::add() {
    logging::Logger& logger = logging::LoggerFactory::getLogger<ThreeWindingsTransformer>();

    if (!m_adder1.is_initialized()) {
        throw ValidationException(newLeg1(), "leg 1 is not defined");
    }
    ThreeWindingsTransformer::Leg leg1 = m_adder1->build();
    VoltageLevel& voltageLevel1 = m_adder1->checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal1 = m_adder1->checkAndGetTerminal(voltageLevel1);

    if (!m_adder2.is_initialized()) {
        throw ValidationException(newLeg2(), "leg 2 is not defined");
    }
    ThreeWindingsTransformer::Leg leg2 = m_adder2->build();
    VoltageLevel& voltageLevel2 = m_adder2->checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal2 = m_adder2->checkAndGetTerminal(voltageLevel2);

    if (!m_adder3.is_initialized()) {
        throw ValidationException(newLeg3(), "leg 3 is not defined");
    }
    ThreeWindingsTransformer::Leg leg3 = m_adder3->build();
    VoltageLevel& voltageLevel3 = m_adder3->checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal3 = m_adder3->checkAndGetTerminal(voltageLevel3);

    // check that the 3 windings transformer is attachable on the 3 sides
    voltageLevel1.attach(*ptrTerminal1, true);
    voltageLevel2.attach(*ptrTerminal2, true);
    voltageLevel3.attach(*ptrTerminal3, true);

    // Define ratedU0 equal to ratedU1 if it has not been defined
    if (std::isnan(m_ratedU0)) {
        m_ratedU0 = leg1.getRatedU();
        logger.info(stdcxx::format("RatedU0 is not set. Fixed to leg1 ratedU: %1%", leg1.getRatedU()));
    }

    std::unique_ptr<ThreeWindingsTransformer> ptrTransformer = stdcxx::make_unique<ThreeWindingsTransformer>(checkAndGetUniqueId(), getName(), isFictitious(), std::move(leg1), std::move(leg2), std::move(leg3), m_ratedU0);
    auto& transformer = getNetwork().checkAndAdd<ThreeWindingsTransformer>(std::move(ptrTransformer));
    transformer.getLeg1().setTransformer(transformer);
    transformer.getLeg2().setTransformer(transformer);
    transformer.getLeg3().setTransformer(transformer);

    Terminal& terminal1 = transformer.addTerminal(std::move(ptrTerminal1));
    Terminal& terminal2 = transformer.addTerminal(std::move(ptrTerminal2));
    Terminal& terminal3 = transformer.addTerminal(std::move(ptrTerminal3));

    voltageLevel1.attach(terminal1, false);
    voltageLevel2.attach(terminal2, false);
    voltageLevel3.attach(terminal3, false);

    return transformer;
}

const Network& ThreeWindingsTransformerAdder::getNetwork() const {
    return m_substation.getNetwork();
}

Network& ThreeWindingsTransformerAdder::getNetwork() {
    return m_substation.getNetwork();
}

Substation& ThreeWindingsTransformerAdder::getSubstation() {
    return m_substation;
}

const std::string& ThreeWindingsTransformerAdder::getTypeDescription() const {
    static std::string s_typeDescription = "3 windings transformer";

    return s_typeDescription;
}

ThreeWindingsTransformerAdder::LegAdder ThreeWindingsTransformerAdder::newLeg1() {
    return ThreeWindingsTransformerAdder::LegAdder(*this, 1);
}

ThreeWindingsTransformerAdder::LegAdder ThreeWindingsTransformerAdder::newLeg2() {
    return ThreeWindingsTransformerAdder::LegAdder(*this, 2).setB(0).setG(0);
}

ThreeWindingsTransformerAdder::LegAdder ThreeWindingsTransformerAdder::newLeg3() {
    return ThreeWindingsTransformerAdder::LegAdder(*this, 3).setB(0).setG(0);
}

void ThreeWindingsTransformerAdder::setLegAdder1(LegAdder& legAdder) {
    m_adder1.emplace(legAdder);
}

void ThreeWindingsTransformerAdder::setLegAdder2(LegAdder& legAdder) {
    m_adder2.emplace(legAdder);
}

void ThreeWindingsTransformerAdder::setLegAdder3(LegAdder& legAdder) {
    m_adder3.emplace(legAdder);
}

ThreeWindingsTransformerAdder& ThreeWindingsTransformerAdder::setRatedU0(double ratedU0) {
    m_ratedU0 = ratedU0;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
