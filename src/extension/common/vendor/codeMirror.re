type editor;

[@bs.module "react-codemirror2"] external codeMirror : ReasonReact.reactClass = "Controlled";
[@bs.send] external execCommand : (editor, string) => unit = "";

let make =
    (
      ~autoFocus: option(bool)=?,
      ~style: option(Js.t({..}))=?,
      ~value: option(string)=?,
      ~editorDidMount: option(editor => unit)=?,
      ~onChange: option(string => unit)=?,
      ~options: option(Js.t({..}))=?,
      children
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=codeMirror,
    ~props={
      "autoFocus": switch (autoFocus) {
      | Some(true) => Js.true_
      | Some(false) | None => Js.false_
      },
      "style": Js.Undefined.fromOption(style),
      "value": Js.Undefined.fromOption(value),
      "onBeforeChange": (_editor, _data, value) => switch (onChange) {
      | Some(onChange) => onChange(value)
      | None => ()
      },
      "editorDidMount": Js.Undefined.fromOption(editorDidMount),
      "options": Js.Undefined.fromOption(options)
    },
    children
  );
