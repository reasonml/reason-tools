
let createElement ::text ::onCopy ::children () =>
  <CopyToClipboard text onCopy>
    <a style=Styles.contextLink href="#"> (ReactRe.stringToElement "copy") </a>
  </CopyToClipboard>
