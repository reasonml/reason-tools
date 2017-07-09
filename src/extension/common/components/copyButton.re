
let component = ReasonReact.statelessComponent "CopyButton";
let make ::label="copy" ::text ::onCopy ::style=? _ => {
  ...component,

  render: fun _ =>
    <CopyToClipboard text onCopy>
      <span className="copy-button" style=?style> (ReasonReact.stringToElement label) </span>
    </CopyToClipboard>
};
