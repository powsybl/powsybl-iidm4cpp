/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_HVDCLINEADDER_HPP
#define POWSYBL_IIDM_HVDCLINEADDER_HPP

#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/IdentifiableAdder.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class HvdcConverterStation;
class Network;

class HvdcLineAdder : public IdentifiableAdder<HvdcLineAdder> {
public:
    ~HvdcLineAdder() noexcept override = default;

    HvdcLine& add();

    HvdcLineAdder& setActivePowerSetpoint(double activePowerSetpoint);

    HvdcLineAdder& setConvertersMode(const HvdcLine::ConvertersMode& convertersMode);

    HvdcLineAdder& setConverterStationId1(const std::string& converterStationId1);

    HvdcLineAdder& setConverterStationId2(const std::string& converterStationId2);

    HvdcLineAdder& setMaxP(double maxP);

    HvdcLineAdder& setNominalV(double nominalV);

    HvdcLineAdder& setR(double r);

protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit HvdcLineAdder(Network& network);

    HvdcConverterStation& getConverterStation(const std::string& converterStationId, unsigned int side) const;

    friend class Network;

private:
    Network& m_network;

    std::string m_converterStationId1;

    std::string m_converterStationId2;

    double m_r = stdcxx::nan();

    double m_nominalV = stdcxx::nan();

    double m_maxP = stdcxx::nan();

    stdcxx::optional<HvdcLine::ConvertersMode> m_convertersMode;

    double m_activePowerSetpoint = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_HVDCLINEADDER_HPP
