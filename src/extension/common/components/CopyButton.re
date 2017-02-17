let createElement ::label="copy" ::text ::onCopy ::style=? children::_ () =>
  <CopyToClipboard text onCopy>
    <a className="copy-button" style=?style href="#"> (ReactRe.stringToElement label) </a>
  </CopyToClipboard>
