open Rebase;

open Core;

let select = (name, onChange, language, lang) =>
  <select name onChange value=(Protocol.stringOfLanguage(language))>
    <option value="auto">
      (
        ReasonReact.stringToElement(
          "Auto"
          ++ (
            switch (lang, language) {
            | (lang, Refmt2.UnknownLang) when lang != Refmt2.UnknownLang =>
              " (" ++ Protocol.stringOfLanguage(lang) ++ ")"
            | _ => ""
            }
          )
        )
      )
    </option>
    <option value="RE"> (ReasonReact.stringToElement("RE")) </option>
    <option value="ML"> (ReasonReact.stringToElement("ML")) </option>
    <option value="REO"> (ReasonReact.stringToElement("RE v1")) </option>
  </select>;

type state = {
  copyConfirmation: option(string),
  inputRef: option(ReasonReact.reactRef),
  inLanguage: Protocol.language,
  outLanguage: Protocol.language
};

let showCopyConfirmation = (text, (), {ReasonReact.state, update}) => {
  Util.setTimeout(update(((), _) => ReasonReact.Update({...state, copyConfirmation: None})), 2500);
  ReasonReact.Update({...state, copyConfirmation: Some(text)})
};

let updateInputRef = (nullableRef, {ReasonReact.state}) =>
  switch (Js.Null.to_opt(nullableRef)) {
  | Some(ref) => ReasonReact.SilentUpdate({...state, inputRef: Some(ref)})
  | None => ReasonReact.NoUpdate
  };

let component = ReasonReact.statefulComponent("PopupWindow");

let make =
    (
      ~inText,
      ~inLang,
      ~outText,
      ~outLang,
      ~link,
      ~onOpen,
      ~onInputChanged:
         (~inLang: Protocol.language=?, ~outLang: Protocol.language=?, string) => unit,
      _
    ) => {
  ...component,
  initialState: () => {
    copyConfirmation: None,
    inputRef: None,
    inLanguage: Refmt2.UnknownLang,
    outLanguage: Refmt2.UnknownLang
  },
  didMount: ({state}) => {
    switch state.inputRef {
    | None => ()
    | Some(ref) =>
      CodeMirror.focus(ref);
      CodeMirror.execCommand(ref, "selectAll")
    };
    ReasonReact.NoUpdate
  },
  render: ({state, update, handle}) => {
    let handleInLangChange = (e, {ReasonReact.state}) => {
      let inLanguage =
        e |> ReactEventRe.Synthetic.target |> LocalDom.Element.value |> Protocol.languageOfString;
      onInputChanged(~inLang=inLanguage, ~outLang=state.outLanguage, inText);
      ReasonReact.Update({...state, inLanguage})
    };
    let handleOutLangChange = (e, {ReasonReact.state}) => {
      let outLanguage =
        e |> ReactEventRe.Synthetic.target |> LocalDom.Element.value |> Protocol.languageOfString;
      onInputChanged(~inLang=state.inLanguage, ~outLang=outLanguage, inText);
      ReasonReact.Update({...state, outLanguage})
    };
    let handleInputChange = (input, {ReasonReact.state}) =>
      onInputChanged(~inLang=state.inLanguage, ~outLang=state.outLanguage, input);
    <div style=PopupStyles.popup>
      <div style=PopupStyles.popupColumn>
        <h1 style=PopupStyles.popupContext>
          <ColumnTitle
            lang=inLang
            select=(select("in", update(handleInLangChange), state.inLanguage, inLang))
          />
        </h1>
        <Editor
          value=inText
          lang=inLang
          inputRef=(update(updateInputRef))
          onChange=(handle(handleInputChange))
        />
      </div>
      <div style=PopupStyles.popupColumn>
        <h1 style=PopupStyles.popupContext>
          <ColumnTitle
            lang=outLang
            select=(select("out", update(handleOutLangChange), state.outLanguage, outLang))
          />
          <CopyButton
            style=PopupStyles.contextLink
            label="share"
            text=link
            onCopy=(update(showCopyConfirmation("Link copied to clipboard")))
          />
          <CopyButton
            style=PopupStyles.contextLink
            text=outText
            onCopy=(update(showCopyConfirmation("Text copied to clipboard")))
          />
          <OpenButton style=PopupStyles.contextIcon onClick=((_) => onOpen(inText)) />
        </h1>
        <Editor value=outText lang=outLang readOnly=true />
        <CopyConfirmation
          style=PopupStyles.copyConfirmation
          show=(Option.isSome(state.copyConfirmation))
          text=(Option.getOr("", state.copyConfirmation))
        />
      </div>
    </div>
  }
};
