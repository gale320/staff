// This file generated by staff_codegen
// Service Implementation

#include <rise/common/Log.h>
#include <rise/common/exmacros.h>
#include <staff/common/Exception.h>
#include <staff/security/Admin.h>
#include "ObjectAdminImpl.h"

namespace staff
{
  namespace admin
  {
/*    template<size_t nSize>
    char* operator<<(char (&szString)[nSize], const std::string& sString)
    {
      strncpy(szString, sString.c_str(), nSize);
      if (nSize < sString.size())
      {
        szString[nSize - 1] = '\0';
      }
      else
      {
        szString[sString.size() - 1] = '\0';
      }

      return szString;
    }*/

    char* ToCString(char* szString, unsigned nSize, const std::string& sString)
    {
      if (nSize > sString.size())
      {
        nSize = sString.size() + 1;
      }
      else
      {
        szString[nSize - 1] = '\0';
        --nSize;
      }

      strncpy(szString, sString.c_str(), nSize);

      return szString;
    }



    SObject& operator<<(SObject& rstObject, const TObject& rtObject)
    {
      rstObject.nObjectId = rtObject.nObjectId;
      rstObject.sObjectName = rtObject.szObjectName;
      rstObject.nType = rtObject.nType;
      rstObject.sDescription = rtObject.szDescription;
      rstObject.nUserId = rtObject.nUserId;
      rstObject.nGroupId = rtObject.nGroupId;
      rstObject.nParentObjectId = rtObject.nParentObjectId;

      rstObject.stPermissions.stUser.bRead = rtObject.stPermissionUser.bRead;
      rstObject.stPermissions.stUser.bWrite = rtObject.stPermissionUser.bWrite;
      rstObject.stPermissions.stUser.bExecute = rtObject.stPermissionUser.bExecute;

      rstObject.stPermissions.stGroup.bRead = rtObject.stPermissionGroup.bRead;
      rstObject.stPermissions.stGroup.bWrite = rtObject.stPermissionGroup.bWrite;
      rstObject.stPermissions.stGroup.bExecute = rtObject.stPermissionGroup.bExecute;

      rstObject.stPermissions.stOthers.bRead = rtObject.stPermissionOthers.bRead;
      rstObject.stPermissions.stOthers.bWrite = rtObject.stPermissionOthers.bWrite;
      rstObject.stPermissions.stOthers.bExecute = rtObject.stPermissionOthers.bExecute;

      return rstObject;
    }

    TObject& operator<<(TObject& rtObject, const SObject& rstObject)
    {
      rtObject.nObjectId = rstObject.nObjectId;

#ifdef WIN32
#pragma warning ( push )
#pragma warning ( disable : 4996 )
#endif
//      rstObject.sObjectName.copy(rtObject.szObjectName, sizeof(rtObject.szObjectName) - 1);
//      rtObject.szObjectName[sizeof(rtObject.szObjectName) - 1] = '\0';
//       rtObject.szObjectName << rstObject.sObjectName;
      ToCString(rtObject.szObjectName, sizeof(rtObject.szObjectName), rstObject.sObjectName);

      rtObject.nType = rstObject.nType;
//       rstObject.sDescription.copy(rtObject.szDescription, sizeof(rtObject.szDescription) - 1);
//       rtObject.szDescription[sizeof(rtObject.szDescription) - 1] = '\0';
//      rtObject.szDescription << rstObject.sDescription;
      ToCString(rtObject.szDescription, sizeof(rtObject.szDescription), rstObject.sDescription);

#ifdef WIN32
#pragma warning ( pop )
#endif

      rtObject.nUserId = rstObject.nUserId;
      rtObject.nGroupId = rstObject.nGroupId;
      rtObject.nParentObjectId = rstObject.nParentObjectId;

      rtObject.stPermissionUser.bRead = rstObject.stPermissions.stUser.bRead;
      rtObject.stPermissionUser.bWrite = rstObject.stPermissions.stUser.bWrite;
      rtObject.stPermissionUser.bExecute = rstObject.stPermissions.stUser.bExecute;

      rtObject.stPermissionGroup.bRead = rstObject.stPermissions.stGroup.bRead;
      rtObject.stPermissionGroup.bWrite = rstObject.stPermissions.stGroup.bWrite;
      rtObject.stPermissionGroup.bExecute = rstObject.stPermissions.stGroup.bExecute;

      rtObject.stPermissionOthers.bRead = rstObject.stPermissions.stOthers.bRead;
      rtObject.stPermissionOthers.bWrite = rstObject.stPermissions.stOthers.bWrite;
      rtObject.stPermissionOthers.bExecute = rstObject.stPermissions.stOthers.bExecute;
      
      return rtObject;
    }


    ::staff::admin::SObjectType& operator<<(::staff::admin::SObjectType& rstObjectType, const ::TObjectType& rtObjectType)
    {
      rstObjectType.nId = rtObjectType.nId;
      rstObjectType.sName = rtObjectType.szName;
      rstObjectType.sDescription = rtObjectType.szDescription;
      return rstObjectType;
    }

    ::TObjectType& operator<<(::TObjectType& rtObjectType, const ::staff::admin::SObjectType& rstObjectType )
    {
      rtObjectType.nId = rstObjectType.nId;
      ToCString(rtObjectType.szName, sizeof(rtObjectType.szName), rstObjectType.sName);
      ToCString(rtObjectType.szDescription, sizeof(rtObjectType.szDescription), rstObjectType.sDescription);
      return rtObjectType;
    }


    //////////////////////////////////////////////////////////////////////////
    // service implementation

