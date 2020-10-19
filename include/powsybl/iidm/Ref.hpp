/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_REF_HPP
#define POWSYBL_IIDM_REF_HPP

namespace powsybl {

namespace iidm {

class Network;

/**
 * A {@link Ref} instance holds a reference to an object.
 * It uses to maintain the back reference between an object and the parent it belongs to.
 */
template <typename T>
class Ref {
public:
    /**
     * Create a reference to an object.
     * @param object the object pointed by this reference.
     */
    explicit Ref(T& object);

    /**
     * Copy constructor
     */
    Ref(const T&) = delete;

    /**
     * Move constructor
     */
    Ref(T&&) noexcept = delete;

    /**
     * Destructor
     */
    ~Ref() = default;

    /**
     * Copy assignment operator
     */
    Ref& operator=(const Ref&) = delete;

    /**
     * Move assignment operator
     */
    Ref& operator=(Ref&&) noexcept = delete;

    /**
     * Get the object pointed by this reference.
     * @return an object
     */
    const T& get() const;

    /**
     * Get the object pointed by this reference.
     * @return an object
     */
    T& get();

    /**
     * Set the object pointed by this reference.
     */
    void set(T& object);

private:
    T* m_pointer;
};

using NetworkRef = Ref<Network>;

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Ref.hxx>

#endif  // POWSYBL_IIDM_REF_HPP
