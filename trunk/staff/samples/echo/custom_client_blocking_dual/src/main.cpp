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

#include <rise/common/Log.h>
#include <staff/common/DataObject.h>
#include <staff/common/Namespace.h>
#include <staff/common/Exception.h>
#include <staff/client/Options.h>
#include <staff/client/ServiceClient.h>

int main(int nArgs, const char* paszArgs[])
{
  try
  {
    staff::CServiceClient tClient;

    // initialize client
    tClient.Init();

    // client options
    staff::COptions& rOptions = tClient.GetOptions();

    // set namespace for each request
    rOptions.SetDefaultNamespace("http://ws.apache.org/axis2/services/echo", "ns1");

    // engage addressing module
    tClient.EngageModule("addressing");

    // set wsa:To
    rOptions.SetToAddress("http://localhost:9090/axis2/services/echo");

    // set wsa Use separate listener
    rOptions.UseSeparateListener();

    // set wsa:Action
    rOptions.SetAction("http://ws.apache.org/axis2/c/samples/echoString");

    // set wsa:ReplyTo
    rOptions.SetReplyToAddress("http://localhost:6060/axis2/services/__ANONYMOUS_SERVICE__/__OPERATION_OUT_IN__");


    // build payload
    staff::CDataObject tdoPayload("echoString");
    tdoPayload.CreateChild("text").SetText("Hello World!");

    // output request
    std::cout << "Request: \n-------\n" << tdoPayload.ToString() << "\n------\n";

    // invoke service synchronously
    staff::CDataObject tdoResult = tClient.Invoke(tdoPayload);

    // output result
    std::cout << "Result: \n-------\n" << tdoResult.ToString() << "\n------\n";
  }
  RISE_CATCH_ALL

  return 0;
}

