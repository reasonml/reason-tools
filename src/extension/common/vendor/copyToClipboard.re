[@bs.module] external copyToClipboard : ReasonReact.reactClass = "react-copy-to-clipboard";

let make = (~text: string, ~onCopy: unit => unit, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=copyToClipboard,
    ~props={"text": text, "onCopy": onCopy},
    children
  );
