let component = ReasonReact.statelessComponent("CopyButton");

let make = (~label="copy", ~text, ~onCopy, ~style=?, _) => {
  ...component,
  render: (_) =>
    <CopyToClipboard text onCopy>
      <span className="copy-button" ?style> (ReasonReact.stringToElement(label)) </span>
    </CopyToClipboard>
};
