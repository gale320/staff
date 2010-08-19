/*
 *  Copyright 2010 Utkin Dmitry
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

#include <axiom.h>
#include <axis2_client.h>
#include <axis2_defines.h>
#include <axis2_callback.h>
#include <axutil_env.h>
#include <axutil_string.h>
#include <axiom_soap_const.h>
#include <string>
#include <rise/common/console.h>
#include <rise/common/Log.h>
#include <rise/common/ExceptionTemplate.h>
#include <staff/common/Exception.h>
#include <staff/common/Runtime.h>
#include <staff/common/QName.h>
#include <staff/common/Namespace.h>
#include <staff/common/Operation.h>
#include <staff/common/DataObject.h>
#include "ICallback.h"
#include "Options.h"
#include "ServiceClient.h"

namespace staff
{
  class CCallbackWrapper
  {
  public:
    CCallbackWrapper(ICallback<const CDataObject&>* pCallback, bool bOwner):
      m_pCallback(pCallback), m_bOwner(bOwner)
    {
    }

    ~CCallbackWrapper()
    {
      if (m_bOwner)
      {
        delete m_pCallback;
      }
    }

    ICallback<const CDataObject&>* Get()
    {
      return m_pCallback;
    }

  private:
    ICallback<const CDataObject&>* m_pCallback;
    bool m_bOwner;
  };

  class CServiceClientTools
  {
  public:
    static axis2_status_t AXIS2_CALL CallbackOnComplete(axis2_callback_t* pAxis2Callback, const axutil_env_t* pEnv)
    {
      if (!pAxis2Callback)
      {
        rise::LogError() << "pCallback is NULL";
        return AXIS2_FAILURE;
      }

      std::auto_ptr<CCallbackWrapper> tpCallbackWrapper(
          reinterpret_cast<CCallbackWrapper*>(axis2_callback_get_data(pAxis2Callback)));

      if (!tpCallbackWrapper.get())
      {
        rise::LogError() << "pointer to CallbackWrapper is NULL";
        return AXIS2_FAILURE;
      }

      axis2_callback_set_data(pAxis2Callback, NULL);  // avoid axis2/c to destroy C++ data

      ICallback<const CDataObject&>* pCallback = tpCallbackWrapper->Get();

      if (!pCallback)
      {
        rise::LogError() << "pointer to ICallback is NULL";
        return AXIS2_FAILURE;
      }

      axiom_soap_envelope_t* pSoapEnvelope = axis2_callback_get_envelope(pAxis2Callback, pEnv);
      if (!pSoapEnvelope)
      {
        CreateFault(*pCallback, "client", "Failed to get soap envelope while parsing response");
        return AXIS2_FAILURE;
      }

      axiom_soap_body_t* pSoapBody = axiom_soap_envelope_get_body(pSoapEnvelope, pEnv);
      if (!pSoapBody)
      {
        CreateFault(*pCallback, "client", "Failed to get soap body while parsing response");
        return AXIS2_FAILURE;
      }

      axiom_node_t* pAxiomBaseNode = axiom_soap_body_get_base_node(pSoapBody, pEnv);
      if (!pAxiomBaseNode)
      {
        CreateFault(*pCallback, "client", "Failed to get base node while parsing response");
        return AXIS2_FAILURE;
      }

      axiom_node_t* pAxiomResponseNode = axiom_node_get_first_element(pAxiomBaseNode, pEnv);
      if (!pAxiomResponseNode)
      {
        CreateFault(*pCallback, "client", "Failed to get response node while parsing response");
        return AXIS2_FAILURE;
      }

#ifdef _DEBUG
      rise::LogDebug2() << "Received Response: \n" << rise::ColorInkBlue
          << CDataObject(pAxiomResponseNode).ToString() << rise::ColorDefault;
#endif

      axiom_element_t* pAxiomResponseElement =
          reinterpret_cast<axiom_element_t*>(axiom_node_get_data_element(pAxiomResponseNode, pEnv));
      if (!pAxiomResponseElement)
      {
        CreateFault(*pCallback, "client", "Failed to get data element while parsing response");
        return AXIS2_FAILURE;
      }

      // is soap Fault?
      if (axutil_strcmp(axiom_element_get_localname(pAxiomResponseElement, pEnv), "Fault") == 0)
      {
        axiom_namespace_t* pNamespace =
            axiom_element_get_namespace(pAxiomResponseElement, pEnv, pAxiomResponseNode);
        if (pNamespace)
        {
          const axis2_char_t* szNamespaceUri = axiom_namespace_get_uri(pNamespace, pEnv);

          if(axutil_strcmp(szNamespaceUri, "http://schemas.xmlsoap.org/soap/envelope/") == 0)
          {
            try
            {
              pCallback->OnFault(pAxiomResponseNode);
            }
            RISE_CATCH_ALL_DESCR("Error while processing fault response");

            return AXIS2_SUCCESS;
          }
        }
      }

      try
      {
        pCallback->OnComplete(pAxiomResponseNode);
      }
      RISE_CATCH_ALL_DESCR("Error while processing response");

      return AXIS2_SUCCESS;
    }

    static axis2_status_t AXIS2_CALL CallbackOnFault(axis2_callback_t* pCallback, const axutil_env_t* pEnv,
                                                     int nFaultCode)
    {
      if (!pCallback)
      {
        rise::LogError() << "pCallback is NULL";
        return AXIS2_FAILURE;
      }

      PICallback tpCallback(reinterpret_cast<ICallback<const CDataObject&>*>(axis2_callback_get_data(pCallback)));
      axis2_callback_set_data(pCallback, NULL);  // avoid axis2/c to destroy C++ data

      if (!tpCallback.get())
      {
        rise::LogError() << "pointer to ICallback is NULL";
        return AXIS2_FAILURE;
      }

      axiom_node_t* pAxiomResponseNode = NULL;
      {
        axiom_soap_envelope_t* pSoapEnvelope = axis2_callback_get_envelope(pCallback, pEnv);
        if (pSoapEnvelope)
        {
          pAxiomResponseNode = axiom_soap_envelope_get_base_node(pSoapEnvelope, pEnv);
#ifdef _DEBUG
      rise::LogDebug2() << "Received Response: \n" << rise::ColorInkBlue
          << CDataObject(pAxiomResponseNode).ToString() << rise::ColorDefault;
#endif
        }
      }

      try
      {
        if (!pAxiomResponseNode)
        {
          CreateFault(*tpCallback, AXIS2_ERROR_GET_MESSAGE(pEnv->error), rise::ToStr(nFaultCode));
        }
        else
        {
          tpCallback->OnFault(pAxiomResponseNode);
        }
      }
      RISE_CATCH_ALL_DESCR("Error while processing response")

      return AXIS2_SUCCESS;
    }

    static void CreateFault(ICallback<const CDataObject&>& rCallback, const std::string& sFaultCode, const std::string& sFaultError)
    {
      // generate fault using Operation
      COperation tFault;
      tFault.SetFault(sFaultCode, sFaultError);
      rCallback.OnFault(tFault.GetResponse());
    }
  };

  CServiceClient::CServiceClient():
    m_pSvcClient(NULL),
    m_pOptions(NULL),
    m_bOptOwner(false),
    m_bInit(false)
  {
    m_pEnv = CRuntime::Inst().GetAxis2Env("staff_client");
  }

  CServiceClient::~CServiceClient()
  {
    Deinit();
    if (m_bOptOwner && m_pOptions)
    {
      delete m_pOptions;
    }
    CRuntime::Inst().FreeAxis2Env("staff_client");
  }

  void CServiceClient::Init(const std::string& sServiceUri /*= ""*/)
  {
    std::auto_ptr<COptions> pOptions(new COptions);
    if (!sServiceUri.empty())
    {
      pOptions->SetToAddress(sServiceUri);
    }

    Init(*pOptions);
    m_bOptOwner = true;
    pOptions.release();
  }

  void CServiceClient::Init(COptions& rOptions)
  {
    Deinit();

    // Create service client
    m_pSvcClient = axis2_svc_client_create(m_pEnv, CRuntime::Inst().GetAxis2Home().c_str());
    RISE_ASSERTS(m_pSvcClient, "Axis2/C client creation failed. error: " + GetLastErrorStr());

    // Set service client options
    SetOptions(rOptions);

    m_bInit = true;
  }

  void CServiceClient::Deinit()
  {
    if (m_pSvcClient != NULL)
    {
      axis2_svc_client_free(m_pSvcClient, m_pEnv);
      m_pSvcClient = NULL;
    }

    m_bInit = false;
  }

  bool CServiceClient::IsInit() const
  {
    return m_bInit;
  }

  void CServiceClient::SetOptions(COptions& rOptions)
  {
    RISE_ASSERTS(m_pSvcClient, "Service client is not initialized");

    rOptions.SetOwner(false);

    if (m_bOptOwner)
    {
      delete m_pOptions;
      m_bOptOwner = false;
    }

    m_pOptions = &rOptions;

    axis2_svc_client_set_options(m_pSvcClient, m_pEnv, *m_pOptions);
  }

  COptions& CServiceClient::GetOptions()
  {
    RISE_ASSERTS(m_pOptions, "Options is not initialized");

    return *m_pOptions;
  }

  void CServiceClient::EngageModule(const std::string& sModuleName)
  {
    RISE_ASSERTS(m_pSvcClient, "Service client is not initialized");

    axis2_status_t nResult =
      axis2_svc_client_engage_module(m_pSvcClient, m_pEnv, sModuleName.c_str());
    RISE_ASSERTS(nResult == AXIS2_SUCCESS, "Failed to engage module [" + sModuleName + "]: " + GetLastErrorStr());
  }

  void CServiceClient::DisengageModule(const std::string& sModuleName)
  {
    RISE_ASSERTS(m_pSvcClient, "Service client is not initialized");

    axis2_status_t nResult =
      axis2_svc_client_disengage_module(m_pSvcClient, m_pEnv, sModuleName.c_str());
    RISE_ASSERTS(nResult == AXIS2_SUCCESS, "Failed to engage module [" + sModuleName + "]: " + GetLastErrorStr());
  }


  void CServiceClient::AddHeader(CDataObject& rdoHeader)
  {
    RISE_ASSERTS(m_pSvcClient, "Service client is not initialized");

    axis2_svc_client_add_header(m_pSvcClient, m_pEnv, rdoHeader);
  }

  void CServiceClient::RemoveAllHeaders()
  {
    RISE_ASSERTS(m_pSvcClient, "Service client is not initialized");

    axis2_svc_client_remove_all_headers(m_pSvcClient, m_pEnv);
  }


  CDataObject CServiceClient::Invoke(CDataObject& rdoPayload)
  {
    RISE_ASSERTS(m_pSvcClient, "Service client is not initialized");

    PrepareToSend(rdoPayload);

#ifdef _DEBUG
    rise::LogDebug2() << "Sending: \n" << rise::ColorInkBlue
        << rdoPayload.ToString() << rise::ColorDefault;
#endif

    axiom_node_t* pAxiomResponseNode = axis2_svc_client_send_receive(m_pSvcClient, m_pEnv, rdoPayload);
    RISE_ASSERTS(pAxiomResponseNode, "Axis2/C client send receive failed. error: " + GetLastErrorStr());

#ifdef _DEBUG
      rise::LogDebug2() << "Received Response: \n" << rise::ColorInkBlue
          << CDataObject(pAxiomResponseNode).ToString() << rise::ColorDefault;
#endif

    return pAxiomResponseNode;
  }

  void CServiceClient::Invoke(CDataObject& rdoPayload, ICallback<const CDataObject&>& rCallback)
  {
    RISE_ASSERTS(m_pSvcClient, "Service client is not initialized");

    PrepareToSend(rdoPayload);

    // asynchronous call
    axis2_callback_t* pAxis2Callback = axis2_callback_create(m_pEnv);
    RISE_ASSERTS(pAxis2Callback, "Cannot create callback");

    // store Axis2/C callback
    rCallback.Set(pAxis2Callback, m_pEnv);

    axis2_callback_set_on_complete(pAxis2Callback, CServiceClientTools::CallbackOnComplete);
    axis2_callback_set_on_error(pAxis2Callback, CServiceClientTools::CallbackOnFault);
    axis2_callback_set_data(pAxis2Callback, new CCallbackWrapper(&rCallback, false));

#ifdef _DEBUG
    rise::LogDebug2() << "Sending: \n" << rise::ColorInkBlue
        << rdoPayload.ToString() << rise::ColorDefault;
#endif

    axis2_svc_client_send_receive_non_blocking(m_pSvcClient, m_pEnv, rdoPayload, pAxis2Callback);
  }

  void CServiceClient::Invoke(CDataObject& rdoPayload, PICallback& rpCallback)
  {
    RISE_ASSERTS(m_pSvcClient, "Service client is not initialized");

    PrepareToSend(rdoPayload);

    // asynchronous call
    axis2_callback_t* pAxis2Callback = axis2_callback_create(m_pEnv);
    RISE_ASSERTS(pAxis2Callback, "Cannot create callback");

    // store Axis2/C callback
    (rpCallback)->Set(pAxis2Callback, m_pEnv);

    axis2_callback_set_on_complete(pAxis2Callback, CServiceClientTools::CallbackOnComplete);
    axis2_callback_set_on_error(pAxis2Callback, CServiceClientTools::CallbackOnFault);
    axis2_callback_set_data(pAxis2Callback, new CCallbackWrapper(rpCallback.release(), true));

#ifdef _DEBUG
    rise::LogDebug2() << "Sending: \n" << rise::ColorInkBlue
        << rdoPayload.ToString() << rise::ColorDefault;
#endif

    axis2_svc_client_send_receive_non_blocking(m_pSvcClient, m_pEnv, rdoPayload, pAxis2Callback);
  }


  void CServiceClient::SendRobust(CDataObject& rdoPayload)
  {
    RISE_ASSERTS(m_pSvcClient, "Service client is not initialized");

    PrepareToSend(rdoPayload);

    axis2_status_t nResult = axis2_svc_client_send_robust(m_pSvcClient, m_pEnv, rdoPayload);
    RISE_ASSERTS(nResult == AXIS2_SUCCESS, "Axis2/C client send robust failed. error: " + GetLastErrorStr());

#ifdef _DEBUG
    rise::LogDebug2() << "Sending: \n" << rise::ColorInkBlue
        << rdoPayload.ToString() << rise::ColorDefault;
#endif

    axis2_bool_t bIsFault = axis2_svc_client_get_last_response_has_fault(m_pSvcClient, m_pEnv);
    RISE_ASSERTS(bIsFault != AXIS2_TRUE, "Axis2/C client send robust fault received.");
  }

  void CServiceClient::Send(CDataObject& rdoPayload)
  {
    RISE_ASSERTS(m_pSvcClient, "Service client is not initialized");

    PrepareToSend(rdoPayload);

#ifdef _DEBUG
    rise::LogDebug2() << "Sending: \n" << rise::ColorInkBlue
        << rdoPayload.ToString() << rise::ColorDefault;
#endif

    axis2_svc_client_fire_and_forget(m_pSvcClient, m_pEnv, rdoPayload);
  }

  bool CServiceClient::GetLastResponseHasFault()
  {
    RISE_ASSERTS(m_pSvcClient, "Service client is not initialized");

    return axis2_svc_client_get_last_response_has_fault(m_pSvcClient, m_pEnv) == AXIS2_TRUE;
  }


  CDataObject CServiceClient::GetLastResponse()
  {
    RISE_ASSERTS(m_pSvcClient, "Service client is not initialized");

    axiom_soap_envelope_t* pSoapEnv =
        axis2_svc_client_get_last_response_soap_envelope(m_pSvcClient, m_pEnv);
    if (!pSoapEnv)
    {
      return NULL;
    }

    return axiom_soap_envelope_get_base_node(pSoapEnv, m_pEnv);
  }


  std::string CServiceClient::GetLastErrorStr()
  {
    const char* szError = reinterpret_cast<const char*>(AXIS2_ERROR_GET_MESSAGE(m_pEnv->error));
    return szError ? szError : "";
  }

  CServiceClient::operator axis2_svc_client_t*()
  {
    return m_pSvcClient;
  }

  void CServiceClient::PrepareToSend(CDataObject& rdoPayload)
  {
    RISE_ASSERTS(m_pOptions, "Options is not initialized");
    rdoPayload.SetOwner(false);

    // adding session id header
    if (!m_pOptions->GetSessionId().empty())
    {
      axiom_node_t* pNodeSessionId = NULL;
      axiom_element_t* pElemSessionId = NULL;
      axiom_namespace_t* pHeaderNs = NULL;

      pHeaderNs = axiom_namespace_create(m_pEnv, "http://tempui.org/staff/sessionid", "sid");
      pElemSessionId = axiom_element_create(m_pEnv, NULL, "SessionId", pHeaderNs, &pNodeSessionId);
      axiom_element_set_text(pElemSessionId, m_pEnv, m_pOptions->GetSessionId().c_str(), pNodeSessionId);
      axis2_svc_client_add_header(m_pSvcClient, m_pEnv, pNodeSessionId);
    }

    // adding instance id header
    if (!m_pOptions->GetInstanceId().empty())
    {
      axiom_node_t* pNodeInstanceId = NULL;
      axiom_element_t* pElemInstanceId = NULL;
      axiom_namespace_t* pHeaderNs = NULL;

      pHeaderNs = axiom_namespace_create(m_pEnv, "http://tempui.org/staff/instanceid", "iid");
      pElemInstanceId = axiom_element_create(m_pEnv, NULL, "InstanceId", pHeaderNs, &pNodeInstanceId);
      axiom_element_set_text(pElemInstanceId, m_pEnv, m_pOptions->GetInstanceId().c_str(), pNodeInstanceId);
      axis2_svc_client_add_header(m_pSvcClient, m_pEnv, pNodeInstanceId);
    }

    if (!m_pOptions->GetDefaultNamespaceUri().empty())
    {
      axiom_namespace_t* pNamespace =
          axiom_namespace_create(m_pEnv, m_pOptions->GetDefaultNamespaceUri().c_str(),
                                 m_pOptions->GetDefaultNamespacePrefix().c_str());
      axiom_element_set_namespace(rdoPayload, m_pEnv, pNamespace, rdoPayload);
    }
  }
}