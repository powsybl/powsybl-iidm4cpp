/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SYNCHRONOUSCOMPONENT_HPP
#define POWSYBL_IIDM_SYNCHRONOUSCOMPONENT_HPP

#include <powsybl/iidm/Component.hpp>

namespace powsybl {

namespace iidm {

class SynchronousComponent : public Component {
public:
    SynchronousComponent(long num, unsigned long size, Network& network);

    ~SynchronousComponent() noexcept override = default;

protected: // Component
    stdcxx::CReference<Component> getFilterComponent(const Bus& bus) const override;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SYNCHRONOUSCOMPONENT_HPP
