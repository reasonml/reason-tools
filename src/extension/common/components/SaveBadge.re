open Core;

let createElement ::show ::style=? children::_ () =>
  (show ?
    <div style=?style> (ReactRe.stringToElement (Str.fromCharCode 10003)) </div> :
    ReactRe.nullElement)
