/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SLD_CONNECTABLEPOSITIONADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_SLD_CONNECTABLEPOSITIONADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>
#include <powsybl/iidm/extensions/sld/ConnectablePosition.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

class ConnectablePositionAdder : public ExtensionAdder {
public:
    class FeederAdder {
    public:
        FeederAdder(ConnectablePositionAdder& parent, ConnectablePosition::OptionalFeeder& feeder);

        ConnectablePositionAdder& add();

        FeederAdder& withDirection(const ConnectablePosition::Direction& direction);

        FeederAdder& withName(const std::string& name);

        FeederAdder& withOrder(unsigned long order);

    private:
        std::string m_name;

        stdcxx::optional<unsigned long> m_order;

        stdcxx::optional<ConnectablePosition::Direction> m_direction;

        ConnectablePositionAdder& m_parent;

        ConnectablePosition::OptionalFeeder& m_feeder;
    };

public:
    /**
     * Constructor
     */
    explicit ConnectablePositionAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~ConnectablePositionAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    ConnectablePositionAdder(const ConnectablePositionAdder&) = default;

    /**
     * Move constructor
     */
    ConnectablePositionAdder(ConnectablePositionAdder&&) = default;

    /**
     * Copy assignment operator
     */
    ConnectablePositionAdder& operator=(const ConnectablePositionAdder&) = delete;

    /**
     * Move assignment operator
     */
    ConnectablePositionAdder& operator=(ConnectablePositionAdder&&) = delete;

    FeederAdder newFeeder();

    FeederAdder newFeeder1();

    FeederAdder newFeeder2();

    FeederAdder newFeeder3();

protected:
    /**
     * Creates the ConnectablePosition extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    ConnectablePosition::OptionalFeeder m_feeder;

    ConnectablePosition::OptionalFeeder m_feeder1;

    ConnectablePosition::OptionalFeeder m_feeder2;

    ConnectablePosition::OptionalFeeder m_feeder3;
};

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SLD_CONNECTABLEPOSITIONADDER_HPP
