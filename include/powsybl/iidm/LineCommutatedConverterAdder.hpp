/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LINECOMMUTATEDCONVERTERADDER_HPP
#define POWSYBL_IIDM_LINECOMMUTATEDCONVERTERADDER_HPP

#include <powsybl/iidm/AcDcConverterAdder.hpp>
#include <powsybl/iidm/LineCommutatedConverter.hpp>

#include <powsybl/stdcxx/math.hpp>


namespace powsybl {

namespace iidm {

class LineCommutatedConverterAdder : public AcDcConverterAdder<LineCommutatedConverter, LineCommutatedConverterAdder> {

public:
    explicit LineCommutatedConverterAdder(VoltageLevel& voltageLevel);
    ~LineCommutatedConverterAdder() noexcept override = default;

    LineCommutatedConverter& add() override;

    LineCommutatedConverterAdder& setReactiveModel(const LineCommutatedConverter::ReactiveModel& reactiveModel);

    LineCommutatedConverterAdder& setPowerFactor(double powerFactor);

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    LineCommutatedConverter::ReactiveModel m_reactiveModel = LineCommutatedConverter::ReactiveModel::FIXED_POWER_FACTOR;
    double m_powerFactor = cos(atan(0.5));
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LINECOMMUTATEDCONVERTERADDER_HPP
