open Core;

let createElement ::show ::children () =>
  (show ?
    <div style=Styles.savedBadge> (ReactRe.stringToElement (Str.fromCharCode 10003)) </div> :
    ReactRe.nullElement)
