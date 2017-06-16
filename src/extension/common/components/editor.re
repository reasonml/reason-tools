
let component = ReasonReact.statelessComponent "Editor";
let make ::value ::lang ::defaultValue=? ::readOnly=false ::inputRef=? ::onChange=? _ => {
  ...component,

  render: fun _ _ =>
  /*
    <CodeMirror
      value=value
      ref=?inputRef
      defaultValue=?defaultValue
      onChange=?onChange
      options={
        "mode": lang == Protocol.ML ? "text/x-ocaml" : "javascript",
        "theme": "oceanic-next",
        "readOnly": Js.Boolean.to_js_boolean readOnly
      }
    />
  */
    ReasonReact.element ref::?inputRef (
      CodeMirror.make 
        value::value
        defaultValue::?defaultValue
        onChange::?onChange
        options::{
          "mode": lang == Protocol.ML ? "text/x-ocaml" : "javascript",
          "theme": "oceanic-next",
          "readOnly": Js.Boolean.to_js_boolean readOnly
        }
        [||]
      )  
}; 