/*
 *  Copyright 2009 Utkin Dmitry
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
 
/* 
 *  This file is part of the WSF Staff project.
 *  Please, visit http://code.google.com/p/staff for more information.
 */

// This file generated by staff_codegen
// Service Implementation

#ifndef _WidgetManagerImpl_h_
#define _WidgetManagerImpl_h_

#include "WidgetManager.h"
#include <rise/xml/XMLDocument.h>

namespace widget
{
  class CWidgetManagerImpl: public CWidgetManager
  {
  public:
    CWidgetManagerImpl();
    virtual ~CWidgetManagerImpl();
    
    virtual void Open(const std::string& sProfile);
    virtual void Close();
    virtual void Commit();

    virtual TStringMap GetWidgetClasses();

    virtual TWidgetMap GetActiveWidgets() const;
    virtual void AddWidget(const SWidget& rWidget);
    virtual void AddWidgets(const TWidgetMap& rWidgets);
    virtual void DeleteWidget(const std::string& sId);
    virtual void DeleteWidgets(const TStringList& lsIds);
    virtual void AlterWidget(const SWidget& rWidget);

    virtual TStringMap GetAvailableWidgetGroups() const;
    virtual TWidgetGroupMap GetWidgetGroups(const TStringList& lsWidgetGroups) const;

    virtual void AddWidgetGroup(const SWidgetGroup& rWidgetGroup);
    virtual void DeleteWidgetGroup(const std::string& sGroupId);
    virtual void AlterWidgetGroup(const SWidgetGroup& rWidgetGroup);

    virtual TStringList GetActiveWidgetGroups() const;
    virtual void SetActiveWidgetGroups(const TStringList& lsActiveWidgetGroups);


  private:
    struct SWidgetInternal;
    typedef std::map<std::string, SWidgetInternal> TWidgetInternalMap;
    struct SWidgetGroupInternal;
    typedef std::map<std::string, SWidgetGroupInternal> TWidgetGroupInternalMap;

  private:
    virtual void LoadWidgetClasses(const std::string& sProfile);
    const std::string& GetSessionId();
    int GetUserId();

    void LoadWidgets(const rise::xml::CXMLNode& rNodeWidgets, TWidgetInternalMap& mWidgets);
    
  private:
    TStringMap m_mWidgetClasses;
    TWidgetInternalMap m_mActiveWidgets;
    TStringList m_lsActiveWidgetGroups;
    TWidgetGroupInternalMap m_mWidgetGroups;
    rise::xml::CXMLDocument m_tDoc;
    std::string m_sUserDbFileName;
    std::string m_sProfile;
    std::string m_sSessionId;
    static std::string m_sComponentHome;
    int m_nUserId;
  };
}


#endif // _WidgetManagerImpl_h_
