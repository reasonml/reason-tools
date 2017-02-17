open Core;
open Core.Dom;

[%bs.raw {|require('../../../../src/popup.html')|}];
[%bs.raw {|require('../../../../src/images/logo19.png')|}];
[%bs.raw {|require('../../../../src/images/logo38.png')|}];
[%bs.raw {|require('../../../../src/images/logo128.png')|}];
[%bs.raw {|require('../../../../src/css/codemirror.css')|}];
[%bs.raw {|require('../../../../src/css/oceanic-next.css')|}];
[%bs.raw {|require('codemirror/mode/javascript/javascript')|}];
[%bs.raw {|require('codemirror/mode/mllike/mllike')|}];

let open_ text =>
  Message.send "open" text;

let refmt value updater => {
  /* this isn't guaranteed to be sync or speedy, so
   * don't set this.state.in here, since it could cause lag.
   */
   RefmtProtocol.send
    { input: value }
    (fun
      | RefmtProtocol.Failure error => updater error None None
      | RefmtProtocol.Success { outText, inLang, outLang } =>
        updater outText (Some inLang) (Some outLang)
    );

  Chrome.Storage.Local.set { "latestRefmtString": value };
};

let getSelection () =>
  Promise.make (fun resolve reject =>
    Chrome.Tabs.executeScript
      { "code": "window.getSelection().toString()" }
      (fun maybeMaybeArray =>
        maybeMaybeArray
          |> Js.Null_undefined.to_opt
          |> Option.map (fun maybeArray => Array.unsafe_get maybeArray 0)
          |> Option.and_then (fun s => Str.isEmpty s ? None : Some s)
          |> Option.map_or_else resolve reject
      )
  );

let getLatestInput () =>
  Promise.make (fun resolve reject =>
    Chrome.Storage.Local.get
      "latestRefmtString"
      (fun response =>
        response##latestRefmtString
          |> Js.Null.to_opt
          |> Option.map_or_else resolve reject
      )
  );

let getInputFromUrl () => {
   let text = Location.hash
    |> Js.String.sliceToEnd from::1
    |> Util.atob;

  if (Str.isEmpty text) {
    Promise.reject ()
  } else {
    Promise.resolve text
  }
};

let render input =>
    ReactDOMRe.render
      <PopupWindow initialText=input onOpen=open_ onRefmt=refmt />
      (ReasonJs.Document.getElementById "app");

let init _ => {
  open Promise;

  getInputFromUrl ()
    |> or_else getSelection
    |> or_else getLatestInput
    |> or_ (fun _=> "")
    |> then_ render
    |> ignore;
};

Document.addEventListener "DOMContentLoaded" init;
