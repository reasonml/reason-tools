
let component = ReasonReact.statelessComponent "CopyButton";
let make ::label="copy" ::text ::onCopy ::style=? _ => {
  ...component,

  render: fun _ _ =>
    <CopyToClipboard text onCopy>
      <span className="copy-button" style=?style> (ReactRe.stringToElement label) </span>
    </CopyToClipboard>
};