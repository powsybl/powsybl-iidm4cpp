/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORK_LOADFOOEXT_HPP
#define POWSYBL_IIDM_NETWORK_LOADFOOEXT_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Load;

}  // namespace iidm

namespace network {

class LoadFooExt : public iidm::Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    explicit LoadFooExt(iidm::Load& load);

    ~LoadFooExt() override = default;

private:  // Extension
    void assertExtendable(const stdcxx::Reference<iidm::Extendable>& extendable) const override;
};

}  // namespace network

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORK_LOADFOOEXT_HPP
