/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SLD_CONNECTABLEPOSITION_HPP
#define POWSYBL_IIDM_EXTENSIONS_SLD_CONNECTABLEPOSITION_HPP

#include <powsybl/iidm/Extension.hpp>
#include <powsybl/stdcxx/optional.hpp>

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

    using OptionalFeeder = stdcxx::optional<Feeder>;

public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    stdcxx::CReference<Feeder> getFeeder() const;

    stdcxx::Reference<Feeder> getFeeder();

    stdcxx::CReference<Feeder> getFeeder1() const;

    stdcxx::Reference<Feeder> getFeeder1();

    stdcxx::CReference<Feeder> getFeeder2() const;

    stdcxx::Reference<Feeder> getFeeder2();

    stdcxx::CReference<Feeder> getFeeder3() const;

    stdcxx::Reference<Feeder> getFeeder3();

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    static void check(const OptionalFeeder& feeder, const OptionalFeeder& feeder1, const OptionalFeeder& feeder2, const OptionalFeeder& feeder3);

private:
    ConnectablePosition(Connectable& connectable, const OptionalFeeder& feeder, const OptionalFeeder& feeder1, const OptionalFeeder& feeder2, const OptionalFeeder& feeder3);

    template <typename B, typename D, typename, typename... Args>
    friend std::unique_ptr<B> stdcxx::make_unique(Args&&... args);

private:
    OptionalFeeder m_feeder;

    OptionalFeeder m_feeder1;

    OptionalFeeder m_feeder2;

    OptionalFeeder m_feeder3;
};

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SLD_CONNECTABLEPOSITION_HPP
