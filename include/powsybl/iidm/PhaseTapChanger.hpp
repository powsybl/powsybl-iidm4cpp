/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_PHASETAPCHANGER_HPP
#define POWSYBL_IIDM_PHASETAPCHANGER_HPP

#include <powsybl/iidm/PhaseTapChangerHolder.hpp>
#include <powsybl/iidm/PhaseTapChangerStep.hpp>
#include <powsybl/iidm/TapChanger.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

class TwoWindingsTransformer;

class IIDM_DECLSPEC PhaseTapChanger : public TapChanger<PhaseTapChangerHolder, PhaseTapChanger, PhaseTapChangerStep> {
public:
    enum class RegulationMode : unsigned char {
        CURRENT_LIMITER,
        ACTIVE_POWER_CONTROL,
        FIXED_TAP
    };

public: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public: // TapChanger
    void remove() override;

    PhaseTapChanger& setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal) override;

public:
    PhaseTapChanger(PhaseTapChangerHolder& parent, long lowTapPosition, const std::vector<PhaseTapChangerStep>& steps, const stdcxx::Reference<Terminal>& regulationTerminal,
                    long tapPosition, bool regulating, const RegulationMode& regulationMode, double regulationValue, double targetDeadband);

    ~PhaseTapChanger() noexcept override = default;

    const RegulationMode& getRegulationMode() const;

    double getRegulationValue() const;

    PhaseTapChanger& setRegulationMode(const RegulationMode& regulationMode);

    PhaseTapChanger& setRegulationValue(double regulationValue);

private:
    RegulationMode m_regulationMode;

    std::vector<double> m_regulationValue;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_PHASETAPCHANGER_HPP
