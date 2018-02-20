type editor;

[@bs.module "react-codemirror2"] external codeMirror : ReasonReact.reactClass = "Controlled";
[@bs.module "react-codemirror2"] external codeMirrorUncontrolled : ReasonReact.reactClass = "UnControlled";
[@bs.send] external execCommand : (editor, string) => unit = "";

let make =
    (
      ~autoFocus: option(bool)=?,
      ~style: option(Js.t({..}))=?,
      ~value: option(string)=?,
      ~defaultValue: option(string)=?,
      ~editorDidMount: option(editor => unit)=?,
      ~onChange: option(string => unit)=?,
      ~options: option(Js.t({..}))=?,
      children
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=(switch (defaultValue) {
    | None => codeMirror
    | Some(_) => codeMirrorUncontrolled
    }),
    ~props={
      "autoFocus": switch (autoFocus) {
      | Some(true) => Js.true_
      | Some(false) | None => Js.false_
      },
      "style": Js.Undefined.from_opt(style),
      "value": switch (defaultValue) {
      | None => Js.Undefined.from_opt(value)
      | Some(default) => Js.Undefined.return(default)
      },
      "onChange": (_editor, _data, value) => switch (onChange) {
      | Some(onChange) => onChange(value)
      | None => ()
      },
      "editorDidMount": Js.Undefined.from_opt(editorDidMount),
      "options": Js.Undefined.from_opt(options)
    },
    children
  );
