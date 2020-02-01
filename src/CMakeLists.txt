#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(IIDM_SOURCES
    AssertionError.cpp
    PowsyblException.cpp

    iidm/AbstractMultiVariantConnectableExtension.cpp
    iidm/Battery.cpp
    iidm/BatteryAdder.cpp
    iidm/Branch.cpp
    iidm/Bus.cpp
    iidm/BusAdder.cpp
    iidm/BusbarSection.cpp
    iidm/BusbarSectionAdder.cpp
    iidm/BusBreakerVoltageLevel.cpp
    iidm/BusBreakerVoltageLevelBusCache.cpp
    iidm/BusBreakerVoltageLevelTopology.cpp
    iidm/BusBreakerVoltageLevelVariant.cpp
    iidm/BusBreakerVoltageLevelViews.cpp
    iidm/BusTerminal.cpp
    iidm/BusTerminalViews.cpp
    iidm/CalculatedBus.cpp
    iidm/ConfiguredBus.cpp
    iidm/Connectable.cpp
    iidm/ConnectableType.cpp
    iidm/Container.cpp
    iidm/Country.cpp
    iidm/CurrentLimits.cpp
    iidm/DanglingLine.cpp
    iidm/DanglingLineAdder.cpp
    iidm/EnergySource.cpp
    iidm/Extendable.cpp
    iidm/Extension.cpp
    iidm/ExtensionProvider.cpp
    iidm/Generator.cpp
    iidm/GeneratorAdder.cpp
    iidm/HvdcConverterStation.cpp
    iidm/HvdcLine.cpp
    iidm/HvdcLineAdder.cpp
    iidm/Identifiable.cpp
    iidm/Injection.cpp
    iidm/InternalConnection.cpp
    iidm/InternalConnectionAdder.cpp
    iidm/LccConverterStation.cpp
    iidm/LccConverterStationAdder.cpp
    iidm/Line.cpp
    iidm/LineAdder.cpp
    iidm/LineCharacteristics.cpp
    iidm/Load.cpp
    iidm/LoadAdder.cpp
    iidm/LoadType.cpp
    iidm/MergedBus.cpp
    iidm/MinMaxReactiveLimits.cpp
    iidm/MinMaxReactiveLimitsAdder.cpp
    iidm/MultipleVariantContext.cpp
    iidm/Network.cpp
    iidm/NetworkIndex.cpp
    iidm/NetworkViews.cpp
    iidm/NodeBreakerVoltageLevel.cpp
    iidm/NodeBreakerVoltageLevelBusCache.cpp
    iidm/NodeBreakerVoltageLevelBusNamingStrategy.cpp
    iidm/NodeBreakerVoltageLevelTopology.cpp
    iidm/NodeBreakerVoltageLevelVariant.cpp
    iidm/NodeBreakerVoltageLevelViews.cpp
    iidm/NodeTerminal.cpp
    iidm/NodeTerminalViews.cpp
    iidm/PhaseTapChanger.cpp
    iidm/PhaseTapChangerAdder.cpp
    iidm/PhaseTapChangerStep.cpp
    iidm/Properties.cpp
    iidm/RatioTapChanger.cpp
    iidm/RatioTapChangerAdder.cpp
    iidm/RatioTapChangerStep.cpp
    iidm/ReactiveCapabilityCurve.cpp
    iidm/ReactiveCapabilityCurveAdder.cpp
    iidm/ReactiveLimitsHolder.cpp
    iidm/ShuntCompensator.cpp
    iidm/ShuntCompensatorAdder.cpp
    iidm/StaticVarCompensator.cpp
    iidm/StaticVarCompensatorAdder.cpp
    iidm/Substation.cpp
    iidm/SubstationAdder.cpp
    iidm/Switch.cpp
    iidm/SwitchAdder.cpp
    iidm/SwitchKind.cpp
    iidm/Terminal.cpp
    iidm/TerminalBuilder.cpp
    iidm/ThreeWindingsTransformer.cpp
    iidm/ThreeWindingsTransformerAdder.cpp
    iidm/ThreeWindingsTransformerLeg2.cpp
    iidm/ThreeWindingsTransformerLeg3.cpp
    iidm/TieLine.cpp
    iidm/TieLineAdder.cpp
    iidm/TopologyKind.cpp
    iidm/TopologyLevel.cpp
    iidm/TwoWindingsTransformer.cpp
    iidm/TwoWindingsTransformerAdder.cpp
    iidm/ValidationException.cpp
    iidm/ValidationUtils.cpp
    iidm/VariantContextGuard.cpp
    iidm/VariantManager.cpp
    iidm/VoltageLevel.cpp
    iidm/VoltageLevelAdder.cpp
    iidm/VscConverterStation.cpp
    iidm/VscConverterStationAdder.cpp

    iidm/converter/BusFilter.cpp
    iidm/converter/ExportOptions.cpp
    iidm/converter/FakeAnonymizer.cpp
    iidm/converter/ImportOptions.cpp
    iidm/converter/xml/BatteryXml.cpp
    iidm/converter/xml/BusbarSectionXml.cpp
    iidm/converter/xml/BusBreakerViewSwitchXml.cpp
    iidm/converter/xml/BusXml.cpp
    iidm/converter/xml/DanglingLineXml.cpp
    iidm/converter/xml/ExtensionXmlSerializer.cpp
    iidm/converter/xml/GeneratorXml.cpp
    iidm/converter/xml/HvdcLineXml.cpp
    iidm/converter/xml/LccConverterStationXml.cpp
    iidm/converter/xml/LineXml.cpp
    iidm/converter/xml/LoadXml.cpp
    iidm/converter/xml/NetworkXml.cpp
    iidm/converter/xml/NetworkXmlReaderContext.cpp
    iidm/converter/xml/NetworkXmlWriterContext.cpp
    iidm/converter/xml/NodeBreakerViewInternalConnectionXml.cpp
    iidm/converter/xml/NodeBreakerViewSwitchXml.cpp
    iidm/converter/xml/PropertiesXml.cpp
    iidm/converter/xml/ReactiveLimitsXml.cpp
    iidm/converter/xml/ShuntCompensatorXml.cpp
    iidm/converter/xml/StaticVarCompensatorXml.cpp
    iidm/converter/xml/SubstationXml.cpp
    iidm/converter/xml/TerminalRefXml.cpp
    iidm/converter/xml/ThreeWindingsTransformerXml.cpp
    iidm/converter/xml/TieLineXml.cpp
    iidm/converter/xml/TwoWindingsTransformerXml.cpp
    iidm/converter/xml/VoltageLevelXml.cpp
    iidm/converter/xml/VscConverterStationXml.cpp

    iidm/extensions/entsoe/EntsoeArea.cpp
    iidm/extensions/entsoe/EntsoeAreaXmlSerializer.cpp
    iidm/extensions/entsoe/EntsoeGeographicalCode.cpp
    iidm/extensions/entsoe/MergedXnode.cpp
    iidm/extensions/entsoe/MergedXnodeXmlSerializer.cpp
    iidm/extensions/entsoe/Xnode.cpp
    iidm/extensions/entsoe/XnodeXmlSerializer.cpp

    iidm/extensions/iidm/ActivePowerControl.cpp
    iidm/extensions/iidm/ActivePowerControlXmlSerializer.cpp
    iidm/extensions/iidm/CoordinatedReactiveControl.cpp
    iidm/extensions/iidm/CoordinatedReactiveControlXmlSerializer.cpp

    iidm/util/DistinctPredicate.cpp
    iidm/util/LimitViolationUtils.cpp

    logging/ConsoleLogger.cpp
    logging/ContainerLogger.cpp
    logging/Level.cpp
    logging/LoggerFactory.cpp
    logging/LogMessage.cpp
    logging/NoopLogger.cpp

    network/BatteryNetworkFactory.cpp
    network/EurostagFactory.cpp

    stdcxx/DateTime.cpp
    stdcxx/demangle.cpp
    stdcxx/put_time.cpp

    xml/XmlCharConversion.cpp
    xml/XmlStreamException.cpp
    xml/XmlStreamReader.cpp
    xml/XmlStreamWriter.cpp
)

add_library(iidm ${IIDM_SOURCES})
target_include_directories(iidm
    PRIVATE ${CMAKE_HOME_DIRECTORY}/include
    PRIVATE ${CMAKE_HOME_DIRECTORY}/src/
)
