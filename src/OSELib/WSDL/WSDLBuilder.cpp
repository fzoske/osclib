/*
 * WSDLBuilder.cpp
 *
 *  Created on: 31.07.2015
 *      Author: roehser
 */

#include <OSELib/SDC/SDCConstants.h>
#include "wsdl-custom.hxx"
#include "wsdl-soap-binding.hxx"
#include "ws-addressing.hxx"

#include "OSELib/WSDL/WSDLBuilder.h"
#include "OSELib/WSDL/schema.h"

namespace OSELib {
namespace WSDL {

WSDLBuilder::WSDLBuilder(const std::string & targetNamespace, const std::string & portType) :
	messageSnippet_("Message"),
	parametersSnippet_("parameters"),
	wsdl(new WS::WSDL::TDefinitions()),
	portType_(new WS::WSDL::TPortType(portType)),
	binding_(new WS::WSDL::TBinding(
			WS::WSDL::SOAP_BINDING::BindingType(),
			portType + "Binding",
			xml_schema::Qname(targetNamespace, portType))),
	targetNamespace_(targetNamespace)
{
	// setup namespaces and prefixes
	wsdl->targetNamespace(targetNamespace);
	definePrefixMapping("tns", targetNamespace);
	definePrefixMapping("wsa", SDC::NS_ADDRESSING);
	definePrefixMapping("wsdl", SDC::NS_WSDL);
	definePrefixMapping("soap", SDC::NS_WSDL_SOAP_BINDING);
	definePrefixMapping("xsd", SDC::NS_XML_SCHEMA);
	definePrefixMapping("mm", SDC::NS_MESSAGE_MODEL);
	definePrefixMapping("wsstm", SDC::WS_MEX_ORNET_NS_STREAM);
	definePrefixMapping("wsp", SDC::WS_POLICY);

	// add wsdl imports
	wsdl->import().push_back(WS::WSDL::TImport(SDC::NS_MESSAGE_MODEL, SCHEMA::SCHEMA_MESSAGEMODEL_NAME));

	// replace empty binding from constructor of binding_ by correctly defined soap binding
	WS::WSDL::SOAP_BINDING::BindingType::StyleType bindingStyle(WS::WSDL::SOAP_BINDING::BindingType::StyleType::document);
	WS::WSDL::SOAP_BINDING::BindingType soapBinding;
	soapBinding.style(bindingStyle);
	WS::WSDL::SOAP_BINDING::BindingType::TransportType transportType(SDC::SOAP_HTTP_BINDING_URI);
	soapBinding.transport(transportType);
	binding_->binding(soapBinding);
}

WSDLBuilder::~WSDLBuilder() {

}

std::string WSDLBuilder::serialize() {
	const xml_schema::Flags xercesFlags(xml_schema::Flags::dont_validate | xml_schema::Flags::no_xml_declaration | xml_schema::Flags::dont_initialize);
	const std::string XML_ENCODING("UTF-8");

	wsdl->portType().clear();
	wsdl->portType().push_back(*portType_);

	wsdl->binding().clear();
	wsdl->binding().push_back(*binding_);

	std::ostringstream result;
	WS::WSDL::definitions(result, *wsdl, map, XML_ENCODING, xercesFlags);
	return result.str();
}

void WSDLBuilder::addNotification(const std::string & notificationName, const xml_schema::Qname & notification, const std::string & action) {
	portType_->EventSource(true);
	addMessage(notification);
	{ // define operation of portType
		WS::WSDL::TOperation operation(notificationName);
		{
			xml_schema::Qname element(targetNamespace_, notification.name() + messageSnippet_);
			WS::WSDL::TParam output(element);
			output.Action(action);
			operation.output(output);
		}
		portType_->operation().push_back(operation);
	}
	{ // define operation of binding
		WS::WSDL::SOAP_BINDING::OperationType operationType;
		operationType.soapAction(action);
		operationType.style(WS::WSDL::SOAP_BINDING::StyleChoice(WS::WSDL::SOAP_BINDING::StyleChoice::document));

		WS::WSDL::SOAP_BINDING::BodyType bodyType;
		bodyType.use(WS::WSDL::SOAP_BINDING::UseChoice(WS::WSDL::SOAP_BINDING::UseChoice::literal));

		WS::WSDL::TBindingOperation bindingOperation(operationType, notificationName);

		WS::WSDL::TBindingOperationMessage output(bodyType);
		bindingOperation.output(output);

		binding_->operation().push_back(bindingOperation);
	}
}

void WSDLBuilder::addStreamType(const std::string & targetNs, const std::string & actionURI, const std::string & type, const std::string & id) {
    WS::STREAMING::TStreamType streamType;
    xml_schema::Uri action(actionURI);
    streamType.actionURI(action);
    xml_schema::Qname element(targetNamespace_, id);
    streamType.element(element);
    streamType.id(id);
    streamType.streamType(type);
    WS::STREAMING::TStreamDescriptions streamDescr(streamType);
    streamDescr.targetNamespace(targetNs);
    WS::STREAMING::StreamSource streamSource(streamDescr);
    WS::POLICY::Policy p(streamSource);
    portType_->Policy(p);
}

void WSDLBuilder::addOperation(const std::string & operationName, const xml_schema::Qname & request, const xml_schema::Qname & response, const std::string & requestAction, const std::string & responseAction) {
	addMessage(request);
	addMessage(response);
	{ // define operation of portType
		WS::WSDL::TOperation operation(operationName);
		{
			xml_schema::Qname element(targetNamespace_, request.name() + messageSnippet_);
			WS::WSDL::TParam input(element);
			input.Action(requestAction);
			operation.input(input);
		}
		{
			xml_schema::Qname element(targetNamespace_, response.name() + messageSnippet_);
			WS::WSDL::TParam output(element);
			output.Action(responseAction);
			operation.output(output);
		}
		portType_->operation().push_back(operation);
	}
	{ // define operation of binding
		WS::WSDL::SOAP_BINDING::OperationType operationType;
		operationType.soapAction(requestAction);
		operationType.style(WS::WSDL::SOAP_BINDING::StyleChoice(WS::WSDL::SOAP_BINDING::StyleChoice::document));

		WS::WSDL::SOAP_BINDING::BodyType bodyType;
		bodyType.use(WS::WSDL::SOAP_BINDING::UseChoice(WS::WSDL::SOAP_BINDING::UseChoice::literal));

		WS::WSDL::TBindingOperation bindingOperation(operationType, operationName);

		WS::WSDL::TBindingOperationMessage input(bodyType);
		bindingOperation.input(input);

		WS::WSDL::TBindingOperationMessage output(bodyType);
		bindingOperation.output(output);

		binding_->operation().push_back(bindingOperation);
	}
}

void WSDLBuilder::definePrefixMapping(const std::string & prefix, const std::string & ns) {
	map[prefix].name = ns;
}

void WSDLBuilder::addMessage(const xml_schema::Qname & qname) {
	WS::WSDL::TMessage message(qname.name() + messageSnippet_);
	WS::WSDL::TPart part(parametersSnippet_);
	part.element(qname);
	message.part().push_back(part);
	wsdl->message().push_back(message);
}

}
} /* namespace SDCLib */
