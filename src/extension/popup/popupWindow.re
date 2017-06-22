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

type state = {
  copyConfirmation: option string, 
  inputRef: option ReasonReact.reactRef,
  inLanguage: Protocol.language,
  outLanguage: Protocol.language
};

let showCopyConfirmation text () state self => {
  Util.setTimeout (self.ReasonReact.update (fun () _ _ => ReasonReact.Update {...state, copyConfirmation: None})) 2500;
  ReasonReact.Update {...state, copyConfirmation: Some text}
};

let updateInputRef nullableRef state _ =>
  switch (Js.Null.to_opt nullableRef) {
  | Some ref => ReasonReact.SilentUpdate {...state, inputRef: Some ref};
  | None => ReasonReact.NoUpdate
};

let component = ReasonReact.statefulComponent "PopupWindow";
let make
  ::inText
  ::inLang
  ::outText
  ::outLang
  ::link
  ::onOpen
  onInputChanged::(onInputChanged: inLang::Protocol.language? => outLang::Protocol.language? => string => unit)
  _ => {
  ...component,

  initialState: fun () => {
    copyConfirmation: None,
    inputRef: None,
    inLanguage: Protocol.UnknownLang,
    outLanguage: Protocol.UnknownLang
  },

  didMount: fun state _ => {
    switch state.inputRef {
    | None => ()
    | Some ref =>
      CodeMirror.focus ref;
      CodeMirror.execCommand ref "selectAll"
    };
    ReasonReact.NoUpdate
  },

  render: fun state self => {
    let handleInLangChange e state _ => {
      let inLanguage =
        e |> ReactEventRe.Synthetic.target |> LocalDom.Element.value |> Protocol.languageOfString;
      onInputChanged inLang::inLanguage outLang::state.outLanguage inText;
      ReasonReact.Update {...state, inLanguage}
    };
    let handleOutLangChange e state _ => {
      let outLanguage =
        e |> ReactEventRe.Synthetic.target |> LocalDom.Element.value |> Protocol.languageOfString;
      onInputChanged inLang::state.inLanguage outLang::outLanguage inText;
      ReasonReact.Update {...state, outLanguage}
    };
    let handleInputChange input state _ =>
      onInputChanged inLang::state.inLanguage outLang::state.outLanguage input;

    <div style=PopupStyles.popup>
      <div style=PopupStyles.popupColumn>
        <h1 style=PopupStyles.popupContext>
          <ColumnTitle
            lang=inLang
            select=(select "in" (self.update handleInLangChange) state.inLanguage inLang)
          />
        </h1>
        <Editor
          value=inText
          lang=inLang
          inputRef=(self.update updateInputRef)
          onChange=(self.handle handleInputChange)
        />
      </div>
      <div style=PopupStyles.popupColumn>
        <h1 style=PopupStyles.popupContext>
          <ColumnTitle
            lang=outLang
            select=(select "out" (self.update handleOutLangChange) state.outLanguage outLang)
          />
          <CopyButton
            style=PopupStyles.contextLink
            label="share"
            text=link
            onCopy=(self.update (showCopyConfirmation "Link copied to clipboard"))
          />
          <CopyButton
            style=PopupStyles.contextLink
            text=outText
            onCopy=(self.update (showCopyConfirmation "Text copied to clipboard"))
          />
          <OpenButton style=PopupStyles.contextIcon onClick=(fun _ => onOpen inText) />
        </h1>
        <Editor value=outText lang=outLang readOnly=true />
        <CopyConfirmation
          style=PopupStyles.copyConfirmation
          show=(Option.isSome state.copyConfirmation)
          text=(Option.getOr "" state.copyConfirmation)
        />
      </div>
    </div>
  }
};