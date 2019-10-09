/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTIDENTIFIABLEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTIDENTIFIABLEXML_HPP

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

template <typename Added, typename Adder, typename Parent>
class AbstractIdentifiableXml {
public:
    void read(Parent& parent, NetworkXmlReaderContext& context) const;

    // FIXME(mathbagu): This method is public due to a bug in GCC 4.8.5 that doesn't allow lambda to acces to private/protected members
    // https://stackoverflow.com/questions/11933999/why-is-it-not-possible-to-use-private-method-in-a-lambda
    virtual void readSubElements(Added& identifiable, NetworkXmlReaderContext& context) const;
    
    void write(const Added& identifiable, const Parent& parent, NetworkXmlWriterContext& context) const;

protected:
    virtual Adder createAdder(Parent& parent) const = 0;

    virtual const char* getRootElementName() const = 0;

    virtual Added& readRootElementAttributes(Adder& adder, NetworkXmlReaderContext& context) const = 0;

    virtual void writeRootElementAttributes(const Added& identifiable, const Parent& parent, NetworkXmlWriterContext& context) const = 0;

    virtual void writeSubElements(const Added& identifiable, const Parent& parent, NetworkXmlWriterContext& context) const;

protected:
    AbstractIdentifiableXml() = default;

    virtual ~AbstractIdentifiableXml() noexcept = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include "AbstractIdentifiableXml.hxx"

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTIDENTIFIABLEXML_HPP
