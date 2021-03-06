// Copyright (c) 2005-2014 Code Synthesis Tools CC
//
// This program was generated by CodeSynthesis XSD, an XML Schema to
// C++ data binding compiler.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis Tools CC gives
// permission to link this program with the Xerces-C++ library (or with
// modified versions of Xerces-C++ that use the same license as Xerces-C++),
// and distribute linked combinations including the two. You must obey
// the GNU General Public License version 2 in all respects for all of
// the code used other than Xerces-C++. If you modify this copy of the
// program, you may extend this exception to your version of the program,
// but you are not obligated to do so. If you do not wish to do so, delete
// this exception statement from your version.
//
// Furthermore, Code Synthesis Tools CC makes a special exception for
// the Free/Libre and Open Source Software (FLOSS) which is described
// in the accompanying FLOSSE file.
//

#ifndef DICOMDEVICE_DESCRIPTION_HXX
#define DICOMDEVICE_DESCRIPTION_HXX

#include "DICOMDeviceDescription-fwd.hxx"

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/config.hxx>

#if (XSD_INT_VERSION != 4000000L)
#error XSD runtime version mismatch
#endif

#include <xsd/cxx/pre.hxx>

#include <memory>    // ::std::unique_ptr
#include <limits>    // std::numeric_limits
#include <algorithm> // std::binary_search
#include <utility>   // std::move

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/containers.hxx>
#include <xsd/cxx/tree/list.hxx>

#include <xsd/cxx/xml/dom/parsing-header.hxx>

#include "BICEPS_ParticipantModel.hxx"

namespace CDM
{
  class DicomTransferRole: public ::xml_schema::String
  {
    public:
    enum Value
    {
      Scu,
      Scp
    };

    DicomTransferRole (Value v);

    DicomTransferRole (const char* v);

    DicomTransferRole (const ::std::string& v);

    DicomTransferRole (const ::xml_schema::String& v);

    DicomTransferRole (const ::xercesc::DOMElement& e,
                       ::xml_schema::Flags f = 0,
                       ::xml_schema::Container* c = 0);

    DicomTransferRole (const ::xercesc::DOMAttr& a,
                       ::xml_schema::Flags f = 0,
                       ::xml_schema::Container* c = 0);

    DicomTransferRole (const ::std::string& s,
                       const ::xercesc::DOMElement* e,
                       ::xml_schema::Flags f = 0,
                       ::xml_schema::Container* c = 0);

    DicomTransferRole (const DicomTransferRole& x,
                       ::xml_schema::Flags f = 0,
                       ::xml_schema::Container* c = 0);

    virtual DicomTransferRole*
    _clone (::xml_schema::Flags f = 0,
            ::xml_schema::Container* c = 0) const;

    DicomTransferRole&
    operator= (Value v);

    virtual
    operator Value () const
    {
      return _xsd_DicomTransferRole_convert ();
    }

    protected:
    Value
    _xsd_DicomTransferRole_convert () const;

    public:
    static const char* const _xsd_DicomTransferRole_literals_[2];
    static const Value _xsd_DicomTransferRole_indexes_[2];
  };

  class DicomTransferCapability: public ::xml_schema::Type
  {
    public:
    // TransferSyntax
    //
    typedef ::xml_schema::String TransferSyntaxType;
    typedef ::xsd::cxx::tree::sequence< TransferSyntaxType > TransferSyntaxSequence;
    typedef TransferSyntaxSequence::iterator TransferSyntaxIterator;
    typedef TransferSyntaxSequence::const_iterator TransferSyntaxConstIterator;
    typedef ::xsd::cxx::tree::traits< TransferSyntaxType, char > TransferSyntaxTraits;

    const TransferSyntaxSequence&
    TransferSyntax () const;

    TransferSyntaxSequence&
    TransferSyntax ();

    void
    TransferSyntax (const TransferSyntaxSequence& s);

    // SopClass
    //
    typedef ::xml_schema::String SopClassType;
    typedef ::xsd::cxx::tree::traits< SopClassType, char > SopClassTraits;

    const SopClassType&
    SopClass () const;

    SopClassType&
    SopClass ();

    void
    SopClass (const SopClassType& x);

