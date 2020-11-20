/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONNECTEDCOMPONENTSCOMPUTATIONRESULT_HPP
#define POWSYBL_IIDM_CONNECTEDCOMPONENTSCOMPUTATIONRESULT_HPP

#include <vector>

#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace math {

namespace graph_util {

class ConnectedComponentsComputationResult {
public:
    ConnectedComponentsComputationResult(std::vector<stdcxx::optional<unsigned long>>&& componentNumber, const std::vector<unsigned long>& orderedComponents);

    const std::vector<stdcxx::optional<unsigned long>>& getComponentNumber() const;

    const std::vector<unsigned long>& getComponentSize() const;

private:

    std::vector<stdcxx::optional<unsigned long>> m_componentNumber;

    std::vector<unsigned long> m_componentSize;
};

}  // namespace graph_util

}  // namespace math

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONNECTEDCOMPONENTSCOMPUTATIONRESULT_HPP
