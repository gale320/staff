namespace('webapp.view');

webapp.view.WidgetFrame = Class.create();
webapp.view.WidgetFrame.prototype.extend(webapp.Event.prototype).extend
({
  initialize: function(tParent, tOptions)
  {
    IncludeCss("webapp/assets/view/WidgetFrame/WidgetFrame");
    this.bCollapsed = false;
    
    if(tOptions == null)
    {
      tOptions = {};
    }

    // parent element
    this._tParentElem = tParent.Element != null ? tParent.Element() : tParent;
    
    // main div
    this._tDivMain = new webapp.ui.Div(this._tParentElem, { sClass: 'WidgetFrame' });
    
    // table for markup layout
    this._tTableMarkup = new webapp.ui.Table(this._tDivMain);
    
    var sNoFrameBorder = tOptions.bNoFrameBorder ? 'NoBorder' : '';

    // header
    this._tRowHead = this._tTableMarkup.AddHeadRow();
    this._tCellHead = this._tTableMarkup.AddCell(this._tRowHead, { sClass: 'WidgetFrameHeader' + sNoFrameBorder });
    
    // body
    this._tRowBody = this._tTableMarkup.AddRow();
    this._tCellBody = this._tTableMarkup.AddCell(this._tRowBody, { sClass: 'WidgetFrameBody' + sNoFrameBorder });

    // header and body divs    
    this.tDivHeader = new webapp.ui.Div(this._tCellHead);
    this._tDivBody = new webapp.ui.Div(this._tCellBody);

    
    //////////////////////////////////////////////////////////////////////////
    // header
    var tTableHeader = new webapp.ui.Table(this.tDivHeader, { sClass: 'WidgetFrameHeaderTable' });
    var tTableHeaderRow = tTableHeader.AddRow();

    // caption    
    var tTableHeaderCellLabel = tTableHeader.AddCell(tTableHeaderRow);
    this.tLabelCaption = new webapp.ui.Label(tTableHeaderCellLabel, { sClass: 'WidgetFrameHeaderLabel' });

    if(tOptions.bConfigure === true)
    {
      // close button
      var tTableHeaderCellConfigure = tTableHeader.AddCell(tTableHeaderRow);
      tTableHeaderCellConfigure.className = "WidgetFrameCellHeaderButton";
      this.tImgConfigure = new webapp.ui.Image(tTableHeaderCellConfigure, 
        { sSrc: "webapp/assets/view/WidgetFrame/img/configure.png", sClass: "WidgetFrameCellHeaderButton" });
      this.tImgConfigure.On('click', this._OnConfigure, this);
    }
    
    if(tOptions.bNoCollapse !== true)
    {
      // rollup button
      var tTableHeaderCellRollup = tTableHeader.AddCell(tTableHeaderRow);
      tTableHeaderCellRollup.className = "WidgetFrameCellHeaderButton";
      this._tImgRollup = new webapp.ui.Image(tTableHeaderCellRollup, 
        { sSrc: "webapp/assets/view/WidgetFrame/img/go-top.png", sClass: "WidgetFrameCellHeaderButton" });
      this._tImgRollup.On('click', this._OnRollup, this);
      this.tLabelCaption.On('dblclick', this._OnRollup, this);
    }
      
    if(tOptions.bNoClose !== true)
    {
      // close button
      var tTableHeaderCellClose = tTableHeader.AddCell(tTableHeaderRow);
      tTableHeaderCellClose.className = "WidgetFrameCellHeaderButton";
      this.tImgClose = new webapp.ui.Image(tTableHeaderCellClose, 
        { sSrc: "webapp/assets/view/WidgetFrame/img/close.png", sClass: "WidgetFrameCellHeaderButton" });
      this.tImgClose.On('click', this._OnClose, this);
    }
    
    if (tOptions.tBody != null)
    {
      this.SetBody(tOptions.tBody);
    }

    if (tOptions.sCaption == null)
    {
      this.tLabelCaption.SetCaption("...");
    }
    else
    {
      this.tLabelCaption.SetCaption(tOptions.sCaption);
    }
    
    this.tEvents = {};
  },
  
  destroy: function()
  {
    this._tParentElem.removeChild(this._tDivMain.Element());
  },
  
  SetBody: function(tBody)
  {
    if (this._tBodyElem)
    {
      this._tDivBody.removeChild(this._tBodyElem);
    }
    
    this._tDivBody.Element().appendChild(tBody.Element ? tBody.Element() : tBody);
    this._tBodyElem = tBody;
  },
  
  GetBody: function()
  {
    return this._tBodyElem;
  },
  
  GetBodyContainer: function()
  {
    return this._tDivBody;
  },
  
  _OnRollup: function(tEvent)
  {
    this.bCollapsed = !this.bCollapsed;

    this.FireEvent('rollup', { bCollapsed: this.bCollapsed });
    if(this.bCollapsed)
    {
      this._tRowBody.style.visibility = "hidden";
      this._tDivMain.Element().style.height = this.tDivHeader.Element().offsetHeight + 2;
      this._tImgRollup.SetSrc("webapp/assets/view/WidgetFrame/img/go-bottom.png");
    }
    else
    {
      this._tRowBody.style.visibility = "visible";
      this._tDivMain.Element().style.height = "auto";
      this._tImgRollup.SetSrc("webapp/assets/view/WidgetFrame/img/go-top.png");
    }
  },
    
  _OnClose: function(tEvent)
  {
    this.FireEvent('close');
  },
  
  _OnConfigure: function(tEvent)
  {
    this.FireEvent('configure');
  }
});
