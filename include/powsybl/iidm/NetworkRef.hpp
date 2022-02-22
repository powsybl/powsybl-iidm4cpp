/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORKREF_HPP
#define POWSYBL_IIDM_NETWORKREF_HPP

namespace powsybl {

namespace iidm {

class Network;

/**
 * A {@link NetworkRef} instance holds a reference to a network.
 * It uses to maintain the back reference between an {@link Identifiable} and the network it belongs to.
 */
class NetworkRef {
public:
    /**
     * Create a reference to a {@link Network} object.
     * @param network the network pointed by this reference.
     */
    explicit NetworkRef(Network& network);

    /**
     * Copy constructor
     */
    NetworkRef(const NetworkRef&) = delete;

    /**
     * Move constructor
     */
     NetworkRef(NetworkRef&&) noexcept = delete;

    /**
     * Destructor
     */
    ~NetworkRef() = default;

    /**
     * Copy assignment operator
     */
    NetworkRef& operator=(const NetworkRef&) = delete;

    /**
     * Move assignment operator
     */
    NetworkRef& operator=(NetworkRef&&) noexcept = delete;

    /**
     * Return true if the reference to the network is valid, false otherwise
     * @return true if the reference to the network is valid, false otherwise
     */
    operator bool() const noexcept;

    /**
     * Get the {@link Network} object pointed by this reference.
     * @return a network object
     */
    const Network& get() const;

    /**
     * Get the {@link Network} object pointed by this reference.
     * @return a network object
     */
    Network& get();

    /**
     * Set the {@link Network} object pointed by this reference.
     */
    void set(Network& network);

private:
    Network* m_pointer;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORKREF_HPP
