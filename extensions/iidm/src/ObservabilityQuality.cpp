/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ObservabilityQuality.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ObservabilityQuality::ObservabilityQuality(double standardDeviation) :
    m_standardDeviation(standardDeviation) {
    m_redundant.reset();
}

ObservabilityQuality::ObservabilityQuality(double standardDeviation, stdcxx::optional<bool> redundant) :
    m_standardDeviation(standardDeviation),
    m_redundant(redundant) {
}

double ObservabilityQuality::getStandardDeviation() const {
    return m_standardDeviation;
}

bool ObservabilityQuality::isRedundant() const {
    return m_redundant.has_value() && m_redundant.get();
}

stdcxx::optional<bool> ObservabilityQuality::getRedundant() const {
    return m_redundant;
}

ObservabilityQuality& ObservabilityQuality::resetRedundant() {
    m_redundant.reset();

    return *this;
}

ObservabilityQuality& ObservabilityQuality::setRedundant(stdcxx::optional<bool> redundant) {
    m_redundant = redundant;

    return *this;
}

ObservabilityQuality& ObservabilityQuality::setStandardDeviation(double standardDeviation) {
    m_standardDeviation = standardDeviation;

    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
