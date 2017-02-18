let createElement ::label="copy" ::text ::onCopy ::style=? children::_ () =>
  <CopyToClipboard text onCopy>
    <span className="copy-button" style=?style> (ReactRe.stringToElement label) </span>
  </CopyToClipboard>;
