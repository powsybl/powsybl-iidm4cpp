/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LINECOMMUTATEDCONVERTER_HPP
#define POWSYBL_IIDM_LINECOMMUTATEDCONVERTER_HPP

#include <powsybl/iidm/AcDcConverter.hpp>

namespace powsybl {

namespace iidm {

class LineCommutatedConverter : public AcDcConverter {

public:
    enum class ReactiveModel : unsigned char {
        FIXED_POWER_FACTOR,
        CALCULATED_POWER_FACTOR
    };

public:
    ~LineCommutatedConverter() noexcept override = default;

public:  // Identifiable
    const IdentifiableType& getType() const override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

public: // AcDcConverter setter for LineCommutatedConverter chaining
    LineCommutatedConverter& setIdleLoss(double idleLoss);
    LineCommutatedConverter& setSwitchingLoss(double switchingLoss);
    LineCommutatedConverter& setResistiveLoss(double resistiveLoss);
    LineCommutatedConverter& setPccTerminal(const stdcxx::Reference<Terminal>& pccTerminal);
    LineCommutatedConverter& setControlMode(const ControlMode& controlMode);
    LineCommutatedConverter& setTargetP(double targetP);
    LineCommutatedConverter& setTargetVdc(double targetVdc);

public:
    const ReactiveModel& getReactiveModel() const;
    LineCommutatedConverter& setReactiveModel(const ReactiveModel& reactiveModel);

    double getPowerFactor() const;
    LineCommutatedConverter& setPowerFactor(double powerFactor);

protected:
    LineCommutatedConverter(const std::string& id, const std::string& name, bool fictitious, unsigned long variantArraySize, 
                    double idleLoss, double switchingLoss, double resistiveLoss, 
                    const stdcxx::Reference<Terminal>& pccTerminal, const ControlMode& controlMode, double targetP, double targetVdc, 
                    const ReactiveModel& reactiveModel, double powerFactor);
    friend class LineCommutatedConverterAdder;

private:
    ReactiveModel m_reactiveModel;
    double m_powerFactor;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LINECOMMUTATEDCONVERTER_HPP
