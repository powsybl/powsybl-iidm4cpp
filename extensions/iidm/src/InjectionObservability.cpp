/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/InjectionObservability.hpp>

#include <powsybl/iidm/Injection.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

InjectionObservability::InjectionObservability(Injection& injection, bool observable) :
    Extension(injection),
    Observability(observable) {
}

void InjectionObservability::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Injection>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Injection>()));
    }
}

const std::string& InjectionObservability::getName() const {
    static std::string s_name = "injectionObservability";
    return s_name;
}

stdcxx::CReference<ObservabilityQuality> InjectionObservability::getQualityP() const {
    return m_qualityP ? stdcxx::cref(*m_qualityP) : stdcxx::cref<ObservabilityQuality>();
}

stdcxx::Reference<ObservabilityQuality> InjectionObservability::getQualityP() {
    return m_qualityP ? stdcxx::ref(*m_qualityP) : stdcxx::ref<ObservabilityQuality>();
}

stdcxx::CReference<ObservabilityQuality> InjectionObservability::getQualityQ() const {
    return m_qualityQ ? stdcxx::cref(*m_qualityQ) : stdcxx::cref<ObservabilityQuality>();
}

stdcxx::Reference<ObservabilityQuality> InjectionObservability::getQualityQ() {
    return m_qualityQ ? stdcxx::ref(*m_qualityQ) : stdcxx::ref<ObservabilityQuality>();
}

stdcxx::CReference<ObservabilityQuality> InjectionObservability::getQualityV() const {
    return m_qualityV ? stdcxx::cref(*m_qualityV) : stdcxx::cref<ObservabilityQuality>();
}

stdcxx::Reference<ObservabilityQuality> InjectionObservability::getQualityV() {
    return m_qualityV ? stdcxx::ref(*m_qualityV) : stdcxx::ref<ObservabilityQuality>();
}

const std::type_index& InjectionObservability::getType() const {
    static std::type_index s_type = typeid(InjectionObservability);
    return s_type;
}

InjectionObservability& InjectionObservability::setQualityP(double standardDeviation, const stdcxx::optional<bool>& redundant) {
    m_qualityP = ObservabilityQuality(standardDeviation, redundant);

    return *this;
}

InjectionObservability& InjectionObservability::setQualityP(double standardDeviation) {
    m_qualityP = ObservabilityQuality(standardDeviation);

    return *this;
}

InjectionObservability& InjectionObservability::setQualityQ(double standardDeviation, const stdcxx::optional<bool>& redundant) {
    m_qualityQ = ObservabilityQuality(standardDeviation, redundant);

    return *this;
}

InjectionObservability& InjectionObservability::setQualityQ(double standardDeviation) {
    m_qualityQ = ObservabilityQuality(standardDeviation);

    return *this;
}

InjectionObservability& InjectionObservability::setQualityV(double standardDeviation, const stdcxx::optional<bool>& redundant) {
    m_qualityV = ObservabilityQuality(standardDeviation, redundant);

    return *this;
}

InjectionObservability& InjectionObservability::setQualityV(double standardDeviation) {
    m_qualityV = ObservabilityQuality(standardDeviation);

    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
