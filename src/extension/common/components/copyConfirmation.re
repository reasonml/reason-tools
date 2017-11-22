let iconStyle = ReactDOMRe.Style.make(~fontSize="16vh", ());

let textStyle = ReactDOMRe.Style.make(~fontSize="3vh", ~whiteSpace="nowrap", ~marginTop="4vh", ());

let component = ReasonReact.statelessComponent("CopyConfirmation");

let make = (~show, ~text="Text copied to clipboard", ~style=?, _) => {
  ...component,
  render: (_) =>
    show ?
      <div ?style>
        <div style=iconStyle> (ReasonReact.stringToElement(Js.String.fromCodePoint(0x2398))) </div>
        <div style=textStyle> (ReasonReact.stringToElement(text)) </div>
      </div> :
      ReasonReact.nullElement
};
