/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONTAINER_HPP
#define POWSYBL_IIDM_CONTAINER_HPP

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

class IIDM_DECLSPEC Container : public Identifiable {
public:
    enum class Type {
        NETWORK,
        SUBSTATION,
        VOLTAGE_LEVEL
    };

public:
    Container(const Container&) = delete;

    Container(Container&& container) noexcept;

    ~Container() noexcept override = default;

    Container& operator=(const Container&) = delete;

    Container& operator=(Container&&) = delete;

    const Type& getContainerType() const;

protected:
    Container(const std::string& id, const std::string& name, const Container::Type& type);

private:
    Container::Type m_type;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONTAINER_HPP
