/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERTOBEESTIMATED_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERTOBEESTIMATED_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class TwoWindingsTransformer;

namespace extensions {

namespace iidm {

class TwoWindingsTransformerToBeEstimated : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    bool shouldEstimatePhaseTapChanger() const;

    TwoWindingsTransformerToBeEstimated& shouldEstimatePhaseTapChanger(bool toBeEstimated);

    bool shouldEstimateRatioTapChanger() const;

    TwoWindingsTransformerToBeEstimated& shouldEstimateRatioTapChanger(bool toBeEstimated);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    TwoWindingsTransformerToBeEstimated(TwoWindingsTransformer& transformer, bool rtcStatus, bool ptcStatus);

    friend class TwoWindingsTransformerToBeEstimatedAdder;

private:
    bool m_ptcStatus;

    bool m_rtcStatus;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERTOBEESTIMATED_HPP
