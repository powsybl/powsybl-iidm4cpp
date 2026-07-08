/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCCOMPONENT_HPP
#define POWSYBL_IIDM_DCCOMPONENT_HPP

#include <powsybl/iidm/Component.hpp>

namespace powsybl {

namespace iidm {

class DcComponent : public Component {
public:
    DcComponent(Network& network, unsigned long num, unsigned long size);

    ~DcComponent() noexcept override = default;

public :  //Component
    stdcxx::const_range<Bus> getBuses() const override;

    stdcxx::range<Bus> getBuses() override;

protected:  // Component
    bool checkBus(const Bus& bus) const override;
    bool checkDcBus(const DcBus& dcBus) const override;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCCOMPONENT_HPP
