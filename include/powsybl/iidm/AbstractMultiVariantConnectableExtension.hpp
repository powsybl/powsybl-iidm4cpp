/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ABSTRACTMULTIVARIANTCONNECTABLEEXTENSION_HPP
#define POWSYBL_IIDM_ABSTRACTMULTIVARIANTCONNECTABLEEXTENSION_HPP

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/iidm/MultiVariantObject.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

class Connectable;
class VariantManagerHolder;

class IIDM_DECLSPEC AbstractMultiVariantConnectableExtension : public Extension, public MultiVariantObject {
public:
    explicit AbstractMultiVariantConnectableExtension(Extendable& extendable);

protected:
    static const VariantManagerHolder& getVariantManagerHolder(const Connectable& connectable);

protected:
    unsigned long getVariantIndex() const;

    const VariantManagerHolder& getVariantManagerHolder() const;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ABSTRACTMULTIVARIANTCONNECTABLEEXTENSION_HPP
