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
// DO NOT EDIT

#ifndef _COMPONENTIMPL_H_
#define _COMPONENTIMPL_H_

#include <staff/component/Component.h>

$(Project.Interfaces.Interface.Classes.Class.OpeningNs)
  //!  Component implementation
  class CComponentImpl: public staff::CComponent
  {
  public:
    //!        default constructor
    CComponentImpl();

    //!        destructor
    virtual ~CComponentImpl();

    //!         get component name
    /*! \return component name
    */
    const rise::CString& GetName() const;

    //!         get service
    /*! \param  sService - service name
        \return pointer to work with service, NULL if no service found
        */
    const staff::CService* GetService(const rise::CString& sService) const;

    //!         get service
    /*! \param  sService - service name
        \return pointer to work with service, NULL if no service found
        */
    staff::CService* GetService(const rise::CString& sService);

    //!         get services map
    /*! \return services map
    */
    const staff::TServiceMap& GetServices() const;

  private:
    staff::TServiceMap m_mServices;
    static const rise::CString m_sName;
  };
$(Project.Interfaces.Interface.Classes.Class.EndingNs)

#endif // _COMPONENTIMPL_H_
