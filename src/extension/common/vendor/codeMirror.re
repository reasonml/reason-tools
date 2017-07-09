external codeMirror: ReasonReact.reactClass = "react-codemirror" [@@bs.module];

external focus : ReasonReact.reactRef => unit = "" [@@bs.send];
let execCommand : ReasonReact.reactRef => string => unit = [%bs.raw {|
  function (el, command) {
    return el.getCodeMirror().execCommand(command);
  }
|}];

let make
  style::(style: option (Js.t {..}))=?
  value::(value: option string)=?
  defaultValue::(defaultValue: option string)=?
  onChange::(onChange: option (string => unit))=?
  options::(options: option (Js.t {..}))=?
  children =>
  ReasonReact.wrapJsForReason
    reactClass::codeMirror
    props::{
      "style": Js.Null_undefined.from_opt style,
      "value": Js.Null_undefined.from_opt value,
      "defaultValue": Js.Null_undefined.from_opt defaultValue,
      "onChange": Js.Null_undefined.from_opt onChange,
      "options": Js.Null_undefined.from_opt options
    }
    children;
