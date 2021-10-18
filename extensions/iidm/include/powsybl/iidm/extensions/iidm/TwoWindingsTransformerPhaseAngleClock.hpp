/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERPHASEANGLECLOCK_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERPHASEANGLECLOCK_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class TwoWindingsTransformer;

namespace extensions {

namespace iidm {

class TwoWindingsTransformerPhaseAngleClock : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    unsigned long getPhaseAngleClock() const;

    TwoWindingsTransformerPhaseAngleClock& setPhaseAngleClock(unsigned long phaseAngleClock);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    TwoWindingsTransformerPhaseAngleClock(TwoWindingsTransformer& transformer, unsigned long phaseAngleClock);

    unsigned long checkPhaseAngleClock(unsigned long phaseAngleClock) const;

    friend class TwoWindingsTransformerPhaseAngleClockAdder;

private:
    unsigned long m_phaseAngleClock;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERPHASEANGLECLOCK_HPP
