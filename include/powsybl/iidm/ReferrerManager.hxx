/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_REFERRERMANAGER_HXX
#define POWSYBL_IIDM_REFERRERMANAGER_HXX

#include <powsybl/iidm/ReferrerManager.hpp>

#include <powsybl/stdcxx/memory.hpp>

#include <algorithm>

namespace powsybl {

namespace iidm {

template<typename T>
void ReferrerManager<T>::registerReferrer(Referrer<T>& referrer) {
    m_referrers.emplace_back(referrer);
}

template<typename T>
void ReferrerManager<T>::unregisterReferrer(const Referrer<T>& referrer) {
    m_referrers.erase(std::remove_if(m_referrers.begin(), m_referrers.end(), [&referrer](const stdcxx::Reference<Referrer<T>>& ref) {
        return (static_cast<bool>(ref) && stdcxx::areSame(referrer, ref.get()));
    }), m_referrers.end());
}

template<typename T>
std::vector<stdcxx::Reference<Referrer<T>>> ReferrerManager<T>::getReferrers() {
    return m_referrers;
}

template<typename T>
void ReferrerManager<T>::notifyRemoval() {
    for(auto& referrer : m_referrers) {
        if(static_cast<bool>(referrer)) {
            referrer.get().onReferencedRemoval(dynamic_cast<T&>(*this));
        }
    }
    //All referrers holding this, should now refer to something else.
    m_referrers.clear();
}

template<typename T>
void ReferrerManager<T>::notifyReplacement(T& newReferred) {
    for(auto& referrer : m_referrers) {
        if(static_cast<bool>(referrer)) {
            referrer.get().onReferencedReplacement(dynamic_cast<T&>(*this), newReferred);
        }
    }
    //All referrers holding this, should now refer to something else.
    m_referrers.clear();
}


}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_REFERRERMANAGER_HXX