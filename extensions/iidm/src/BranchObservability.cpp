/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/BranchObservability.hpp>

#include <powsybl/iidm/Branch.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BranchObservability::BranchObservability(Branch& branch, bool observable) :
    Extension(branch),
    Observability(observable) {
}

void BranchObservability::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Branch>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Branch>()));
    }
}

const std::string& BranchObservability::getName() const {
    static std::string s_name = "branchObservability";
    return s_name;
}

stdcxx::CReference<ObservabilityQuality> BranchObservability::getQualityP1() const {
    return stdcxx::cref<ObservabilityQuality>(m_qualityP1);
}

stdcxx::Reference<ObservabilityQuality> BranchObservability::getQualityP1() {
    return stdcxx::ref<ObservabilityQuality>(m_qualityP1);
}

stdcxx::CReference<ObservabilityQuality> BranchObservability::getQualityP2() const {
    return stdcxx::cref<ObservabilityQuality>(m_qualityP2);
}

stdcxx::Reference<ObservabilityQuality> BranchObservability::getQualityP2() {
    return stdcxx::ref<ObservabilityQuality>(m_qualityP2);
}

stdcxx::CReference<ObservabilityQuality> BranchObservability::getQualityQ1() const {
    return stdcxx::cref<ObservabilityQuality>(m_qualityQ1);
}

stdcxx::Reference<ObservabilityQuality> BranchObservability::getQualityQ1() {
    return stdcxx::ref<ObservabilityQuality>(m_qualityQ1);
}

stdcxx::CReference<ObservabilityQuality> BranchObservability::getQualityQ2() const {
    return stdcxx::cref<ObservabilityQuality>(m_qualityQ2);
}

stdcxx::Reference<ObservabilityQuality> BranchObservability::getQualityQ2() {
    return stdcxx::ref<ObservabilityQuality>(m_qualityQ2);
}

const std::type_index& BranchObservability::getType() const {
    static std::type_index s_type = typeid(BranchObservability);
    return s_type;
}

BranchObservability& BranchObservability::setQualityP1(double standardDeviation) {
    m_qualityP1 = ObservabilityQuality(standardDeviation);

    return *this;
}
BranchObservability& BranchObservability::setQualityP1(double standardDeviation, stdcxx::optional<bool> redundant) {
    m_qualityP1 = ObservabilityQuality(standardDeviation, redundant);

    return *this;
}

BranchObservability& BranchObservability::setQualityP2(double standardDeviation) {
    m_qualityP2 = ObservabilityQuality(standardDeviation);

    return *this;
}
BranchObservability& BranchObservability::setQualityP2(double standardDeviation, stdcxx::optional<bool> redundant) {
    m_qualityP2 = ObservabilityQuality(standardDeviation, redundant);

    return *this;
}

BranchObservability& BranchObservability::setQualityQ1(double standardDeviation) {
    m_qualityQ1 = ObservabilityQuality(standardDeviation);

    return *this;
}
BranchObservability& BranchObservability::setQualityQ1(double standardDeviation, stdcxx::optional<bool> redundant) {
    m_qualityQ1 = ObservabilityQuality(standardDeviation, redundant);

    return *this;
}

BranchObservability& BranchObservability::setQualityQ2(double standardDeviation) {
    m_qualityQ2 = ObservabilityQuality(standardDeviation);

    return *this;
}
BranchObservability& BranchObservability::setQualityQ2(double standardDeviation, stdcxx::optional<bool> redundant) {
    m_qualityQ2 = ObservabilityQuality(standardDeviation, redundant);

    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
