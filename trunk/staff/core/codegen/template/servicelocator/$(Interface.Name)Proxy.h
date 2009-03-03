// This file generated by staff_codegen
// DO NOT EDIT

#ifndef _$(Interface.Name)Proxy_h_
#define _$(Interface.Name)Proxy_h_

#include "$(Interface.FileName)"

namespace staff { class CService; }
#ifneq("$(Interface.Namespace)","")
namespace $(Interface.Namespace)
{
#ifeqend
#foreach $(Interface.Classes)
class $(Class.Name)Proxy: public $(Class.Name)
{
public:
  $(Class.Name)Proxy(staff::CService* pService, const rise::CString& sID);
  void SetService(staff::CService* pService);
#foreach $(Class.Members)
  $(Member.Return) $(Member.Name)($(Member.Params))$(Member.Const);
#end

private:
  mutable staff::CService* m_pService;
  const rise::CString m_sID;
};

#end
#ifneq("$(Interface.Namespace)","")
}

#ifeqend
#endif