    void
    SopClass (::std::unique_ptr< SopClassType > p);

    // TransferRole
    //
    typedef ::CDM::DicomTransferRole TransferRoleType;
    typedef ::xsd::cxx::tree::traits< TransferRoleType, char > TransferRoleTraits;

    const TransferRoleType&
    TransferRole () const;

    TransferRoleType&
    TransferRole ();

    void
    TransferRole (const TransferRoleType& x);

    void
    TransferRole (::std::unique_ptr< TransferRoleType > p);

    // Constructors.
    //
    DicomTransferCapability (const SopClassType&,
                             const TransferRoleType&);

    DicomTransferCapability (const ::xercesc::DOMElement& e,
                             ::xml_schema::Flags f = 0,
                             ::xml_schema::Container* c = 0);

    DicomTransferCapability (const DicomTransferCapability& x,
                             ::xml_schema::Flags f = 0,
                             ::xml_schema::Container* c = 0);

    virtual DicomTransferCapability*
    _clone (::xml_schema::Flags f = 0,
            ::xml_schema::Container* c = 0) const;

    DicomTransferCapability&
    operator= (const DicomTransferCapability& x);

    virtual 
    ~DicomTransferCapability ();

    // Implementation.
    //
    protected:
    void
    parse (::xsd::cxx::xml::dom::parser< char >&,
           ::xml_schema::Flags);

    protected:
    TransferSyntaxSequence TransferSyntax_;
    ::xsd::cxx::tree::one< SopClassType > SopClass_;
    ::xsd::cxx::tree::one< TransferRoleType > TransferRole_;
  };

  class DicomNetworkAe: public ::xml_schema::Type
  {
    public:
    // TransferCapability
    //
    typedef ::CDM::DicomTransferCapability TransferCapabilityType;
    typedef ::xsd::cxx::tree::sequence< TransferCapabilityType > TransferCapabilitySequence;
    typedef TransferCapabilitySequence::iterator TransferCapabilityIterator;
    typedef TransferCapabilitySequence::const_iterator TransferCapabilityConstIterator;
    typedef ::xsd::cxx::tree::traits< TransferCapabilityType, char > TransferCapabilityTraits;

    const TransferCapabilitySequence&
    TransferCapability () const;

    TransferCapabilitySequence&
    TransferCapability ();

    void
    TransferCapability (const TransferCapabilitySequence& s);

    // ApplicationCluster
    //
    typedef ::CDM::LocalizedText ApplicationClusterType;
    typedef ::xsd::cxx::tree::sequence< ApplicationClusterType > ApplicationClusterSequence;
    typedef ApplicationClusterSequence::iterator ApplicationClusterIterator;
    typedef ApplicationClusterSequence::const_iterator ApplicationClusterConstIterator;
    typedef ::xsd::cxx::tree::traits< ApplicationClusterType, char > ApplicationClusterTraits;

    const ApplicationClusterSequence&
    ApplicationCluster () const;

    ApplicationClusterSequence&
    ApplicationCluster ();

    void
    ApplicationCluster (const ApplicationClusterSequence& s);

    // SupportedCharacterSet
    //
    typedef ::xml_schema::String SupportedCharacterSetType;
    typedef ::xsd::cxx::tree::sequence< SupportedCharacterSetType > SupportedCharacterSetSequence;
    typedef SupportedCharacterSetSequence::iterator SupportedCharacterSetIterator;
    typedef SupportedCharacterSetSequence::const_iterator SupportedCharacterSetConstIterator;
    typedef ::xsd::cxx::tree::traits< SupportedCharacterSetType, char > SupportedCharacterSetTraits;

    const SupportedCharacterSetSequence&
    SupportedCharacterSet () const;

    SupportedCharacterSetSequence&
    SupportedCharacterSet ();

    void
    SupportedCharacterSet (const SupportedCharacterSetSequence& s);

    // Description
    //
    typedef ::CDM::LocalizedText DescriptionType;
    typedef ::xsd::cxx::tree::sequence< DescriptionType > DescriptionSequence;
    typedef DescriptionSequence::iterator DescriptionIterator;
    typedef DescriptionSequence::const_iterator DescriptionConstIterator;
    typedef ::xsd::cxx::tree::traits< DescriptionType, char > DescriptionTraits;

