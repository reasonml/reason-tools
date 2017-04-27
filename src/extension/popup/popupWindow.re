open Rebase;

open Core;

module PopupWindow = {
  include ReactRe.Component.Stateful;
  let name = "PopupWindow";
  type props = {
    inText: string,
    outText: string,
    inLang: option string,
    outLang: option string,
    link: string,
    onOpen: string => unit,
    onInputChanged: string => unit
  };
  type state = {copyConfirmation: option string, mutable inputRef: option ReactRe.reactRef};
  /* Init */
  let getInitialState _ => {copyConfirmation: None, inputRef: None};
  /* Actions */
  let showCopyConfirmation text {state, updater} () => {
    Util.setTimeout (updater (fun _ () => Some {...state, copyConfirmation: None})) 2500;
    Some {...state, copyConfirmation: Some text}
  };
  let updateInputRef {state} ref => state.inputRef = Some ref;
  /* Lifecycle events */
  let componentDidMount {state} => {
    switch state.inputRef {
    | None => ()
    | Some ref =>
      CodeMirror.focus ref;
      CodeMirror.execCommand ref "selectAll"
    };
    None
  };
  let render {props, state, updater, handler} =>
    <div style=PopupStyles.popup>
      <div style=PopupStyles.popupColumn>
        <h1 style=PopupStyles.popupContext> <ColumnTitle name="In" lang=props.inLang /> </h1>
        <Editor
          value=props.inText
          lang=props.inLang
          ref=(handler updateInputRef)
          onChange=props.onInputChanged
        />
      </div>
      <div style=PopupStyles.popupColumn>
        <h1 style=PopupStyles.popupContext>
          <ColumnTitle name="Out" lang=props.outLang />
          <CopyButton
            style=PopupStyles.contextLink
            label="share"
            text=props.link
            onCopy=(updater (showCopyConfirmation "Link copied to clipboard"))
          />
          <CopyButton
            style=PopupStyles.contextLink
            text=props.outText
            onCopy=(updater (showCopyConfirmation "Text copied to clipboard"))
          />
          <OpenButton style=PopupStyles.contextIcon onClick=(fun _ => props.onOpen props.inText) />
        </h1>
        <Editor value=props.outText lang=props.outLang readOnly=true />
        <CopyConfirmation
          style=PopupStyles.copyConfirmation
          show=(Option.isSome state.copyConfirmation)
          text=(Option.getOr "" state.copyConfirmation)
        />
      </div>
    </div>;
};

include ReactRe.CreateComponent PopupWindow;

let createElement ::inText ::inLang ::outText ::outLang ::link ::onOpen ::onInputChanged =>
  wrapProps {inText, inLang, outText, outLang, onOpen, link, onInputChanged};
