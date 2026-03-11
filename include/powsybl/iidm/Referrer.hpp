/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_REFERRER_HPP
#define POWSYBL_IIDM_REFERRER_HPP


namespace powsybl {

namespace iidm {

/**
 * Abstract class to make inherit from for classes holding references to object of type T
 * Manages the removal of the referenced object.
 */
template<typename T>
class Referrer {
public:
    /**
     * Called when a referenced object is removed (for instance a connectable removal which deletes its terminals)
     * Implementations of this method should handle any required cleanup or updates implied by this referenced object deletion.
     * @param removedReference : The reference that has been removed
     */
virtual void onReferencedRemoval(T& removedReference) = 0;

    /**
    * Called when a referenced object is being replaced (before being removed)
    * Implementations of this method should handle any required cleanup or updates implied by this referenced object deletion,
    * and registering the new reference. Do not unregister the previously hold reference, since it is intented to be deleted after replacement.
    * @param oldReference : The reference being replaced
    * @param newReference : The reference that should replace the oldReference
    */
virtual void onReferencedReplacement(T& oldReference, T& newReference) = 0;

};

}  // namespace iidm

}  // namespace powsybl

// #include <powsybl/iidm/Referrer.hxx>

#endif  // POWSYBL_IIDM_REFERRER_HPP