    const DescriptionSequence&
    Description () const;

    DescriptionSequence&
    Description ();

    void
    Description (const DescriptionSequence& s);

    // AeTitle
    //
    typedef ::xml_schema::String AeTitleType;
    typedef ::xsd::cxx::tree::traits< AeTitleType, char > AeTitleTraits;

    const AeTitleType&
    AeTitle () const;

    AeTitleType&
    AeTitle ();

    void
    AeTitle (const AeTitleType& x);

    void
    AeTitle (::std::unique_ptr< AeTitleType > p);

    // NetworkConnectionReference
    //
    typedef ::xml_schema::Idrefs NetworkConnectionReferenceType;
    typedef ::xsd::cxx::tree::traits< NetworkConnectionReferenceType, char > NetworkConnectionReferenceTraits;

    const NetworkConnectionReferenceType&
    NetworkConnectionReference () const;

    NetworkConnectionReferenceType&
    NetworkConnectionReference ();

    void
    NetworkConnectionReference (const NetworkConnectionReferenceType& x);

    void
    NetworkConnectionReference (::std::unique_ptr< NetworkConnectionReferenceType > p);

    // AssociationInitiator
    //
    typedef ::xml_schema::Boolean AssociationInitiatorType;
    typedef ::xsd::cxx::tree::traits< AssociationInitiatorType, char > AssociationInitiatorTraits;

    const AssociationInitiatorType&
    AssociationInitiator () const;

    AssociationInitiatorType&
    AssociationInitiator ();

    void
    AssociationInitiator (const AssociationInitiatorType& x);

    // AssociationAcceptor
    //
    typedef ::xml_schema::Boolean AssociationAcceptorType;
    typedef ::xsd::cxx::tree::traits< AssociationAcceptorType, char > AssociationAcceptorTraits;

    const AssociationAcceptorType&
    AssociationAcceptor () const;

    AssociationAcceptorType&
    AssociationAcceptor ();

    void
    AssociationAcceptor (const AssociationAcceptorType& x);

    // Constructors.
    //
    DicomNetworkAe (const AeTitleType&,
                    const NetworkConnectionReferenceType&,
                    const AssociationInitiatorType&,
                    const AssociationAcceptorType&);

    DicomNetworkAe (const ::xercesc::DOMElement& e,
                    ::xml_schema::Flags f = 0,
                    ::xml_schema::Container* c = 0);

    DicomNetworkAe (const DicomNetworkAe& x,
                    ::xml_schema::Flags f = 0,
                    ::xml_schema::Container* c = 0);

    virtual DicomNetworkAe*
    _clone (::xml_schema::Flags f = 0,
            ::xml_schema::Container* c = 0) const;

    DicomNetworkAe&
    operator= (const DicomNetworkAe& x);

    virtual 
    ~DicomNetworkAe ();

    // Implementation.
    //
    protected:
    void
    parse (::xsd::cxx::xml::dom::parser< char >&,
           ::xml_schema::Flags);

    protected:
    TransferCapabilitySequence TransferCapability_;
    ApplicationClusterSequence ApplicationCluster_;
    SupportedCharacterSetSequence SupportedCharacterSet_;
    DescriptionSequence Description_;
    ::xsd::cxx::tree::one< AeTitleType > AeTitle_;
    ::xsd::cxx::tree::one< NetworkConnectionReferenceType > NetworkConnectionReference_;
    ::xsd::cxx::tree::one< AssociationInitiatorType > AssociationInitiator_;
    ::xsd::cxx::tree::one< AssociationAcceptorType > AssociationAcceptor_;
  };

  class DicomNetworkConnection: public ::xml_schema::Type
  {
    public:
    // TlsCipherSuite
    //
    typedef ::xml_schema::String TlsCipherSuiteType;
    typedef ::xsd::cxx::tree::sequence< TlsCipherSuiteType > TlsCipherSuiteSequence;
    typedef TlsCipherSuiteSequence::iterator TlsCipherSuiteIterator;
    typedef TlsCipherSuiteSequence::const_iterator TlsCipherSuiteConstIterator;
    typedef ::xsd::cxx::tree::traits< TlsCipherSuiteType, char > TlsCipherSuiteTraits;