    CObjectAdminImpl::CObjectAdminImpl()
    {
    }

    CObjectAdminImpl::~CObjectAdminImpl()
    {
    }

    ::staff::admin::TObjectIdList CObjectAdminImpl::GetObjectIdList()
    {
      ::staff::admin::TObjectIdList tResult;

      int* pnObjectsIds = NULL;
      int nObjectsCount = 0;

      RISE_ASSERTES(StaffSecurityAdminGetObjectIdList(&pnObjectsIds, &nObjectsCount), 
        staff::CRemoteException, "Failed to get object id list");

      try
      {
        tResult.assign(pnObjectsIds, pnObjectsIds + nObjectsCount);
      }
      catch(...)
      {
        StaffSecurityAdminFreeObjectIds(pnObjectsIds);
        throw;
      }
      
      StaffSecurityAdminFreeObjectIds(pnObjectsIds);

      return tResult;  // result
    }

    ::staff::admin::TObjectIdList CObjectAdminImpl::GetObjectIdListWithParent(int nParentId)
    {
      ::staff::admin::TObjectIdList tResult;

      int* pnObjectsIds = NULL;
      int nObjectsCount = 0;

      RISE_ASSERTES(StaffSecurityAdminGetObjectIdListWithParent(&pnObjectsIds, nParentId, &nObjectsCount), 
        staff::CRemoteException, "Failed to get object id list");

      try
      {
        tResult.assign(pnObjectsIds, pnObjectsIds + nObjectsCount);
      }
      catch(...)
      {
        StaffSecurityAdminFreeObjectIds(pnObjectsIds);
        throw;
      }

      StaffSecurityAdminFreeObjectIds(pnObjectsIds);

      return tResult;  // result
    }

    ::staff::admin::SObject CObjectAdminImpl::GetObjectById(int nObjectId)
    {
      ::staff::admin::SObject tResult;
      TObject tObject;

      StaffSecurityAdminGetObjectById(nObjectId, &tObject);
      tResult << tObject;

      return tResult;  // result
    }

    ::staff::admin::TObjectList CObjectAdminImpl::GetObjectList(const ::staff::admin::TObjectIdList& rlsObjectIdList)
    {
      ::staff::admin::TObjectList tResult;
      TObject* pObjectList = NULL;
      SObject stObject;
      int nObjectCount = 0;

      RISE_ASSERTES(rlsObjectIdList.size() > 0, staff::CRemoteException, "id list is empty");

      StaffSecurityAdminGetObjectList(&rlsObjectIdList[0], rlsObjectIdList.size(), &pObjectList, &nObjectCount);

      try
      {
        for (int nIndex = 0; nIndex < nObjectCount; ++nIndex)
        {
          stObject << pObjectList[nIndex];
          tResult.push_back(stObject);
        }
      }
      catch(...)
      {
        StaffSecurityFreeObjectList(pObjectList);
        throw;
      }

      StaffSecurityFreeObjectList(pObjectList);

      return tResult;  // result
    }

    int CObjectAdminImpl::AddObject(const ::staff::admin::SObject& rstObject)
    {
      int tResult;
      TObject tObject;
      tObject << rstObject;

      RISE_ASSERTES(StaffSecurityAdminAddObject(&tObject, &tResult), staff::CRemoteException, "Failed to add object");

      return tResult;  // result
    }

    void CObjectAdminImpl::RemoveObject(int nObjectId)
    {
      RISE_ASSERTES(StaffSecurityAdminRemoveObject(nObjectId), staff::CRemoteException, "Failed to remove object");
    }

    void CObjectAdminImpl::ReplaceObject(const ::staff::admin::SObject& rstObject)
    {
      TObject tObject;
      tObject << rstObject;

      RISE_ASSERTES(StaffSecurityAdminReplaceObject(&tObject), staff::CRemoteException, "Failed to replace object");
    }



    ::staff::admin::TObjectTypesList CObjectAdminImpl::GetObjectTypeList()
    {
      ::staff::admin::TObjectTypesList tResult;

      ::TObjectType* ptObjectType = NULL;
      int nCount = 0;
      SObjectType stObjectTypeTmp;

      RISE_ASSERTES(StaffSecurityAdminGetObjectTypeList(&ptObjectType, &nCount), 
        staff::CRemoteException, "Failed to get object type");
      
      for (int nIndex = 0; nIndex < nCount; ++nIndex)
      {
        stObjectTypeTmp << ptObjectType[nIndex];
        tResult.push_back(stObjectTypeTmp);
      }

      StaffSecurityAdminFreeObjectTypes(ptObjectType);

      return tResult;
    }

    int CObjectAdminImpl::AddObjectType( const ::staff::admin::SObjectType& rstObjectType )
    {
      int tResult = 0;
      ::TObjectType tObjectType;

      tObjectType << rstObjectType;

      RISE_ASSERTES(StaffSecurityAdminAddObjectType(&tObjectType, &tResult), 
        staff::CRemoteException, "Failed to add object type");

      return tResult;
    }

    void CObjectAdminImpl::ReplaceObjectType( const ::staff::admin::SObjectType& rstObjectType )
    {
      ::TObjectType tObjectType;

      tObjectType << rstObjectType;

      RISE_ASSERTES(StaffSecurityAdminReplaceObjectType(&tObjectType), 
        staff::CRemoteException, "Failed to replace object type");
    }

    void CObjectAdminImpl::RemoveObjectType( int nObjectTypeId )
    {
      RISE_ASSERTES(StaffSecurityAdminRemoveObjectType(nObjectTypeId), 
        staff::CRemoteException, "Failed to replace object type");
    }
  }
}

