let component = ReasonReact.statelessComponent("Editor");

let make = (~value, ~autoFocus=?, ~editorDidMount=?, ~lang, ~defaultValue=?, ~readOnly=false, ~onChange=?, _) => {
  ...component,
  render: (_) =>
    <CodeMirror
      value
      ?editorDidMount
      ?autoFocus
      ?defaultValue
      ?onChange
      options={
        "mode": lang == RefmtShared.ML ? "text/x-ocaml" : "javascript",
        "theme": "oceanic-next",
        "readOnly": Js.Boolean.to_js_boolean(readOnly)
      }
    />
};
