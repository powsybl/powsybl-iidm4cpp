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
class VoltageLevel;

class BusbarSectionAdder : public IdentifiableAdder<BusbarSectionAdder> {
public:
    virtual ~BusbarSectionAdder() = default;

    BusbarSection& add();

    BusbarSectionAdder& setNode(int node);

protected: // IdentifiableAdder
    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit BusbarSectionAdder(VoltageLevel& voltageLevel);

    friend class VoltageLevel;

private:
    VoltageLevel& m_voltageLevel;

    stdcxx::optional<int> m_node;
};


}

}

#endif  // POWSYBL_IIDM_BUSBARSECTIONADDER_HPP
