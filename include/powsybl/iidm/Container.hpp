/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONTAINER_HPP
#define POWSYBL_IIDM_CONTAINER_HPP

#include <powsybl/iidm/Identifiable.hpp>

namespace powsybl {

namespace iidm {

class Container : public Identifiable {
public:
    enum class Type {
        NETWORK,
        SUBSTATION,
        VOLTAGE_LEVEL
    };

public:
    ~Container() override = default;

    const Type& getType() const;

protected:
    Container(const std::string& id, const std::string& name, const Container::Type& type);

    Container(Container&&) = default;

private:
    Container::Type m_type;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONTAINER_HPP
