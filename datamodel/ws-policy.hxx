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

#ifndef WS_POLICY_HXX
#define WS_POLICY_HXX

#include "ws-policy-fwd.hxx"

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

#include "ws-streaming.hxx"

namespace WS
{
  namespace POLICY
  {
    class Policy: public ::xml_schema::Type
    {
      public:
      // StreamSource
      //
      typedef ::WS::STREAMING::StreamSource StreamSourceType;
      typedef ::xsd::cxx::tree::traits< StreamSourceType, char > StreamSourceTraits;

      const StreamSourceType&
      StreamSource () const;

      StreamSourceType&
      StreamSource ();

      void
      StreamSource (const StreamSourceType& x);

      void
      StreamSource (::std::unique_ptr< StreamSourceType > p);

      // Constructors.
      //
      Policy (const StreamSourceType&);

      Policy (::std::unique_ptr< StreamSourceType >);

      Policy (const ::xercesc::DOMElement& e,
              ::xml_schema::Flags f = 0,
              ::xml_schema::Container* c = 0);

      Policy (const Policy& x,
              ::xml_schema::Flags f = 0,
              ::xml_schema::Container* c = 0);

      virtual Policy*
      _clone (::xml_schema::Flags f = 0,
              ::xml_schema::Container* c = 0) const;

      Policy&
      operator= (const Policy& x);

      virtual 
      ~Policy ();

      // Implementation.
      //
      protected:
      void
      parse (::xsd::cxx::xml::dom::parser< char >&,
             ::xml_schema::Flags);

      protected:
      ::xsd::cxx::tree::one< StreamSourceType > StreamSource_;
    };
  }
}

#include <iosfwd>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>

namespace WS
{
  namespace POLICY
  {
    // Parse a URI or a local file.
    //

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (const ::std::string& uri,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (const ::std::string& uri,
             ::xml_schema::ErrorHandler& eh,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (const ::std::string& uri,
             ::xercesc::DOMErrorHandler& eh,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());

    // Parse std::istream.
    //

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (::std::istream& is,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (::std::istream& is,
             ::xml_schema::ErrorHandler& eh,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (::std::istream& is,
             ::xercesc::DOMErrorHandler& eh,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (::std::istream& is,
             const ::std::string& id,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (::std::istream& is,
             const ::std::string& id,
             ::xml_schema::ErrorHandler& eh,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (::std::istream& is,
             const ::std::string& id,
             ::xercesc::DOMErrorHandler& eh,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());

    // Parse xercesc::InputSource.
    //

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (::xercesc::InputSource& is,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (::xercesc::InputSource& is,
             ::xml_schema::ErrorHandler& eh,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (::xercesc::InputSource& is,
             ::xercesc::DOMErrorHandler& eh,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());

    // Parse xercesc::DOMDocument.
    //

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (const ::xercesc::DOMDocument& d,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());

    ::std::unique_ptr< ::WS::POLICY::Policy >
    Policy_ (::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument > d,
             ::xml_schema::Flags f = 0,
             const ::xml_schema::Properties& p = ::xml_schema::Properties ());
  }
}

#include <iosfwd>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

namespace WS
{
  namespace POLICY
  {
    // Serialize to std::ostream.
    //

    void
    Policy_ (::std::ostream& os,
             const ::WS::POLICY::Policy& x, 
             const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
             const ::std::string& e = "UTF-8",
             ::xml_schema::Flags f = 0);

    void
    Policy_ (::std::ostream& os,
             const ::WS::POLICY::Policy& x, 
             ::xml_schema::ErrorHandler& eh,
             const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
             const ::std::string& e = "UTF-8",
             ::xml_schema::Flags f = 0);

    void
    Policy_ (::std::ostream& os,
             const ::WS::POLICY::Policy& x, 
             ::xercesc::DOMErrorHandler& eh,
             const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
             const ::std::string& e = "UTF-8",
             ::xml_schema::Flags f = 0);

    // Serialize to xercesc::XMLFormatTarget.
    //

    void
    Policy_ (::xercesc::XMLFormatTarget& ft,
             const ::WS::POLICY::Policy& x, 
             const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
             const ::std::string& e = "UTF-8",
             ::xml_schema::Flags f = 0);

    void
    Policy_ (::xercesc::XMLFormatTarget& ft,
             const ::WS::POLICY::Policy& x, 
             ::xml_schema::ErrorHandler& eh,
             const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
             const ::std::string& e = "UTF-8",
             ::xml_schema::Flags f = 0);

    void
    Policy_ (::xercesc::XMLFormatTarget& ft,
             const ::WS::POLICY::Policy& x, 
             ::xercesc::DOMErrorHandler& eh,
             const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
             const ::std::string& e = "UTF-8",
             ::xml_schema::Flags f = 0);

    // Serialize to an existing xercesc::DOMDocument.
    //

    void
    Policy_ (::xercesc::DOMDocument& d,
             const ::WS::POLICY::Policy& x,
             ::xml_schema::Flags f = 0);

    // Serialize to a new xercesc::DOMDocument.
    //

    ::xml_schema::dom::unique_ptr< ::xercesc::DOMDocument >
    Policy_ (const ::WS::POLICY::Policy& x, 
             const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
             ::xml_schema::Flags f = 0);

    void
    operator<< (::xercesc::DOMElement&, const Policy&);
  }
}

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

#endif // WS_POLICY_HXX
