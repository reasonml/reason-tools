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
  Chrome.Tabs.create { "url": ("popup.html#" ^ (Util.btoa text)) };

let refmt value updater => {
  open RefmtProtocol;
  /* this isn't guaranteed to be sync or speedy, so
   * don't set this.state.in here, since it could cause lag.
   */
   RefmtProtocol.send
    { input: value }
    (fun response =>
      switch response {
      | Failure error => updater error None None
      | Success { outText, inLang, outLang } =>
        updater outText (Some inLang) (Some outLang)
    });

  Chrome.Storage.Local.set { "latestRefmtString": value };
};

Document.addEventListener "DOMContentLoaded" (fun () =>
  ignore (Promise.all [|
    Promise.make (fun resolve => {
      Chrome.Tabs.executeScript
        { "code": "window.getSelection().toString()" }
        (fun maybeMaybeArray =>
          maybeMaybeArray
          |> Js.Null_undefined.to_opt
          |> Option.map (fun maybeArray => Array.unsafe_get maybeArray 0)
          |> resolve)
    }),
    Promise.make (fun resolve => {
      Chrome.Storage.Local.get
        "latestRefmtString"
        (fun response => resolve (Js.Null.to_opt response##latestRefmtString))
    })
  |]
  |> Promise.then_ (fun results => {
    let (maybeSelection, latestRefmtString) =
      switch results {
      | [| a, b |] => (a, b)
      | _ => (None, None);
      };
    let selection =
      maybeSelection |> Option.or_ latestRefmtString
                     |> Option.get_or (
                       Location.hash
                       |> Js.String.sliceToEnd 1
                       |> Util.atob
                     );

    ReactDOMRe.render
      <PopupWindow initialText=selection onOpen=open_ onRefmt=refmt />
      (ReasonJs.Document.getElementById "app");

    Js.Undefined.empty
  })));
