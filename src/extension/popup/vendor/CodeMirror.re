external codeMirror: ReactRe.reactClass = "react-codemirror" [@@bs.module];

let createElement
  style::(style: option (Js.t {..}))=?
  value::(value: option string)=?
  defaultValue::(defaultValue: option string)=?
  onChange::(onChange: option (string => unit))=?
  options::(options: option (Js.t {..}))=?  =>
  ReactRe.wrapPropsShamelessly codeMirror {
    "style": Js.Null_undefined.from_opt style,
    "value": Js.Null_undefined.from_opt value,
    "defaultValue": Js.Null_undefined.from_opt defaultValue,
    "onChange": Js.Null_undefined.from_opt onChange,
    "options": Js.Null_undefined.from_opt options
  };

external focus : ReactRe.reactRef => unit = "" [@@bs.send];
let execCommand : ReactRe.reactRef => string => unit = [%bs.raw {|
  function (el, command) {
    return el.getCodeMirror().execCommand(command);
  }
|}];
