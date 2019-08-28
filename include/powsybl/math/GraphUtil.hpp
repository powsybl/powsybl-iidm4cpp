/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_MATH_GRAPHUTIL_HPP
#define POWSYBL_MATH_GRAPHUTIL_HPP

#include <vector>

namespace powsybl {

namespace math {

namespace GraphUtil {

class ConnectedComponentsComputationResult {
public:
    ConnectedComponentsComputationResult(std::vector<long>& componentNumber, std::vector<unsigned long>& orderedComponents);

    ConnectedComponentsComputationResult(ConnectedComponentsComputationResult&&) noexcept = default;

    ~ConnectedComponentsComputationResult() noexcept = default;

    const std::vector<long>& getComponentNumber() const;

    const std::vector<unsigned long>& getComponentSize() const;

private:
    std::vector<long> m_componentNumber;

    std::vector<unsigned long> m_componentSize;
};

ConnectedComponentsComputationResult computeConnectedComponents(const std::vector<std::vector<long>>& adjacencyList);

}  // namespace GraphUtil

}  // namespace math

}  // namespace powsybl

#endif  // POWSYBL_MATH_GRAPHUTIL_HPP
