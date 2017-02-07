open Core;

module PopupWindow = {
  include ReactRe.Component.Stateful.InstanceVars;
  let name = "PopupWindow";

  type props = {
    initialText: string
  };
  type state = {
    copy: bool,
    in_: string,
    out: string,
    inLang: option string,
    outLang: option string
  };
  type instanceVars = {
    mutable _in: option ReactRe.reactRef
  };

  let getInstanceVars () => {
    _in: None
  };

  let getInitialState props => {
    copy: false,
    in_: "",
    out: "",
    inLang: None,
    outLang: None
  };

  let copy { state, updater } () => {
    Util.setTimeout (updater (fun { state } () => Some { ...state, copy: false })) 2500;
    Some { ...state, copy: true }
  };

  let open_ { state } e => {
    Chrome.Tabs.create { "url": ("popup.html#" ^ (Util.btoa state.in_)) };
    None
  };

  let refmt { state, updater } value => {
    open RefmtProtocol;
    /* this isn't guaranteed to be sync or speedy, so
     * don't set this.state.in here, since it could cause lag.
     */
    Chrome.Runtime.sendMessage
      { in_: value }
      (fun { out: (conversionType, out) } => {
        let (inLang, outLang) = parseConversionType conversionType;
        updater (fun { state } () => Some { ...state, out, inLang, outLang }) ()
      });

    Chrome.Storage.Local.set { "latestRefmtString": value };
    Some { ...state, in_: value }
  };

  let componentDidMount { props, instanceVars, updater } => {
    updater refmt props.initialText;

    switch instanceVars._in {
      | None => ()
      | Some ref => {
        CM.focus ref;
        CM.execCommand ref "selectAll";
      }
    };
    None
  };

  let render { props, state, updater, refSetter } => {
    /* extra cautious here because of the async on _refmt
     * sometimes typing can flash the wrong lang very quickly
     */
    let hasConverted = not (Str.isEmpty state.in_) && not (Str.isEmpty state.out);

    <div style=Styles.popup>
      <div style=Styles.popupColumn>
        <h1 style=Styles.popupContext>
          <ColumnTitle name="In" lang=(hasConverted ? state.inLang : None) />
        </h1>
        <Editor
          value=state.in_
          defaultValue=props.initialText
          lang=state.inLang
          ref=(refSetter (fun {instanceVars} ref => instanceVars._in = Some ref))
          onChange=(updater refmt)
        />
      </div>
      <div style=Styles.popupColumn>
        <h1 style=Styles.popupContext>
          <ColumnTitle name="Out" lang=(hasConverted ? state.outLang : None) />
          <CopyButton text=state.out onCopy=(updater copy) />
          <OpenButton onClick=(updater open_) />
        </h1>
        <Editor value=state.out lang=state.outLang readOnly=true />
        <SaveBadge show=state.copy />
      </div>
    </div>;
  }
};

include ReactRe.CreateComponent PopupWindow;
let createElement ::initialText ::children =>
  wrapProps { initialText: initialText } ::children;
