/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSBARSECTION_HPP
#define POWSYBL_IIDM_BUSBARSECTION_HPP

#include <functional>

#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/MultiStateObject.hpp>

namespace powsybl {

namespace iidm {

class BusbarSection : public Injection {
public: // Identifiable
    const std::string& getTypeDescription() const override;

public:
    BusbarSection(MultiStateObject& network, const std::string& id, const std::string& name);

    virtual ~BusbarSection() = default;

    double getAngle() const;

    double getV() const;

private:
    std::reference_wrapper<MultiStateObject> m_network;
};

}

}

#endif  // POWSYBL_IIDM_BUSBARSECTION_HPP
