
external copyToClipboard: ReactRe.reactClass = "react-copy-to-clipboard" [@@bs.module];

let createElement
  text::(text: string)
  onCopy::(onCopy: unit => unit) =>
  ReactRe.wrapPropsShamelessly copyToClipboard {
    "text": text,
    "onCopy": onCopy
  };
