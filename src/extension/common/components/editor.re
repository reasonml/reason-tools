
let make ::value ::lang ::defaultValue=? ::readOnly=false ::inputRef=? ::onChange=? _ => {
  ...(ReasonReact.statelessComponent "Editor"),

  render: fun _ _ =>
    <CodeMirror
      value=value
      inputRef=?inputRef
      defaultValue=?defaultValue
      onChange=?onChange
      options={
        "mode": lang == Protocol.ML ? "text/x-ocaml" : "javascript",
        "theme": "oceanic-next",
        "readOnly": Js.Boolean.to_js_boolean readOnly
      }
    />
}; 