open Rebase;
open Core;
open Dom;

[%bs.raw {|require('../../../../src/popup.html')|}];
[%bs.raw {|require('../../../../src/images/logo19.png')|}];
[%bs.raw {|require('../../../../src/images/logo38.png')|}];
[%bs.raw {|require('../../../../src/images/logo128.png')|}];
[%bs.raw {|require('../../../../src/css/codemirror.css')|}];
[%bs.raw {|require('../../../../src/css/oceanic-next.css')|}];
[%bs.raw {|require('codemirror/mode/javascript/javascript')|}];
[%bs.raw {|require('codemirror/mode/mllike/mllike')|}];

let open_ text =>
  Message.send "background:open" text;

let generateShareableLink text =>
  "https://reasonml.github.io/reason-tools/popup.html#" ^ (Util.btoa text);

let getSelection () =>
  Promise.make (fun resolve reject =>
    Chrome.Tabs.executeScript
      { "code": "window.getSelection().toString()" }
      (fun maybeMaybeArray =>
        maybeMaybeArray
          |> Js.Null_undefined.to_opt
          |> Option.map (fun maybeArray => Array.unsafe_get maybeArray 0)
          |> Option.andThen (fun s => Str.isEmpty s ? None : Some s)
          |> Option.mapOrElse resolve reject
      )
  );

let getLatestInput () =>
  Promise.make (fun resolve reject =>
    Chrome.Storage.Local.get
      "latestRefmtString"
      (fun response =>
        response##latestRefmtString
          |> Js.Null.to_opt
          |> Option.mapOrElse resolve reject
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

let rec inputChanged input => {
  let link = generateShareableLink input;
  /* this isn't guaranteed to be sync or speedy, so
   * don't set this.state.in here, since it could cause lag.
   */
   RefmtProtocol.send
    { input: input }
    (fun
      | RefmtProtocol.Failure error =>
        render input error None None link
      | RefmtProtocol.Success { outText, inLang, outLang } =>
        render input outText (Some inLang) (Some outLang) link
    );

  Chrome.Storage.Local.set { "latestRefmtString": input };
}

and render inText outText inLang outLang link =>
    ReactDOMRe.render
      <PopupWindow
        inText
        inLang
        outText
        outLang
        link
        onOpen=open_
        onInputChanged=inputChanged
      />
      (ReasonJs.Document.getElementById "app");

let init _ => {
  open Promise;

  getInputFromUrl ()
    |> or_else getSelection
    |> or_else getLatestInput
    |> or_ (fun _=> "")
    |> then_ inputChanged
    |> ignore;
};

Document.addEventListener "DOMContentLoaded" init;
