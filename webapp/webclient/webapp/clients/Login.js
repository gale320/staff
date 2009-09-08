// This file generated by staff_codegen
// DO NOT EDIT
namespace('staff');



///////////////////////////////////////////////////////////////////////////////////
// class: staff.Login

staff.Login = Class.create();
staff.Login.prototype = 
{
  initialize: function(sServiceUri, sSessionId, sTargetNamespace)
  {
    if (!sServiceUri)
    {
      sServiceUri = webapp.Env.protocol + Session.sHost + (Session.sPort ? (':' + Session.sPort) : '') + '/axis2/services/staff.Login';
    }
    
    if (!sTargetNamespace)
    {
      sTargetNamespace = sServiceUri;
    }
    
    this.sTargetNamespace = sTargetNamespace || sServiceUri;

    this.tClient = new staff.Client(sServiceUri, sSessionId || Session.sID || "");
  },
  
  SetDataObjectAsXml: function(bDataObjectAsXml)
  {
    this.bDataObjectAsXml = bDataObjectAsXml;
  },
  
  SetID: function(sSessionId)
  {
    this.tClient.SetSessionId(sSessionId);
  },

  Login: function(sUserName, sPassword, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('Login', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    tOperation.AddParameter('sUserName', sUserName);
    tOperation.AddParameter('sPassword', sPassword);
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation.ResultValue(), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
      return tOperation.ResultValue();
    }
  },

  OpenSession: function(sUserName, sPassword, bCloseExisting, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('OpenSession', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    tOperation.AddParameter('sUserName', sUserName);
    tOperation.AddParameter('sPassword', sPassword);
    tOperation.AddParameter('bCloseExisting', bCloseExisting);
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation.ResultValue(), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
      return tOperation.ResultValue();
    }
  },

  OpenExtraSession: function(nExtraSessionId, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('OpenExtraSession', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    tOperation.AddParameter('nExtraSessionId', nExtraSessionId);
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation.ResultValue(), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
      return tOperation.ResultValue();
    }
  },

  Logout: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('Logout', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  KeepAliveSession: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('KeepAliveSession', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  ValidateSession: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('ValidateSession', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation.ResultValue(), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return tOperation.ResultValue();
    }
  },

  GetUserName: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetUserName', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation.ResultValue(), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
      return tOperation.ResultValue();
    }
  },

  GetSessionExpiration: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetSessionExpiration', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation.ResultValue(), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return tOperation.ResultValue();
    }
  }
}

