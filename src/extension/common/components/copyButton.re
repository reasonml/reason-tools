
let make ::label="copy" ::text ::onCopy ::style=? _ => {
  ...(ReasonReact.statelessComponent "CopyButton"),

  render: fun _ _ =>
    <CopyToClipboard text onCopy>
      <span className="copy-button" style=?style> (ReactRe.stringToElement label) </span>
    </CopyToClipboard>
};