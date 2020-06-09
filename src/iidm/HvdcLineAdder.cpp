/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/HvdcLineAdder.hpp>

#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

HvdcLineAdder::HvdcLineAdder(Network& network) :
    m_network(network) {
}

HvdcLine& HvdcLineAdder::add() {
    checkR(*this, m_r);
    checkOptional(*this, m_convertersMode, "converter mode is invalid");
    checkConvertersMode(*this, *m_convertersMode);
    checkNominalVoltage(*this, m_nominalVoltage);
    checkActivePowerSetpoint(*this, m_activePowerSetpoint);
    checkMaxP(*this, m_maxP);

    HvdcConverterStation& converterStation1 = getConverterStation(m_converterStationId1, 1U);
    HvdcConverterStation& converterStation2 = getConverterStation(m_converterStationId2, 2U);

    std::unique_ptr<HvdcLine> ptrHvdcLine = stdcxx::make_unique<HvdcLine>(getNetwork(), checkAndGetUniqueId(), getName(), m_r, m_nominalVoltage, m_maxP, *m_convertersMode, m_activePowerSetpoint,
                                                                          converterStation1, converterStation2);
    auto& line = m_network.checkAndAdd<HvdcLine>(std::move(ptrHvdcLine));

    return line;
}

HvdcConverterStation& HvdcLineAdder::getConverterStation(const std::string& converterStationId, unsigned int side) const {
    checkNotEmpty(converterStationId, stdcxx::format("Side %1% converter station not set", side));

    stdcxx::Reference<HvdcConverterStation> converterStation = m_network.find<HvdcConverterStation>(converterStationId);
    if (!converterStation) {
        throw PowsyblException(stdcxx::format("Side %1% converter station %2% not found", side, converterStationId));
    }
    return converterStation.get();
}

const Network& HvdcLineAdder::getNetwork() const {
    return m_network;
}

Network& HvdcLineAdder::getNetwork() {
    return m_network;
}

const std::string& HvdcLineAdder::getTypeDescription() const {
    static std::string s_typeDescription = "hvdcLine";

    return s_typeDescription;
}

HvdcLineAdder& HvdcLineAdder::setActivePowerSetpoint(double activePowerSetpoint) {
    m_activePowerSetpoint = activePowerSetpoint;
    return *this;
}

HvdcLineAdder& HvdcLineAdder::setConvertersMode(const HvdcLine::ConvertersMode& convertersMode) {
    m_convertersMode = convertersMode;
    return *this;
}

HvdcLineAdder& HvdcLineAdder::setConverterStationId1(const std::string& converterStationId1) {
    m_converterStationId1 = converterStationId1;
    return *this;
}

HvdcLineAdder& HvdcLineAdder::setConverterStationId2(const std::string& converterStationId2) {
    m_converterStationId2 = converterStationId2;
    return *this;
}

HvdcLineAdder& HvdcLineAdder::setMaxP(double maxP) {
    m_maxP = maxP;
    return *this;
}

HvdcLineAdder& HvdcLineAdder::setNominalVoltage(double nominalVoltage) {
    m_nominalVoltage = nominalVoltage;
    return *this;
}

HvdcLineAdder& HvdcLineAdder::setR(double r) {
    m_r = r;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
