/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerToBeEstimated.hpp>

#include <powsybl/iidm/Enum.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ThreeWindingsTransformerToBeEstimated::ThreeWindingsTransformerToBeEstimated(ThreeWindingsTransformer& transformer, bool rtc1Status, bool rtc2Status, bool rtc3Status, bool ptc1Status,
                                      bool ptc2Status, bool ptc3Status) :
    Extension(transformer),
    m_ptc1Status(ptc1Status),
    m_ptc2Status(ptc2Status),
    m_ptc3Status(ptc3Status),
    m_rtc1Status(rtc1Status),
    m_rtc2Status(rtc2Status),
    m_rtc3Status(rtc3Status) {
}

void ThreeWindingsTransformerToBeEstimated::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<ThreeWindingsTransformer>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<ThreeWindingsTransformer>()));
    }
}

const std::string& ThreeWindingsTransformerToBeEstimated::getName() const {
    static std::string s_name = "threeWindingsTransformerToBeEstimated";
    return s_name;
}

const std::type_index& ThreeWindingsTransformerToBeEstimated::getType() const {
    static std::type_index s_type = typeid(ThreeWindingsTransformerToBeEstimated);
    return s_type;
}

bool ThreeWindingsTransformerToBeEstimated::shouldEstimatePhaseTapChanger(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_ptc1Status;

        case ThreeWindingsTransformer::Side::TWO:
            return m_ptc2Status;

        case ThreeWindingsTransformer::Side::THREE:
            return m_ptc3Status;

        default:
            throw AssertionError(stdcxx::format("Unexpected side: %1%", side));
    }
}

ThreeWindingsTransformerToBeEstimated& ThreeWindingsTransformerToBeEstimated::shouldEstimatePhaseTapChanger(bool toBeEstimated, const ThreeWindingsTransformer::Side& side) {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            m_ptc1Status = toBeEstimated;
            break;

        case ThreeWindingsTransformer::Side::TWO:
            m_ptc2Status = toBeEstimated;
            break;

        case ThreeWindingsTransformer::Side::THREE:
            m_ptc3Status = toBeEstimated;
            break;

        default:
            throw AssertionError(stdcxx::format("Unexpected side: %1%", side));
    }

    return *this;
}

bool ThreeWindingsTransformerToBeEstimated::shouldEstimatePhaseTapChanger1() const {
    return m_ptc1Status;
}

ThreeWindingsTransformerToBeEstimated& ThreeWindingsTransformerToBeEstimated::shouldEstimatePhaseTapChanger1(bool toBeEstimated) {
    m_ptc1Status = toBeEstimated;

    return *this;
}

bool ThreeWindingsTransformerToBeEstimated::shouldEstimatePhaseTapChanger2() const {
    return m_ptc2Status;
}

ThreeWindingsTransformerToBeEstimated& ThreeWindingsTransformerToBeEstimated::shouldEstimatePhaseTapChanger2(bool toBeEstimated) {
    m_ptc2Status = toBeEstimated;

    return *this;
}

bool ThreeWindingsTransformerToBeEstimated::shouldEstimatePhaseTapChanger3() const {
    return m_ptc3Status;
}

ThreeWindingsTransformerToBeEstimated& ThreeWindingsTransformerToBeEstimated::shouldEstimatePhaseTapChanger3(bool toBeEstimated) {
    m_ptc3Status = toBeEstimated;

    return *this;
}

bool ThreeWindingsTransformerToBeEstimated::shouldEstimateRatioTapChanger(const ThreeWindingsTransformer::Side& side) const {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            return m_rtc1Status;

        case ThreeWindingsTransformer::Side::TWO:
            return m_rtc2Status;

        case ThreeWindingsTransformer::Side::THREE:
            return m_rtc3Status;

        default:
            throw AssertionError(stdcxx::format("Unexpected side: %1%", side));
    }
}

ThreeWindingsTransformerToBeEstimated& ThreeWindingsTransformerToBeEstimated::shouldEstimateRatioTapChanger(bool toBeEstimated, const ThreeWindingsTransformer::Side& side) {
    switch (side) {
        case ThreeWindingsTransformer::Side::ONE:
            m_rtc1Status = toBeEstimated;
            break;

        case ThreeWindingsTransformer::Side::TWO:
            m_rtc2Status = toBeEstimated;
            break;

        case ThreeWindingsTransformer::Side::THREE:
            m_rtc3Status = toBeEstimated;
            break;

        default:
            throw AssertionError(stdcxx::format("Unexpected side: %1%", side));
    }

    return *this;
}

bool ThreeWindingsTransformerToBeEstimated::shouldEstimateRatioTapChanger1() const {
    return m_rtc1Status;
}

ThreeWindingsTransformerToBeEstimated& ThreeWindingsTransformerToBeEstimated::shouldEstimateRatioTapChanger1(bool toBeEstimated) {
    m_rtc1Status = toBeEstimated;

    return *this;
}

bool ThreeWindingsTransformerToBeEstimated::shouldEstimateRatioTapChanger2() const {
    return m_rtc2Status;
}

ThreeWindingsTransformerToBeEstimated& ThreeWindingsTransformerToBeEstimated::shouldEstimateRatioTapChanger2(bool toBeEstimated) {
    m_rtc2Status = toBeEstimated;

    return *this;

}

bool ThreeWindingsTransformerToBeEstimated::shouldEstimateRatioTapChanger3() const {
    return m_rtc3Status;
}

ThreeWindingsTransformerToBeEstimated& ThreeWindingsTransformerToBeEstimated::shouldEstimateRatioTapChanger3(bool toBeEstimated) {
    m_rtc3Status = toBeEstimated;

    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
