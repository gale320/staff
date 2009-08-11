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

#ifndef _$(Interface.Name)Wrapper_h_
#define _$(Interface.Name)Wrapper_h_

#include <staff/service/ServiceWrapper.h>

#foreach $(Interface.Classes)
$(Class.OpeningNs)
  class $(Class.Name)Impl;

  class $(Class.Name)Wrapper: public staff::CServiceWrapper
  {
  public:
    $(Class.Name)Wrapper();
    virtual ~$(Class.Name)Wrapper();

    virtual void Invoke(staff::COperation& rOperation);

  protected:
    $(Class.Name)Impl& ServiceImpl();
  private:
    $(Class.Name)Impl* m_pServiceImpl;
  };
$(Class.EndingNs)

#end
#endif // _$(Interface.Name)Wrapper_h_
