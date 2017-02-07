open Core;

module PopupWindow = {
  include ReactRe.Component.Stateful.InstanceVars;
  let name = "PopupWindow";

  type props = {
    initialText: string
  };
  type state = {
    showSaveBadge: bool,
    inText: string,
    outText: string,
    inLang: option string,
    outLang: option string
  };
  type instanceVars = {
    mutable inputRef: option ReactRe.reactRef
  };

  let getInstanceVars () => {
    inputRef: None
  };

  let getInitialState props => {
    showSaveBadge: false,
    inText: "",
    outText: "",
    inLang: None,
    outLang: None
  };

  let showSaveBadge { state, updater } () => {
    Util.setTimeout (updater (fun { state } () => Some { ...state, showSaveBadge: false })) 2500;
    Some { ...state, showSaveBadge: true }
  };

  let open_ { state } e => {
    Chrome.Tabs.create { "url": ("popup.html#" ^ (Util.btoa state.inText)) };
    None
  };

  let refmt { state, updater } value => {
    open RefmtProtocol;
    /* this isn't guaranteed to be sync or speedy, so
     * don't set this.state.in here, since it could cause lag.
     */
    Chrome.Runtime.sendMessage
      { input: value }
      (fun { output: (conversionType, outText) } => {
        let (inLang, outLang) = parseConversionType conversionType;
        updater (fun { state } () => Some { ...state, outText, inLang, outLang }) ()
      });

    Chrome.Storage.Local.set { "latestRefmtString": value };
    Some { ...state, inText: value }
  };

  let componentDidMount { props, instanceVars, updater } => {
    updater refmt props.initialText;

    switch instanceVars.inputRef {
      | None => ()
      | Some ref => {
        CM.focus ref;
        CM.execCommand ref "selectAll";
      }
    };
    None
  };

  let render { props, state, updater, refSetter } => {
    <div style=Styles.popup>
      <div style=Styles.popupColumn>
        <h1 style=Styles.popupContext>
          <ColumnTitle name="In" lang=state.inLang />
        </h1>
        <Editor
          value=state.inText
          defaultValue=props.initialText
          lang=state.inLang
          ref=(refSetter (fun {instanceVars} ref => instanceVars.inputRef = Some ref))
          onChange=(updater refmt)
        />
      </div>
      <div style=Styles.popupColumn>
        <h1 style=Styles.popupContext>
          <ColumnTitle name="Out" lang=state.outLang />
          <CopyButton text=state.outText onCopy=(updater showSaveBadge) />
          <OpenButton onClick=(updater open_) />
        </h1>
        <Editor value=state.outText lang=state.outLang readOnly=true />
        <SaveBadge show=state.showSaveBadge />
      </div>
    </div>;
  }
};

include ReactRe.CreateComponent PopupWindow;
let createElement ::initialText ::children =>
  wrapProps { initialText: initialText } ::children;
