/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_RATIOTAPCHANGER_HPP
#define POWSYBL_IIDM_RATIOTAPCHANGER_HPP

#include <powsybl/iidm/RatioTapChangerStep.hpp>
#include <powsybl/iidm/TapChanger.hpp>

namespace powsybl {

namespace iidm {

class RatioTapChangerHolder;
class RatioTapChangerStepsReplacer;

class RatioTapChanger : public TapChanger<RatioTapChangerHolder, RatioTapChanger, RatioTapChangerStep, RatioTapChangerStepsReplacer> {
public:
    enum class RegulationMode : unsigned char {
        VOLTAGE,
        REACTIVE_POWER
    };

public: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public: // TapChanger
    void remove() override;

    RatioTapChanger& setRegulating(bool regulating) override;

    RatioTapChanger& setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal) override;

    RatioTapChanger& setLoadTapChangingCapabilities(bool loadTapChangingCapabilities) override;

public:
    RatioTapChanger(RatioTapChangerHolder& parent, long lowTapPosition, const std::vector<RatioTapChangerStep>& steps, const stdcxx::Reference<Terminal>& regulationTerminal,
                    bool loadTapChangingCapabilities, long tapPosition, const stdcxx::optional<long>& solvedTapPosition, bool regulating, const RegulationMode& regulationMode, double regulationValue, double targetDeadband);

    ~RatioTapChanger() noexcept override = default;

    double getTargetV() const;
    double getRegulationValue() const;

    RatioTapChanger& setTargetV(double targetV);
    RatioTapChanger& setRegulationValue(double regulationValue);

    const RegulationMode& getRegulationMode() const;
    RatioTapChanger& setRegulationMode(const RegulationMode& regulationMode);

    RatioTapChangerStepsReplacer stepsReplacer() override;

protected:
    virtual stdcxx::optional<long> getRelativeNeutralPosition() const override;

private:
    RegulationMode m_regulationMode;

    std::vector<double> m_regulationValue;

    friend class RatioTapChangerStepsReplacer;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_RATIOTAPCHANGER_HPP
