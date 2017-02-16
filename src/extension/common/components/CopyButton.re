let createElement ::text ::onCopy ::style=? children::_ () =>
  <CopyToClipboard text onCopy>
    <a className="copy-button" style=?style href="#"> (ReactRe.stringToElement "copy") </a>
  </CopyToClipboard>
