let iconStyle = {
  "fontSize": "16vh"
};
let textStyle = {
  "fontSize": "3vh",
  "whiteSpace": "nowrap",
  "marginTop": "4vh",
};

let createElement ::show ::text="Text copied to clipboard" ::style=? children::_ () =>
  (show ?
    <div style=?style>
      <div style=iconStyle> (ReactRe.stringToElement (Js.String.fromCodePoint 0x2398)) </div>
      <div style=textStyle> (ReactRe.stringToElement text) </div>
    </div> :
    ReactRe.nullElement)