    const TlsCipherSuiteSequence&
    TlsCipherSuite () const;

    TlsCipherSuiteSequence&
    TlsCipherSuite ();

    void
    TlsCipherSuite (const TlsCipherSuiteSequence& s);

    // id
    //
    typedef ::xml_schema::Id IdType;
    typedef ::xsd::cxx::tree::traits< IdType, char > IdTraits;

    const IdType&
    id () const;

    IdType&
    id ();

    void
    id (const IdType& x);

    void
    id (::std::unique_ptr< IdType > p);

    // Hostname
    //
    typedef ::xml_schema::String HostnameType;
    typedef ::xsd::cxx::tree::traits< HostnameType, char > HostnameTraits;

    const HostnameType&
    Hostname () const;

    HostnameType&
    Hostname ();

    void
    Hostname (const HostnameType& x);

    void
    Hostname (::std::unique_ptr< HostnameType > p);

    // Port
    //
    typedef ::xml_schema::UnsignedShort PortType;
    typedef ::xsd::cxx::tree::optional< PortType > PortOptional;
    typedef ::xsd::cxx::tree::traits< PortType, char > PortTraits;

    const PortOptional&
    Port () const;

    PortOptional&
    Port ();

    void
    Port (const PortType& x);

    void
    Port (const PortOptional& x);

    // Constructors.
    //
    DicomNetworkConnection (const IdType&,
                            const HostnameType&);

    DicomNetworkConnection (const ::xercesc::DOMElement& e,
                            ::xml_schema::Flags f = 0,
                            ::xml_schema::Container* c = 0);

    DicomNetworkConnection (const DicomNetworkConnection& x,
                            ::xml_schema::Flags f = 0,
                            ::xml_schema::Container* c = 0);

    virtual DicomNetworkConnection*
    _clone (::xml_schema::Flags f = 0,
            ::xml_schema::Container* c = 0) const;

    DicomNetworkConnection&
    operator= (const DicomNetworkConnection& x);

    virtual 
    ~DicomNetworkConnection ();

    // Implementation.
    //
    protected:
    void
    parse (::xsd::cxx::xml::dom::parser< char >&,
           ::xml_schema::Flags);

    protected:
    TlsCipherSuiteSequence TlsCipherSuite_;
    ::xsd::cxx::tree::one< IdType > id_;
    ::xsd::cxx::tree::one< HostnameType > Hostname_;
    PortOptional Port_;
  };

  class DicomDeviceDescriptor: public ::CDM::AbstractComplexDeviceComponentDescriptor
  {
    public:
    // NetworkAe
    //
    typedef ::CDM::DicomNetworkAe NetworkAeType;
    typedef ::xsd::cxx::tree::sequence< NetworkAeType > NetworkAeSequence;
    typedef NetworkAeSequence::iterator NetworkAeIterator;
    typedef NetworkAeSequence::const_iterator NetworkAeConstIterator;
    typedef ::xsd::cxx::tree::traits< NetworkAeType, char > NetworkAeTraits;

    const NetworkAeSequence&
    NetworkAe () const;

    NetworkAeSequence&
    NetworkAe ();

    void
    NetworkAe (const NetworkAeSequence& s);

    // NetworkConnection
    //
    typedef ::CDM::DicomNetworkConnection NetworkConnectionType;
    typedef ::xsd::cxx::tree::sequence< NetworkConnectionType > NetworkConnectionSequence;
    typedef NetworkConnectionSequence::iterator NetworkConnectionIterator;
    typedef NetworkConnectionSequence::const_iterator NetworkConnectionConstIterator;
    typedef ::xsd::cxx::tree::traits< NetworkConnectionType, char > NetworkConnectionTraits;

    const NetworkConnectionSequence&
    NetworkConnection () const;

    NetworkConnectionSequence&
    NetworkConnection ();

    void
    NetworkConnection (const NetworkConnectionSequence& s);

    // SoftwareVersion
    //
    typedef ::xml_schema::String SoftwareVersionType;
    typedef ::xsd::cxx::tree::sequence< SoftwareVersionType > SoftwareVersionSequence;
    typedef SoftwareVersionSequence::iterator SoftwareVersionIterator;
    typedef SoftwareVersionSequence::const_iterator SoftwareVersionConstIterator;
    typedef ::xsd::cxx::tree::traits< SoftwareVersionType, char > SoftwareVersionTraits;

