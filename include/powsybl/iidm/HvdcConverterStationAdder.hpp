/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_HVDCCONVERTERSTATIONADDER_HPP
#define POWSYBL_IIDM_HVDCCONVERTERSTATIONADDER_HPP

#include <powsybl/iidm/InjectionAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class Network;
class VoltageLevel;

template<typename Adder>
class HvdcConverterStationAdder : public InjectionAdder<Adder> {
public:
    ~HvdcConverterStationAdder() noexcept override = default;

    Adder& setLossFactor(double lossFactor);

protected: // IdentifiableAdder
    Network& getNetwork() override;

protected:
    explicit HvdcConverterStationAdder(VoltageLevel& voltageLevel);

    double getLossFactor() const;

    VoltageLevel& getVoltageLevel() const;

    virtual void validate() const;

private:
    VoltageLevel& m_voltageLevel;

    double m_lossFactor = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/HvdcConverterStationAdder.hxx>

#endif  // POWSYBL_IIDM_HVDCCONVERTERSTATIONADDER_HPP
