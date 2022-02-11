/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERTOBEESTIMATED_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERTOBEESTIMATED_HPP

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ThreeWindingsTransformerToBeEstimated : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    bool shouldEstimatePhaseTapChanger(const ThreeWindingsTransformer::Side& side) const;

    ThreeWindingsTransformerToBeEstimated& shouldEstimatePhaseTapChanger(bool toBeEstimated, const ThreeWindingsTransformer::Side& side);

    bool shouldEstimatePhaseTapChanger1() const;

    ThreeWindingsTransformerToBeEstimated& shouldEstimatePhaseTapChanger1(bool toBeEstimated);

    bool shouldEstimatePhaseTapChanger2() const;

    ThreeWindingsTransformerToBeEstimated& shouldEstimatePhaseTapChanger2(bool toBeEstimated);

    bool shouldEstimatePhaseTapChanger3() const;

    ThreeWindingsTransformerToBeEstimated& shouldEstimatePhaseTapChanger3(bool toBeEstimated);

    bool shouldEstimateRatioTapChanger(const ThreeWindingsTransformer::Side& side) const;

    ThreeWindingsTransformerToBeEstimated& shouldEstimateRatioTapChanger(bool toBeEstimated, const ThreeWindingsTransformer::Side& side);

    bool shouldEstimateRatioTapChanger1() const;

    ThreeWindingsTransformerToBeEstimated& shouldEstimateRatioTapChanger1(bool toBeEstimated);

    bool shouldEstimateRatioTapChanger2() const;

    ThreeWindingsTransformerToBeEstimated& shouldEstimateRatioTapChanger2(bool toBeEstimated);

    bool shouldEstimateRatioTapChanger3() const;

    ThreeWindingsTransformerToBeEstimated& shouldEstimateRatioTapChanger3(bool toBeEstimated);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    ThreeWindingsTransformerToBeEstimated(ThreeWindingsTransformer& transformer, bool rtc1Status, bool rtc2Status, bool rtc3Status, bool ptc1Status,
                                          bool ptc2Status, bool ptc3Status);

    friend class ThreeWindingsTransformerToBeEstimatedAdder;

private:
    bool m_ptc1Status;

    bool m_ptc2Status;

    bool m_ptc3Status;

    bool m_rtc1Status;

    bool m_rtc2Status;

    bool m_rtc3Status;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERTOBEESTIMATED_HPP
