/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_CVG_GENERATORENTSOECATEGORY_HPP
#define POWSYBL_IIDM_EXTENSIONS_CVG_GENERATORENTSOECATEGORY_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Generator;

namespace extensions {

namespace cvg {

class GeneratorEntsoeCategory : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    GeneratorEntsoeCategory(Generator& generator, unsigned long code);

    unsigned long getCode() const;

    GeneratorEntsoeCategory& setCode(unsigned long code);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    static unsigned long checkCode(unsigned long code);

private:
    unsigned long m_code;
};

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_CVG_GENERATORENTSOECATEGORY_HPP

