/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_MULTIVARIANTOBJECT_HPP
#define POWSYBL_IIDM_MULTIVARIANTOBJECT_HPP

#include <set>

#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

class VariantManager;

class IIDM_DECLSPEC MultiVariantObject {
public:
    virtual ~MultiVariantObject() noexcept = default;

protected:
    virtual void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) = 0;

    virtual void deleteVariantArrayElement(unsigned long index) = 0;

    virtual void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) = 0;

    virtual void reduceVariantArraySize(unsigned long number) = 0;

private:
    friend class VariantManager;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_MULTIVARIANTOBJECT_HPP
