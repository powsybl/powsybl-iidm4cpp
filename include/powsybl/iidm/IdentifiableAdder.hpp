/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_IDENTIFIABLEADDER_HPP
#define POWSYBL_IIDM_IDENTIFIABLEADDER_HPP

#include <string>

#include <powsybl/iidm/Validable.hpp>

namespace powsybl {

namespace iidm {

class Network;

template<typename Adder>
class IdentifiableAdder : public Validable {
public:
    std::string getMessageHeader() const override;

public:
    virtual ~IdentifiableAdder() = default;

    Adder& setId(const std::string& id);

    Adder& setName(const std::string& name);

protected:
    virtual Network& getNetwork() = 0;

    virtual const std::string& getTypeDescription() const = 0;

protected:
    std::string m_id;

    std::string m_name;
};

}

}

#include <powsybl/iidm/IdentifiableAdder.hxx>

#endif  // POWSYBL_IIDM_IDENTIFIABLEADDER_HPP
