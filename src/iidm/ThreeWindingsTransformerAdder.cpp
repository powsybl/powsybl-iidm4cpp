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
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

ThreeWindingsTransformerAdder::ThreeWindingsTransformerAdder(Network& network) :
    m_network(network) {
}

ThreeWindingsTransformerAdder::ThreeWindingsTransformerAdder(Substation& substation) :
    m_network(substation.getNetwork()),
    m_substation(substation) {
}

ThreeWindingsTransformer& ThreeWindingsTransformerAdder::add() {
    logging::Logger& logger = logging::LoggerFactory::getLogger<ThreeWindingsTransformer>();

    if (!m_adder1) {
        throw ValidationException(*this, "leg 1 is not defined");
    }
    ThreeWindingsTransformer::Leg leg1 = m_adder1->build();
    VoltageLevel& voltageLevel1 = m_adder1->checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal1 = m_adder1->checkAndGetTerminal(voltageLevel1);

    if (!m_adder2) {
        throw ValidationException(*this, "leg 2 is not defined");
    }
    ThreeWindingsTransformer::Leg leg2 = m_adder2->build();
    VoltageLevel& voltageLevel2 = m_adder2->checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal2 = m_adder2->checkAndGetTerminal(voltageLevel2);

    if (!m_adder3) {
        throw ValidationException(*this, "leg 3 is not defined");
    }
    ThreeWindingsTransformer::Leg leg3 = m_adder3->build();
    VoltageLevel& voltageLevel3 = m_adder3->checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal3 = m_adder3->checkAndGetTerminal(voltageLevel3);

    if (m_substation) {
        if (!voltageLevel1.getSubstation() || voltageLevel1.getSubstation() != m_substation ||
            !voltageLevel2.getSubstation() || voltageLevel2.getSubstation() != m_substation ||
            !voltageLevel3.getSubstation() || voltageLevel3.getSubstation() != m_substation) {
            const std::string& substationId1 = voltageLevel1.getSubstation() ? voltageLevel1.getSubstation().get().getId() : "null";
            const std::string& substationId2 = voltageLevel2.getSubstation() ? voltageLevel2.getSubstation().get().getId() : "null";
            const std::string& substationId3 = voltageLevel3.getSubstation() ? voltageLevel3.getSubstation().get().getId() : "null";

            throw ValidationException(*this, stdcxx::format("the 3 windings of the transformer shall belong to the substation '%1%' ('%2%', '%3%', '%4%')",
                m_substation.get().getId(), substationId1, substationId2, substationId3));
        }
    } else if (voltageLevel1.getSubstation() || voltageLevel2.getSubstation() || voltageLevel3.getSubstation()) {
        throw ValidationException(*this, stdcxx::format("the 3 windings of the transformer shall belong to a substation since there are located in voltage levels with substations ('%1%', '%2%', '%3%')",
            voltageLevel1.getId(), voltageLevel2.getId(), voltageLevel3.getId()));
    }

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

    Terminal& terminal1 = transformer.addTerminal(std::move(ptrTerminal1));
    Terminal& terminal2 = transformer.addTerminal(std::move(ptrTerminal2));
    Terminal& terminal3 = transformer.addTerminal(std::move(ptrTerminal3));

    voltageLevel1.attach(terminal1, false);
    voltageLevel2.attach(terminal2, false);
    voltageLevel3.attach(terminal3, false);

    return transformer;
}

const Network& ThreeWindingsTransformerAdder::getNetwork() const {
    return m_network;
}

Network& ThreeWindingsTransformerAdder::getNetwork() {
    return m_network;
}

stdcxx::Reference<Substation> ThreeWindingsTransformerAdder::getSubstation() {
    return m_substation;
}

const std::string& ThreeWindingsTransformerAdder::getTypeDescription() const {
    static std::string s_typeDescription = "3 windings transformer";

    return s_typeDescription;
}

ThreeWindingsTransformerAdder::LegAdder ThreeWindingsTransformerAdder::newLeg1() {
    return LegAdder(*this, 1);
}

ThreeWindingsTransformerAdder::LegAdder ThreeWindingsTransformerAdder::newLeg2() {
    return LegAdder(*this, 2).setB(0).setG(0);
}

ThreeWindingsTransformerAdder::LegAdder ThreeWindingsTransformerAdder::newLeg3() {
    return LegAdder(*this, 3).setB(0).setG(0);
}

void ThreeWindingsTransformerAdder::setLegAdder1(const LegAdder& legAdder) {
    m_adder1.emplace(legAdder);
}

void ThreeWindingsTransformerAdder::setLegAdder2(const LegAdder& legAdder) {
    m_adder2.emplace(legAdder);
}

void ThreeWindingsTransformerAdder::setLegAdder3(const LegAdder& legAdder) {
    m_adder3.emplace(legAdder);
}

ThreeWindingsTransformerAdder& ThreeWindingsTransformerAdder::setRatedU0(double ratedU0) {
    m_ratedU0 = ratedU0;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
