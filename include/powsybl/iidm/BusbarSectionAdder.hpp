/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSBARSECTIONADDER_HPP
#define POWSYBL_IIDM_BUSBARSECTIONADDER_HPP

#include <powsybl/iidm/IdentifiableAdder.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class BusbarSection;
class Network;
class NodeBreakerVoltageLevel;

class BusbarSectionAdder : public IdentifiableAdder<BusbarSectionAdder> {
public:
    explicit BusbarSectionAdder(NodeBreakerVoltageLevel& voltageLevel);

    ~BusbarSectionAdder() noexcept override = default;

    BusbarSection& add();

    BusbarSectionAdder& setNode(unsigned long node);

protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    NodeBreakerVoltageLevel& m_voltageLevel;

    stdcxx::optional<unsigned long> m_node;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSBARSECTIONADDER_HPP
