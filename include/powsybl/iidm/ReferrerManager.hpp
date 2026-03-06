/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_REFERRERMANAGER_HPP
#define POWSYBL_IIDM_REFERRERMANAGER_HPP

#include <powsybl/iidm/Referrer.hpp>
#include <powsybl/stdcxx/reference.hpp>

#include <vector>

namespace powsybl {

namespace iidm {

/**
 * Class to make inherit from for classes subject of being referenced by a Referer<T>
 * Manages the list of Referer<T> that reference this object
 */
template<typename T>
class ReferrerManager {
public:
/**
* Add a referrer
*/
virtual void registerReferrer(Referrer<T>& referrer);

/**
 * Removes a referrer
 */
virtual void unregisterReferrer(const Referrer<T>& referrer);

virtual std::vector<stdcxx::Reference<Referrer<T>>> getReferrers();

/**
 * notify each registered Referrer that this ReferrerManager object is removed
 */
virtual void notifyRemoval();


private:
    std::vector<stdcxx::Reference<Referrer<T>>> m_referrers;

};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/ReferrerManager.hxx>

#endif  // POWSYBL_IIDM_REFERRER_HPP