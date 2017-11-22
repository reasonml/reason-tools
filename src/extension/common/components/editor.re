let component = ReasonReact.statelessComponent("Editor");

let make = (~value, ~lang, ~defaultValue=?, ~readOnly=false, ~inputRef=?, ~onChange=?, _) => {
  ...component,
  render: (_) =>
    <CodeMirror
      value
      ref=?inputRef
      ?defaultValue
      ?onChange
      options={
        "mode": lang == Refmt2.ML ? "text/x-ocaml" : "javascript",
        "theme": "oceanic-next",
        "readOnly": Js.Boolean.to_js_boolean(readOnly)
      }
    />
};
