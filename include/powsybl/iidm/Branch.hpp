/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BRANCH_HPP
#define POWSYBL_IIDM_BRANCH_HPP

#include <ostream>

#include <powsybl/iidm/Connectable.hpp>

namespace powsybl {

namespace iidm {

class Branch : public Connectable {
public:
    enum class Side : unsigned int {
        ONE,
        TWO
    };

public:
    ~Branch() override = default;

    Terminal& getTerminal(const Side& side) const;

    Terminal& getTerminal1() const;

    Terminal& getTerminal2() const;

protected:
    Branch(const std::string& id, const std::string& name, const ConnectableType& connectableType);
};

const std::string& getSideName(const Branch::Side& side);

std::ostream& operator<<(std::ostream& stream, const Branch::Side& side);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BRANCH_HPP
