/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/math/ConnectedComponentsComputationResult.hpp>

namespace powsybl {

namespace math {

namespace GraphUtil {

ConnectedComponentsComputationResult::ConnectedComponentsComputationResult(std::vector<stdcxx::optional<unsigned long>> componentNumber, std::vector<unsigned long> orderedComponents) :
    m_componentNumber(std::move(componentNumber)),
    m_componentSize(std::move(orderedComponents)) {
}

const std::vector<stdcxx::optional<unsigned long>>& ConnectedComponentsComputationResult::getComponentNumber() const {
    return m_componentNumber;
}

const std::vector<unsigned long>& ConnectedComponentsComputationResult::getComponentSize() const {
    return m_componentSize;
}

}  // namespace GraphUtil

}  // namespace math

}  // namespace powsybl
