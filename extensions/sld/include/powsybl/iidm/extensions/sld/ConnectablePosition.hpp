/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SLD_CONNECTABLEPOSITION_HPP
#define POWSYBL_IIDM_EXTENSIONS_SLD_CONNECTABLEPOSITION_HPP

#include <boost/optional.hpp>

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Connectable;

namespace extensions {

namespace sld {

class ConnectablePosition : public Extension {
public:
    enum class Direction : unsigned char {
        TOP,
        BOTTOM,
        UNDEFINED
    };

    class Feeder {
    public:
        Feeder(const std::string& name, unsigned long order, const Direction& direction);

        const Direction& getDirection() const;

        const std::string& getName() const;

        unsigned long getOrder() const;

        Feeder& setDirection(const Direction& direction);

        Feeder& setName(const std::string& name);

        Feeder& setOrder(unsigned long order);

    private:
        std::string m_name;

        unsigned long m_order;

        Direction m_direction;
    };

    using OptionalFeeder = boost::optional<Feeder>;

public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    ConnectablePosition(Connectable& connectable, OptionalFeeder&& feeder, OptionalFeeder&& feeder1, OptionalFeeder&& feeder2, OptionalFeeder&& feeder3);

    const OptionalFeeder& getFeeder() const;

    OptionalFeeder& getFeeder();

    const OptionalFeeder& getFeeder1() const;

    OptionalFeeder& getFeeder1();

    const OptionalFeeder& getFeeder2() const;

    OptionalFeeder& getFeeder2();

    const OptionalFeeder& getFeeder3() const;

    OptionalFeeder& getFeeder3();

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    static void check(const OptionalFeeder& feeder, const OptionalFeeder& feeder1, const OptionalFeeder& feeder2, const OptionalFeeder& feeder3);

private:
    OptionalFeeder m_feeder;

    OptionalFeeder m_feeder1;

    OptionalFeeder m_feeder2;

    OptionalFeeder m_feeder3;
};

std::ostream& operator<<(std::ostream& stream, const ConnectablePosition::Direction& value);

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SLD_CONNECTABLEPOSITION_HPP
