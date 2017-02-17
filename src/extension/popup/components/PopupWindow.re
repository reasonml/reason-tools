open Core;

module PopupWindow = {
  include ReactRe.Component.Stateful.InstanceVars;
  let name = "PopupWindow";
  type props = {
    initialText: string,
    onOpen: string => unit,
    onRefmt: string => (string => option string => option string => unit) => unit
  };
  type state = {
    showSaveBadge: bool,
    inText: string,
    outText: string,
    inLang: option string,
    outLang: option string
  };
  type instanceVars = {mutable inputRef: option ReactRe.reactRef};
  /* Init */
  let getInstanceVars () => {inputRef: None};
  let getInitialState _ => {
    showSaveBadge: false,
    inText: "",
    outText: "",
    inLang: None,
    outLang: None
  };
  /* Actions */
  let showSaveBadge {state, updater} () => {
    Util.setTimeout (updater (fun {state} () => Some {...state, showSaveBadge: false})) 2500;
    Some {...state, showSaveBadge: true}
  };
  let open_ {props, state} _ => {
    props.onOpen state.inText;
    None
  };
  let refmt {props, state, updater} inText => {
    let updater' outText inLang outLang =>
      updater (fun {state} () => Some {...state, outText, inLang, outLang}) ();
    props.onRefmt inText updater';
    Some {...state, inText}
  };
  /* Lifecycle events */
  let componentDidMount {props, instanceVars, updater} => {
    updater refmt props.initialText;
    switch instanceVars.inputRef {
    | None => ()
    | Some ref =>
      CodeMirror.focus ref;
      CodeMirror.execCommand ref "selectAll"
    };
    None
  };
  let render {props, state, updater, refSetter} =>
    <div style=PopupStyles.popup>
      <div style=PopupStyles.popupColumn>
        <h1 style=PopupStyles.popupContext> <ColumnTitle name="In" lang=state.inLang /> </h1>
        <Editor
          value=state.inText
          defaultValue=props.initialText
          lang=state.inLang
          ref=(refSetter (fun {instanceVars} ref => instanceVars.inputRef = Some ref))
          onChange=(updater refmt)
        />
      </div>
      <div style=PopupStyles.popupColumn>
        <h1 style=PopupStyles.popupContext>
          <ColumnTitle name="Out" lang=state.outLang />
          <CopyButton
            style=PopupStyles.contextLink
            text=state.outText
            onCopy=(updater showSaveBadge)
          />
          <OpenButton style=PopupStyles.contextIcon onClick=(updater open_) />
        </h1>
        <Editor value=state.outText lang=state.outLang readOnly=true />
        <SaveBadge style=PopupStyles.savedBadge show=state.showSaveBadge />
      </div>
    </div>;
};

include ReactRe.CreateComponent PopupWindow;

let createElement ::initialText ::onOpen ::onRefmt => wrapProps {initialText, onOpen, onRefmt};
