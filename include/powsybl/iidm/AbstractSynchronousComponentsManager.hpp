/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ABSTRACTSYNCHRONOUSCOMPONENTSMANAGER_HPP
#define POWSYBL_IIDM_ABSTRACTSYNCHRONOUSCOMPONENTSMANAGER_HPP

#include <powsybl/iidm/AbstractComponentsManager.hpp>

namespace powsybl {

namespace iidm {

template <typename C>
class AbstractSynchronousComponentsManager : public AbstractComponentsManager<C> {
protected:
    AbstractSynchronousComponentsManager();
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/AbstractSynchronousComponentsManager.hxx>

#endif  // POWSYBL_IIDM_ABSTRACTSYNCHRONOUSCOMPONENTSMANAGER_HPP
