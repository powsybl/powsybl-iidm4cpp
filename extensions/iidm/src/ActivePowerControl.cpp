/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ActivePowerControl.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include <powsybl/stdcxx/format.hpp>

#include <limits>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ActivePowerControl::PLimits ActivePowerControl::getPLimits(const Injection& extendedComponent) const {
    double minP = -std::numeric_limits<double>::max();
    double maxP = std::numeric_limits<double>::max();
    if (stdcxx::isInstanceOf<Battery>(extendedComponent)) {
        const auto& battery = dynamic_cast<const Battery&>(extendedComponent);
        minP = battery.getMinP();
        maxP = battery.getMaxP();
    } else if(stdcxx::isInstanceOf<Generator>(extendedComponent)) {
        const auto& generator = dynamic_cast<const Generator&>(extendedComponent);
        minP = generator.getMinP();
        maxP = generator.getMaxP();
    }
    return ActivePowerControl::PLimits{minP, maxP};
}

double ActivePowerControl::checkWithinPMinMax(double value, const Injection& extendedComponent) const {
    PLimits pLimits = getPLimits(extendedComponent);

    if(!std::isnan(value) && (value < pLimits.m_minP || value > pLimits.m_maxP)) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<ActivePowerControl>();
        logger.warn("targetP limit is now outside of pMin,pMax for component %1%. Returning closest value in [pmin,pMax].",
                        extendedComponent.getId());
        return value < pLimits.m_minP ? pLimits.m_minP : pLimits.m_maxP;
    }
    return value;
}

double ActivePowerControl::checkTargetPLimit(double targetPLimit, const std::string& name, const Injection& extendedComponent) const {
    PLimits pLimits = getPLimits(extendedComponent);
    if (!std::isnan(targetPLimit) && (targetPLimit < pLimits.m_minP || targetPLimit > pLimits.m_maxP)) {
        throw PowsyblException(stdcxx::format("%1% value (%2%) is not between minP and maxP for component %3%",
            name,
            targetPLimit,
            extendedComponent.getId()));
    }
    return targetPLimit;
}
void ActivePowerControl::checkLimitOrder(double minTargetP, double maxTargetP) const {
    if (!std::isnan(minTargetP) && !std::isnan(maxTargetP) && minTargetP > maxTargetP) {
        throw PowsyblException(stdcxx::format("invalid targetP limits [%1%, %2%]", minTargetP, maxTargetP));
    }
}


ActivePowerControl::ActivePowerControl(Battery& battery, bool participate, double droop, double participationFactor, double minTargetP, double maxTargetP) :
    AbstractMultiVariantIdentifiableExtension(battery) {
    unsigned long variantArraySize = getVariantManagerHolder().getVariantManager().getVariantArraySize();
    m_participate.resize(variantArraySize, participate);
    m_droop.resize(variantArraySize, droop);
    m_participationFactor.resize(variantArraySize, participationFactor);
    m_minTargetP.resize(variantArraySize, checkTargetPLimit(minTargetP, "minTargetP", battery));
    m_maxTargetP.resize(variantArraySize, checkTargetPLimit(maxTargetP, "maxTargetP", battery));
    checkLimitOrder(minTargetP, maxTargetP);
}

ActivePowerControl::ActivePowerControl(Generator& generator, bool participate, double droop, double participationFactor, double minTargetP, double maxTargetP) :
    AbstractMultiVariantIdentifiableExtension(generator) {
    unsigned long variantArraySize = getVariantManagerHolder().getVariantManager().getVariantArraySize();
    m_participate.resize(variantArraySize, participate);
    m_droop.resize(variantArraySize, droop);
    m_participationFactor.resize(variantArraySize, participationFactor);
    m_minTargetP.resize(variantArraySize, checkTargetPLimit(minTargetP, "minTargetP", generator));
    m_maxTargetP.resize(variantArraySize, checkTargetPLimit(maxTargetP, "maxTargetP", generator));
    checkLimitOrder(minTargetP, maxTargetP);
}

void ActivePowerControl::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Battery>(extendable.get()) && !stdcxx::isInstanceOf<Generator>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% or %3% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Battery>(), stdcxx::demangle<Generator>()));
    }
}

double ActivePowerControl::getDroop() const {
    return m_droop[getVariantIndex()];
}

double ActivePowerControl::getParticipationFactor() const {
    return m_participationFactor[getVariantIndex()];
}

double ActivePowerControl::getMinTargetP() const {
    return checkWithinPMinMax(m_minTargetP[getVariantIndex()], getExtendable<Injection>().get());
}

double ActivePowerControl::getMaxTargetP() const {
    return checkWithinPMinMax(m_maxTargetP[getVariantIndex()], getExtendable<Injection>().get());
}

const std::string& ActivePowerControl::getName() const {
    static std::string s_name = "activePowerControl";
    return s_name;
}

const std::type_index& ActivePowerControl::getType() const {
    static std::type_index s_type = typeid(ActivePowerControl);
    return s_type;
}

bool ActivePowerControl::isParticipate() const {
    return m_participate[getVariantIndex()];
}

ActivePowerControl& ActivePowerControl::setDroop(double droop) {
    m_droop[getVariantIndex()] = droop;
    return *this;
}

ActivePowerControl& ActivePowerControl::setParticipationFactor(double participationFactor) {
    m_participationFactor[getVariantIndex()] = participationFactor;
    return *this;
}

ActivePowerControl& ActivePowerControl::setParticipate(bool participate) {
    m_participate[getVariantIndex()] = participate;
    return *this;
}

ActivePowerControl& ActivePowerControl::setMinTargetP(double minTargetP) {
    checkLimitOrder(minTargetP, m_maxTargetP[getVariantIndex()]);
    m_minTargetP[getVariantIndex()] = checkTargetPLimit(minTargetP, "minTargetP", getExtendable<Injection>().get());
    return *this;
}

ActivePowerControl& ActivePowerControl::setMaxTargetP(double maxTargetP) {
    checkLimitOrder(m_minTargetP[getVariantIndex()], maxTargetP);
    m_maxTargetP[getVariantIndex()] = checkTargetPLimit(maxTargetP, "maxTargetP", getExtendable<Injection>().get());
    return *this;
}

void ActivePowerControl::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        m_droop[index] = m_droop[sourceIndex];
        m_participate[index] = m_participate[sourceIndex];
        m_participationFactor[index] = m_participationFactor[sourceIndex];
        m_minTargetP[index] = m_minTargetP[sourceIndex];
        m_maxTargetP[index] = m_maxTargetP[sourceIndex];
    }
}

void ActivePowerControl::deleteVariantArrayElement(unsigned long /*index*/) {
    //nothing to do
}

void ActivePowerControl::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_droop.resize(m_droop.size() + number, m_droop[sourceIndex]);
    m_participate.resize(m_participate.size() + number, m_participate[sourceIndex]);
    m_participationFactor.resize(m_participationFactor.size() + number, m_participationFactor[sourceIndex]);
    m_minTargetP.resize(m_minTargetP.size() + number, m_minTargetP[sourceIndex]);
    m_maxTargetP.resize(m_maxTargetP.size() + number, m_maxTargetP[sourceIndex]);
}

void ActivePowerControl::reduceVariantArraySize(unsigned long number) {
    m_droop.resize(m_droop.size() - number);
    m_participate.resize(m_participate.size() - number);
    m_participationFactor.resize(m_participationFactor.size() - number);
    m_minTargetP.resize(m_minTargetP.size() - number);
    m_maxTargetP.resize(m_maxTargetP.size() - number);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
