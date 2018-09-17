/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSBARSECTION_HPP
#define POWSYBL_IIDM_BUSBARSECTION_HPP

#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/MultiStateObject.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class BusbarSection : public Injection {
public:
    BusbarSection(MultiStateObject& network, const std::string& id, const std::string& name);

    ~BusbarSection() noexcept override = default;

    double getAngle() const;

    double getV() const;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    stdcxx::Reference<MultiStateObject> m_network;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSBARSECTION_HPP
