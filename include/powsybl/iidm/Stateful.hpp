/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_STATEFUL_HPP
#define POWSYBL_IIDM_STATEFUL_HPP

#include <set>

namespace powsybl {

namespace iidm {

class Stateful {
public:
    virtual ~Stateful() = default;

    virtual void allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) = 0;

    virtual void deleteStateArrayElement(unsigned long index) = 0;

    virtual void extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) = 0;

    virtual void reduceStateArraySize(unsigned long number) = 0;
};

}

}

#endif  // POWSYBL_IIDM_STATEFUL_HPP
