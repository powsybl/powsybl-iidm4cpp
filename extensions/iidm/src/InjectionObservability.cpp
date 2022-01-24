/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
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
    return stdcxx::cref<ObservabilityQuality>(m_qualityP);
}

stdcxx::Reference<ObservabilityQuality> InjectionObservability::getQualityP() {
    return stdcxx::ref<ObservabilityQuality>(m_qualityP);
}

stdcxx::CReference<ObservabilityQuality> InjectionObservability::getQualityQ() const {
    return stdcxx::cref<ObservabilityQuality>(m_qualityQ);
}

stdcxx::Reference<ObservabilityQuality> InjectionObservability::getQualityQ() {
    return stdcxx::ref<ObservabilityQuality>(m_qualityQ);
}

stdcxx::CReference<ObservabilityQuality> InjectionObservability::getQualityV() const {
    return stdcxx::cref<ObservabilityQuality>(m_qualityV);
}

stdcxx::Reference<ObservabilityQuality> InjectionObservability::getQualityV() {
    return stdcxx::ref<ObservabilityQuality>(m_qualityV);
}

const std::type_index& InjectionObservability::getType() const {
    static std::type_index s_type = typeid(InjectionObservability);
    return s_type;
}

InjectionObservability& InjectionObservability::setQualityP(double standardDeviation, bool redundant) {
    m_qualityP = ObservabilityQuality(standardDeviation, redundant);

    return *this;
}

InjectionObservability& InjectionObservability::setQualityQ(double standardDeviation, bool redundant) {
    m_qualityQ = ObservabilityQuality(standardDeviation, redundant);

    return *this;
}

InjectionObservability& InjectionObservability::setQualityV(double standardDeviation, bool redundant) {
    m_qualityV = ObservabilityQuality(standardDeviation, redundant);

    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
