/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/sld/BranchStatusXmlSerializer.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Extendable.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/sld/BranchStatus.hpp>
#include <powsybl/iidm/extensions/sld/BranchStatusAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

BranchStatusXmlSerializer::BranchStatusXmlSerializer() :
    AbstractExtensionXmlSerializer("branchStatus", "network", "bs", "http://www.powsybl.org/schema/iidm/ext/branch_status/1_0") {
}

Extension& BranchStatusXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    const auto& status = Enum::fromString<BranchStatus::Status>(context.getReader().readUntilEndElement(getExtensionName()));
    extendable.newExtension<BranchStatusAdder>()
        .withStatus(status)
        .add();
    return extendable.getExtension<BranchStatus>();
}

void BranchStatusXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& branchStatus = safeCast<BranchStatus>(extension);
    context.getWriter().writeCharacters(Enum::toString(branchStatus.getStatus()));
}

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