    const SoftwareVersionSequence&
    SoftwareVersion () const;

    SoftwareVersionSequence&
    SoftwareVersion ();

    void
    SoftwareVersion (const SoftwareVersionSequence& s);

    // InstitutionName
    //
    typedef ::CDM::LocalizedText InstitutionNameType;
    typedef ::xsd::cxx::tree::sequence< InstitutionNameType > InstitutionNameSequence;
    typedef InstitutionNameSequence::iterator InstitutionNameIterator;
    typedef InstitutionNameSequence::const_iterator InstitutionNameConstIterator;
    typedef ::xsd::cxx::tree::traits< InstitutionNameType, char > InstitutionNameTraits;

    const InstitutionNameSequence&
    InstitutionName () const;

    InstitutionNameSequence&
    InstitutionName ();

    void
    InstitutionName (const InstitutionNameSequence& s);

    // InstitutionAddress
    //
    typedef ::CDM::LocalizedText InstitutionAddressType;
    typedef ::xsd::cxx::tree::sequence< InstitutionAddressType > InstitutionAddressSequence;
    typedef InstitutionAddressSequence::iterator InstitutionAddressIterator;
    typedef InstitutionAddressSequence::const_iterator InstitutionAddressConstIterator;
    typedef ::xsd::cxx::tree::traits< InstitutionAddressType, char > InstitutionAddressTraits;

    const InstitutionAddressSequence&
    InstitutionAddress () const;

    InstitutionAddressSequence&
    InstitutionAddress ();

    void
    InstitutionAddress (const InstitutionAddressSequence& s);

    // InstitutionalDepartmentName
    //
    typedef ::CDM::LocalizedText InstitutionalDepartmentNameType;
    typedef ::xsd::cxx::tree::sequence< InstitutionalDepartmentNameType > InstitutionalDepartmentNameSequence;
    typedef InstitutionalDepartmentNameSequence::iterator InstitutionalDepartmentNameIterator;
    typedef InstitutionalDepartmentNameSequence::const_iterator InstitutionalDepartmentNameConstIterator;
    typedef ::xsd::cxx::tree::traits< InstitutionalDepartmentNameType, char > InstitutionalDepartmentNameTraits;

    const InstitutionalDepartmentNameSequence&
    InstitutionalDepartmentName () const;

    InstitutionalDepartmentNameSequence&
    InstitutionalDepartmentName ();

    void
    InstitutionalDepartmentName (const InstitutionalDepartmentNameSequence& s);

    // PrimaryDeviceType
    //
    typedef ::CDM::LocalizedText PrimaryDeviceTypeType;
    typedef ::xsd::cxx::tree::sequence< PrimaryDeviceTypeType > PrimaryDeviceTypeSequence;
    typedef PrimaryDeviceTypeSequence::iterator PrimaryDeviceTypeIterator;
    typedef PrimaryDeviceTypeSequence::const_iterator PrimaryDeviceTypeConstIterator;
    typedef ::xsd::cxx::tree::traits< PrimaryDeviceTypeType, char > PrimaryDeviceTypeTraits;

    const PrimaryDeviceTypeSequence&
    PrimaryDeviceType () const;

    PrimaryDeviceTypeSequence&
    PrimaryDeviceType ();

    void
    PrimaryDeviceType (const PrimaryDeviceTypeSequence& s);

    // PublicCertificate
    //
    typedef ::xml_schema::Base64Binary PublicCertificateType;
    typedef ::xsd::cxx::tree::sequence< PublicCertificateType > PublicCertificateSequence;
    typedef PublicCertificateSequence::iterator PublicCertificateIterator;
    typedef PublicCertificateSequence::const_iterator PublicCertificateConstIterator;
    typedef ::xsd::cxx::tree::traits< PublicCertificateType, char > PublicCertificateTraits;

    const PublicCertificateSequence&
    PublicCertificate () const;

    PublicCertificateSequence&
    PublicCertificate ();

    void
    PublicCertificate (const PublicCertificateSequence& s);

