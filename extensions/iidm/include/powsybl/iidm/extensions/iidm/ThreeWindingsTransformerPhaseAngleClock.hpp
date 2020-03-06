/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERPHASEANGLECLOCK_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERPHASEANGLECLOCK_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class ThreeWindingsTransformer;

namespace extensions {

namespace iidm {

class ThreeWindingsTransformerPhaseAngleClock : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    ThreeWindingsTransformerPhaseAngleClock(ThreeWindingsTransformer& battery, unsigned long phaseAngleClockLeg2, unsigned long phaseAngleClockLeg3);

    unsigned long getPhaseAngleClockLeg2() const;

    unsigned long getPhaseAngleClockLeg3() const;

    ThreeWindingsTransformerPhaseAngleClock& setPhaseAngleClockLeg2(unsigned long phaseAngleClockLeg2);

    ThreeWindingsTransformerPhaseAngleClock& setPhaseAngleClockLeg3(unsigned long phaseAngleClockLeg3);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

    unsigned long checkPhaseAngleClock(unsigned long phaseAngleClock) const;

private:
    unsigned long m_phaseAngleClockLeg2;

    unsigned long m_phaseAngleClockLeg3;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERPHASEANGLECLOCK_HPP
