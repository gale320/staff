// This file generated by staff_codegen from wsdl
// For more information please visit: http://code.google.com/p/staff/

#ifndef _$(Interface.Name)_h_
#define _$(Interface.Name)_h_

#include <staff/common/WsdlTypes.h>
#include <staff/common/IService.h>

// targetNamespace: $(Interface.TargetNamespace)
#ifneq($(Interface.Structs.$Count),0)

#foreach $(Interface.Structs)
#ifneq($(Struct.NativeName),DataObject)
struct $(Struct.Name);
#else
\
#ifeqend
#end
#else
\
#ifeqend
#ifneq($(Interface.Typedefs.$Count),0)

#foreach $(Interface.Typedefs)
typedef $(Typedef.DataType) $(Typedef.Name);
#end

#else
\
#ifeqend
#ifneq($(Interface.Typedefs.$Count),0)

#foreach $(Interface.Structs)
struct $(Struct.Name)$(Struct.ParentDecl)
{
#foreach $(Struct.Members)
  $(Param.DataType) $(Param.Name);
#end
};
#end
#else
\
#ifeqend
#foreach $(Interface.Classes)
$(Class.OpeningNs)
#ifneq($(Class.Description),)
  /*! $(Class.Description) */
#else
  //!  service $(Class.ServiceNsName)
#ifeqend
  // uri: $(Class.ServiceUri)
  class $(Class.Name): public staff::IService
  {
  public:
    virtual ~$(Class.Name)() {}
#foreach $(Class.Members)

#ifneq($(Member.Description),)
    /*! $(Member.Description) */
#else
\
#ifeqend
#ifneq($(Member.SoapAction),)
    // soapAction: $(Member.SoapAction)
#else
\
#ifeqend
#ifneq($(Member.NodeName),)
    // requestElement: $(Member.NodeName)
#else
\
#ifeqend
#ifneq($(Member.Return.ResponseName),)
    // responseElement: $(Member.Return.ResponseName)
#else
\
#ifeqend
#ifneq($(Member.Return.NodeName),)
    // resultElement: $(Member.Return.NodeName)
#else
\
#ifeqend
    virtual $(Member.Return) $(Member.Name)($(Member.Params))$(Member.Const) = 0;
#end
  };

$(Class.EndingNs)
#end
#endif // _$(Interface.Name)_h_
