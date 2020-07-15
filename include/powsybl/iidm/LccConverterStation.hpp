/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LCCCONVERTERSTATION_HPP
#define POWSYBL_IIDM_LCCCONVERTERSTATION_HPP

#include <powsybl/iidm/HvdcConverterStation.hpp>

namespace powsybl {

namespace iidm {

class LccConverterStation : public HvdcConverterStation {
public:  // Identifiable
    LccConverterStation& setFictitious(bool fictitious) override;

public: // HvdcConverterStation
    HvdcConverterStation::HvdcType getHvdcType() const override;

    LccConverterStation& setLossFactor(double lossFactor) override;

public:
    LccConverterStation(const std::string& id, const std::string& name, bool fictitious, double lossFactor, double powerFactor);

    ~LccConverterStation() noexcept override = default;

    double getPowerFactor() const;

    LccConverterStation& setPowerFactor(double powerFactor);

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    double m_powerFactor;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LCCCONVERTERSTATION_HPP
