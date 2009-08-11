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

#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include <rise/string/String.h>
#include <staff/common/DataObject.h>

namespace samples
{
  //! sample service: Calculator
  class CCalculator
  {
  public:
    virtual ~CCalculator() {}

    //! addition
    /*! \param nA - first operand
        \param nB - second operand
        \return nA + nB
        */
    virtual int Add(int nA, int nB) = 0;

    //! substraction
    /*! \param nA - first operand
        \param nB - second operand
        \return nA - nB
        */
    virtual int Sub(int nA, int nB) = 0;

    //! multiply operation: example of using dataobject for complex datatypes/xml transmittion
    /*! \param rDataObject - reference to dataobject, containing operands nA and nB
        \return nA * nB
        */
    virtual staff::CDataObject MulDO(const staff::CDataObject& rDataObject) = 0;
  };
}

#endif // _CALCULATOR_H_
