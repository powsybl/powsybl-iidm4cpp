/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LCCCONVERTERSTATIONADDER_HPP
#define POWSYBL_IIDM_LCCCONVERTERSTATIONADDER_HPP

#include <powsybl/iidm/HvdcConverterStationAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class LccConverterStation;

class LccConverterStationAdder : public HvdcConverterStationAdder<LccConverterStationAdder> {
public:
    ~LccConverterStationAdder() noexcept override = default;

    LccConverterStation& add();

    LccConverterStationAdder& setPowerFactor(double powerFactor);

protected: // HvdcConverterStationAdder
    void validate() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit LccConverterStationAdder(VoltageLevel& voltageLevel);

    friend class VoltageLevel;

private:
    double m_powerFactor = stdcxx::nan();

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LCCCONVERTERSTATIONADDER_HPP
