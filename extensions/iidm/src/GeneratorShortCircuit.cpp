/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/GeneratorShortCircuit.hpp>

#include <powsybl/iidm/Generator.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorShortCircuit::GeneratorShortCircuit(Generator& generator, double directSubtransX, double directTransX, double stepUpTransformerX) :
    Extension(generator),
    m_directSubtransX(directSubtransX),
    m_directTransX(directTransX),
    m_stepUpTransformerX(stepUpTransformerX) {
}

void GeneratorShortCircuit::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Generator>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Generator>()));
    }
}

double GeneratorShortCircuit::getDirectSubtransX() const {
    return m_directSubtransX;
}

double GeneratorShortCircuit::getDirectTransX() const {
    return m_directTransX;
}

const std::string& GeneratorShortCircuit::getName() const {
    static std::string s_name = "generatorShortCircuit";
    return s_name;
}

double GeneratorShortCircuit::getStepUpTransformerX() const {
    return m_stepUpTransformerX;
}

const std::type_index& GeneratorShortCircuit::getType() const {
    static std::type_index s_type = typeid(GeneratorShortCircuit);
    return s_type;
}

GeneratorShortCircuit& GeneratorShortCircuit::setDirectSubtransX(double directSubtransX) {
    m_directSubtransX = directSubtransX;
    return *this;
}

GeneratorShortCircuit& GeneratorShortCircuit::setDirectTransX(double directTransX) {
    m_directTransX = directTransX;
    return *this;
}

GeneratorShortCircuit& GeneratorShortCircuit::setStepUpTransformerX(double setUpTransformerX) {
    m_stepUpTransformerX = setUpTransformerX;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
