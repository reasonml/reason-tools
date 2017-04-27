
let createElement ::value ::lang ::defaultValue=? ::readOnly=false ::ref=? ::onChange=? children::_ () =>
  <CodeMirror
    value=value
    ref=?ref
    defaultValue=?defaultValue
    onChange=?onChange
    options={
      "mode": lang == Protocol.ML ? "text/x-ocaml" : "javascript",
      "theme": "oceanic-next",
      "readOnly": Js.Boolean.to_js_boolean readOnly
    }
  />
