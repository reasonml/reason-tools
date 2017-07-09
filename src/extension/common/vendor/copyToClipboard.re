
external copyToClipboard: ReasonReact.reactClass = "react-copy-to-clipboard" [@@bs.module];

let make
  text::(text: string)
  onCopy::(onCopy: unit => unit)
  children =>
  ReasonReact.wrapJsForReason
    reactClass::copyToClipboard
    props::{
      "text": text,
      "onCopy": onCopy
    }
    children;
