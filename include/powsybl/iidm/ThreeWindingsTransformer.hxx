/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_THREEWINDINGSTRANSFORMER_HXX
#define POWSYBL_IIDM_THREEWINDINGSTRANSFORMER_HXX

#include <powsybl/iidm/ThreeWindingsTransformer.hpp>

namespace powsybl {

namespace iidm {

double checkR(const Validable& validable, double r);
double checkRatedU(const Validable& validable, double ratedU);
double checkX(const Validable& validable, double x);

template <typename L>
ThreeWindingsTransformer::LegBase<L>::LegBase(double r, double x, double ratedU) :
    m_r(checkR(*this, r)),
    m_x(checkX(*this, x)),
    m_ratedU(checkRatedU(*this, ratedU)) {
}

template <typename L>
stdcxx::CReference<CurrentLimits> ThreeWindingsTransformer::LegBase<L>::getCurrentLimits() const {
    return stdcxx::cref(m_limits);
}

template <typename L>
stdcxx::Reference<CurrentLimits> ThreeWindingsTransformer::LegBase<L>::getCurrentLimits() {
    return stdcxx::ref<CurrentLimits>(m_limits);
}

template <typename L>
std::string ThreeWindingsTransformer::LegBase<L>::getMessageHeader() const {
    return logging::format("%1% '%2%': ", getTypeDescription(), getRefTransformer().get().getId());
}

template <typename L>
double ThreeWindingsTransformer::LegBase<L>::getR() const {
    return m_r;
}

template <typename L>
double ThreeWindingsTransformer::LegBase<L>::getRatedU() const {
    return m_ratedU;
}

template <typename L>
stdcxx::CReference<Terminal> ThreeWindingsTransformer::LegBase<L>::getTerminal() const {
    return getTerminal(0UL);
}

template <typename L>
stdcxx::Reference<Terminal> ThreeWindingsTransformer::LegBase<L>::getTerminal() {
    return getTerminal(0UL);
}

template <typename L>
stdcxx::CReference<Terminal> ThreeWindingsTransformer::LegBase<L>::getTerminal(unsigned long index) const {
    if (m_transformer) {
        return stdcxx::cref<Terminal>(m_transformer.get().Connectable::getTerminal(index));
    }
    return {};
}

template <typename L>
stdcxx::Reference<Terminal> ThreeWindingsTransformer::LegBase<L>::getTerminal(unsigned long index) {
    const auto& terminal = static_cast<const ThreeWindingsTransformer::LegBase<L>*>(this)->getTerminal(index);

    return stdcxx::ref(terminal);
}

template <typename L>
stdcxx::CReference<ThreeWindingsTransformer> ThreeWindingsTransformer::LegBase<L>::getRefTransformer() const {
    return stdcxx::cref(m_transformer);
}

template <typename L>
stdcxx::Reference<ThreeWindingsTransformer>& ThreeWindingsTransformer::LegBase<L>::getRefTransformer() {
    return m_transformer;
}

template <typename L>
double ThreeWindingsTransformer::LegBase<L>::getX() const {
    return m_x;
}

template <typename L>
CurrentLimitsAdder<const std::nullptr_t, ThreeWindingsTransformer::LegBase<L>> ThreeWindingsTransformer::LegBase<L>::newCurrentLimits() {
    return CurrentLimitsAdder<const std::nullptr_t, LegBase<L>>(nullptr, *this);
}

template <typename L>
void ThreeWindingsTransformer::LegBase<L>::setCurrentLimits(std::nullptr_t /*side*/, std::unique_ptr<CurrentLimits> limits) {
    m_limits = std::move(limits);
}

template <typename L>
L& ThreeWindingsTransformer::LegBase<L>::setR(double r) {
    m_r = checkR(*this, r);

    return static_cast<L&>(*this);
}

template <typename L>
L& ThreeWindingsTransformer::LegBase<L>::setRatedU(double ratedU) {
    m_ratedU = checkRatedU(*this, ratedU);

    return static_cast<L&>(*this);
}

template <typename L>
void ThreeWindingsTransformer::LegBase<L>::setTransformer(ThreeWindingsTransformer& transformer) {
    m_transformer = stdcxx::ref(transformer);
}

template <typename L>
L& ThreeWindingsTransformer::LegBase<L>::setX(double x) {
    m_x = checkX(*this, x);

    return static_cast<L&>(*this);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_THREEWINDINGSTRANSFORMER_HXX