    // Description
    //
    typedef ::CDM::LocalizedText DescriptionType;
    typedef ::xsd::cxx::tree::sequence< DescriptionType > DescriptionSequence;
    typedef DescriptionSequence::iterator DescriptionIterator;
    typedef DescriptionSequence::const_iterator DescriptionConstIterator;
    typedef ::xsd::cxx::tree::traits< DescriptionType, char > DescriptionTraits;

    const DescriptionSequence&
    Description () const;

    DescriptionSequence&
    Description ();

    void
    Description (const DescriptionSequence& s);

    // StationName
    //
    typedef ::CDM::LocalizedText StationNameType;
    typedef ::xsd::cxx::tree::sequence< StationNameType > StationNameSequence;
    typedef StationNameSequence::iterator StationNameIterator;
    typedef StationNameSequence::const_iterator StationNameConstIterator;
    typedef ::xsd::cxx::tree::traits< StationNameType, char > StationNameTraits;

    const StationNameSequence&
    StationName () const;

    StationNameSequence&
    StationName ();

    void
    StationName (const StationNameSequence& s);

    // IssuerOfPatientID
    //
    typedef ::xml_schema::String IssuerOfPatientIDType;
    typedef ::xsd::cxx::tree::optional< IssuerOfPatientIDType > IssuerOfPatientIDOptional;
    typedef ::xsd::cxx::tree::traits< IssuerOfPatientIDType, char > IssuerOfPatientIDTraits;

    const IssuerOfPatientIDOptional&
    IssuerOfPatientID () const;

    IssuerOfPatientIDOptional&
    IssuerOfPatientID ();

    void
    IssuerOfPatientID (const IssuerOfPatientIDType& x);

    void
    IssuerOfPatientID (const IssuerOfPatientIDOptional& x);

    void
    IssuerOfPatientID (::std::unique_ptr< IssuerOfPatientIDType > p);

    // Constructors.
    //
    DicomDeviceDescriptor (const HandleType&);

    DicomDeviceDescriptor (const ::xercesc::DOMElement& e,
                           ::xml_schema::Flags f = 0,
                           ::xml_schema::Container* c = 0);

    DicomDeviceDescriptor (const DicomDeviceDescriptor& x,
                           ::xml_schema::Flags f = 0,
                           ::xml_schema::Container* c = 0);

    virtual DicomDeviceDescriptor*
    _clone (::xml_schema::Flags f = 0,
            ::xml_schema::Container* c = 0) const;

    DicomDeviceDescriptor&
    operator= (const DicomDeviceDescriptor& x);

    virtual 
    ~DicomDeviceDescriptor ();

    // Implementation.
    //
    protected:
    void
    parse (::xsd::cxx::xml::dom::parser< char >&,
           ::xml_schema::Flags);

    protected:
    NetworkAeSequence NetworkAe_;
    NetworkConnectionSequence NetworkConnection_;
    SoftwareVersionSequence SoftwareVersion_;
    InstitutionNameSequence InstitutionName_;
    InstitutionAddressSequence InstitutionAddress_;
    InstitutionalDepartmentNameSequence InstitutionalDepartmentName_;
    PrimaryDeviceTypeSequence PrimaryDeviceType_;
    PublicCertificateSequence PublicCertificate_;
    DescriptionSequence Description_;
    StationNameSequence StationName_;
    IssuerOfPatientIDOptional IssuerOfPatientID_;
  };
}

#include <iosfwd>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>

namespace CDM
{
}

#include <iosfwd>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

namespace CDM
{
  void
  operator<< (::xercesc::DOMElement&, const DicomTransferRole&);

  void
  operator<< (::xercesc::DOMAttr&, const DicomTransferRole&);

  void
  operator<< (::xml_schema::ListStream&,
              const DicomTransferRole&);

  void
  operator<< (::xercesc::DOMElement&, const DicomTransferCapability&);

  void
  operator<< (::xercesc::DOMElement&, const DicomNetworkAe&);

  void
  operator<< (::xercesc::DOMElement&, const DicomNetworkConnection&);

  void
  operator<< (::xercesc::DOMElement&, const DicomDeviceDescriptor&);
}

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

#endif // DICOMDEVICE_DESCRIPTION_HXX
