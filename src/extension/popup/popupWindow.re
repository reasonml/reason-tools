open Rebase;

open Core;

let select name onChange language lang =>
  <select name onChange value=(Protocol.stringOfLanguage language)>
    <option value="auto">
      (
        ReactRe.stringToElement (
          "Auto" ^ (
            switch (lang, language) {
            | (lang, Protocol.UnknownLang) when lang != Protocol.UnknownLang =>
              " (" ^ Protocol.stringOfLanguage lang ^ ")"
            | _ => ""
            }
          )
        )
      )
    </option>
    <option value="RE"> (ReactRe.stringToElement "RE") </option>
    <option value="ML"> (ReactRe.stringToElement "ML") </option>
  </select>;

module PopupWindow = {
  include ReactRe.Component.Stateful;
  let name = "PopupWindow";
  type props = {
    inText: string,
    outText: string,
    inLang: Protocol.language,
    outLang: Protocol.language,
    link: string,
    onOpen: string => unit,
    onInputChanged: inLang::Protocol.language? => outLang::Protocol.language? => string => unit
  };
  type state = {
    copyConfirmation: option string,
    mutable inputRef: option ReactRe.reactRef,
    inLanguage: Protocol.language,
    outLanguage: Protocol.language
  };
  /* Init */
  let getInitialState _ => {
    copyConfirmation: None,
    inputRef: None,
    inLanguage: Protocol.UnknownLang,
    outLanguage: Protocol.UnknownLang
  };
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
  let handleInLangChange {props, state} e => {
    let inLanguage =
      e |> ReactEventRe.Synthetic.target |> LocalDom.Element.value |> Protocol.languageOfString;
    props.onInputChanged inLang::inLanguage outLang::state.outLanguage props.inText;
    Some {...state, inLanguage}
  };
  let handleOutLangChange {props, state} e => {
    let outLanguage =
      e |> ReactEventRe.Synthetic.target |> LocalDom.Element.value |> Protocol.languageOfString;
    props.onInputChanged inLang::state.inLanguage outLang::outLanguage props.inText;
    Some {...state, outLanguage}
  };
  let handleInputChange {props, state} input =>
    props.onInputChanged inLang::state.inLanguage outLang::state.outLanguage input;
  let render {props, state, updater, handler} =>
    <div style=PopupStyles.popup>
      <div style=PopupStyles.popupColumn>
        <h1 style=PopupStyles.popupContext>
          <ColumnTitle
            lang=props.inLang
            select=(select "in" (updater handleInLangChange) state.inLanguage props.inLang)
          />
        </h1>
        <Editor
          value=props.inText
          lang=props.inLang
          ref=(handler updateInputRef)
          onChange=(handler handleInputChange)
        />
      </div>
      <div style=PopupStyles.popupColumn>
        <h1 style=PopupStyles.popupContext>
          <ColumnTitle
            lang=?props.outLang
            select=(select "out" (updater handleOutLangChange) state.outLanguage props.outLang)
          />
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
