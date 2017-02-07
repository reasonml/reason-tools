open Core;

let getLangList conversionType =>
  switch (conversionType |> Js.String.split "to") {
  | [| inLang, outLang |] => (Some inLang, Some outLang)
  | _ => (None, None)
};

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
    open Background;
    /* this isn't guaranteed to be sync or speedy, so
     * don't set this.state.in here, since it could cause lag.
     */
    Chrome.Runtime.sendMessage
      { in_: value }
      (fun { out: (conversionType, out) } => {
        let (inLang, outLang) = getLangList conversionType;
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
    let inTitle = hasConverted ? state.inLang |> (Option.map_or (fun lang => "In (" ^ lang ^ ")") "In") : "In";
    let outTitle = hasConverted ? state.outLang |> (Option.map_or (fun lang => "Out (" ^ lang ^ ")") "Out") : "Out";

    <div style=Styles.popup>
      <div style=Styles.popupColumn>
        <h1 style=Styles.popupContext>
          (ReactRe.stringToElement inTitle)
        </h1>
        <CodeMirror
          style=Styles.popupInNOut
          ref=(refSetter (fun {instanceVars} ref => instanceVars._in = Some ref))
          defaultValue=props.initialText
          value=state.in_
          onChange=(updater refmt)
          options={
            "mode": state.inLang == Some "ML" ? "text/ocaml" : "javascript",
            "theme": "oceanic-next"
          }
        />
      </div>
      <div style=Styles.popupColumn>
        <h1 style=Styles.popupContext>
          <span style=Styles.contextTitle>
            (ReactRe.stringToElement outTitle)
          </span>
          <CopyToClipboard text=state.out onCopy=(updater copy)>
            <a style=Styles.contextLink href="#"> (ReactRe.stringToElement "copy") </a>
          </CopyToClipboard>
          <a style=Styles.contextLink href="#" onClick=(updater open_)>
            <svg
              height="3vh"
              width="3vh"
              viewBox="0 0 748 1024"
              /*xmlns="http://www.w3.org/2000/svg"*/> /* Unsupported attribute */
              <path
                d="M640 768H128V257.90599999999995L256 256V128H0v768h768V576H640V768zM384 128l128 128L320 448l128 128 192-192 128 128V128H384z"
                fill=Styles.contextLink##color
              />
            </svg>
          </a>
        </h1>
        <CodeMirror
          style=Styles.popupInNOut
          value=state.out
          options={
            "mode": state.outLang == Some "ML" ? "text/x-ocaml" : "javascript",
            "theme": "oceanic-next",
            "readOnly": Js.Boolean.to_js_boolean true
          }
        />
        (state.copy ?
          <div style=Styles.savedBadge> (ReactRe.stringToElement (Str.fromCharCode 10003)) </div> :
          ReactRe.nullElement)
      </div>
    </div>;
  }
};

include ReactRe.CreateComponent PopupWindow;
let createElement ::initialText ::children =>
  wrapProps { initialText: initialText } ::children;
