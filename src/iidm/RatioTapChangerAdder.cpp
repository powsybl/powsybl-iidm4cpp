/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/RatioTapChanger.hpp>

#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/RatioTapChangerHolder.hpp>
#include <powsybl/iidm/RatioTapChangerStep.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

RatioTapChangerAdder::RatioTapChangerAdder(RatioTapChangerHolder& parent) :
    TapChangerAdder(parent) {
}

RatioTapChangerAdder::RatioTapChangerAdder(RatioTapChangerHolder& parent, const RatioTapChanger& ratioTapChanger) :
    TapChangerAdder(parent) {

        //Init from ratioTapChanger:
        setRegulationMode(ratioTapChanger.getRegulationMode());
        setRegulationTerminal(stdcxx::ref(ratioTapChanger.getRegulationTerminal()));
        setRegulating(ratioTapChanger.isRegulating());
        setRegulationValue(ratioTapChanger.getRegulationValue());
        setLoadTapChangingCapabilities(ratioTapChanger.hasLoadTapChangingCapabilities());
        setLowTapPosition(ratioTapChanger.getLowTapPosition());
        setTapPosition(ratioTapChanger.getTapPosition());
        setTargetDeadband(ratioTapChanger.getTargetDeadband());
        for(const auto& step : ratioTapChanger.getAllSteps() ){
            beginStep()
                .setRho(step.second.get().getRho())
                .setB(step.second.get().getB())
                .setG(step.second.get().getG())
                .setX(step.second.get().getX())
                .setR(step.second.get().getR())
                .endStep();
        }


}

RatioTapChanger& RatioTapChangerAdder::add() {
    logging::Logger& logger = logging::LoggerFactory::getLogger<RatioTapChangerAdder>();
    Network& network = getNetwork();
    network.setValidationLevelIfGreaterThan(checkOptional(m_parent, m_tapPosition, "tap position is not set", network.getMinimumValidationLevel()));
    if (m_steps.empty()) {
        throw ValidationException(m_parent, "ratio tap changer should have at least one step");
    }
    long highTapPosition = m_lowTapPosition + m_steps.size() - 1;
    if(!m_tapPosition) {
        m_tapPosition = 0L;
    }
    network.setValidationLevelIfGreaterThan(checkTapPosition(m_parent, *m_tapPosition, m_lowTapPosition, highTapPosition, network.getMinimumValidationLevel()));

    network.setValidationLevelIfGreaterThan(checkRatioTapChangerRegulation(m_parent, m_regulating, m_loadTapChangingCapabilities, m_regulationTerminal, m_regulationMode, m_regulationValue, network, network.getMinimumValidationLevel()));
    network.setValidationLevelIfGreaterThan(checkTargetDeadband(m_parent, "ratio tap changer", m_regulating, m_targetDeadband, network.getMinimumValidationLevel()));

    std::unique_ptr<RatioTapChanger> ptrRatioTapChanger = stdcxx::make_unique<RatioTapChanger>(m_parent, m_lowTapPosition, m_steps, m_regulationTerminal,
                                                                                               m_loadTapChangingCapabilities, *m_tapPosition, m_regulating, m_regulationMode, m_regulationValue, m_targetDeadband);

    bool wasRegulating = m_parent.hasRatioTapChanger() && m_parent.getRatioTapChanger().isRegulating();
    unsigned long count = m_parent.getRegulatingTapChangerCount() - (wasRegulating ? 1 : 0);
    network.setValidationLevelIfGreaterThan(checkOnlyOneTapChangerRegulatingEnabled(m_parent, count, m_regulating, network.getMinimumValidationLevel()));

    if (m_parent.hasPhaseTapChanger()) {
        logger.warn("%1% both Ratio and Phase Tap Changer are defined", m_parent.getMessageHeader());
    }

    m_parent.setRatioTapChanger(std::move(ptrRatioTapChanger));

    return m_parent.getRatioTapChanger();
}

RatioTapChangerAdder& RatioTapChangerAdder::addStep(double rho, double r, double x, double g, double b) {
    m_steps.emplace_back(rho, r, x, g, b);
    return *this;
}

RatioTapChangerStepAdder<RatioTapChangerAdder> RatioTapChangerAdder::beginStep() {
    return RatioTapChangerStepAdder<RatioTapChangerAdder>(*this);
}

Validable& RatioTapChangerAdder::getValidable() {
    return m_parent;
}

RatioTapChangerAdder& RatioTapChangerAdder::setLoadTapChangingCapabilities(bool loadTapChangingCapabilities) {
    m_loadTapChangingCapabilities = loadTapChangingCapabilities;
    return *this;
}

RatioTapChangerAdder& RatioTapChangerAdder::setRegulationMode(const RatioTapChanger::RegulationMode& regulationMode) {
    m_regulationMode = regulationMode;
    return *this;
}

RatioTapChangerAdder& RatioTapChangerAdder::setTargetV(double targetV) {
    if(!std::isnan(targetV)){
        m_regulationMode = RatioTapChanger::RegulationMode::VOLTAGE;
    }
    m_regulationValue = targetV;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
