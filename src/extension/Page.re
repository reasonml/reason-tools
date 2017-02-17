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

let open_: string => unit = [%bs.raw
  {|
  function (hash) {
    window.open(
      window.location.href,
      "_blank"
    );
  }
|}
];

let setHash: string => unit = [%bs.raw
  {|
  function (hash) {
    window.location.hash = window.btoa(hash);
  }
|}
];

exception DeserializationFail;

type request = {input: string};

type payload = {outText: string, inLang: string, outLang: string};

let refmt value updater =>
  ignore (
    /* the chrome api is async and we make some assumptions about
     * it always being async that are easier to just satisfy than
     * to account for */
    ReasonJs.setTimeout
      (
        fun () => {
          switch (Background.Refmt.refmt value) {
          | ("Failure", error) => updater error None None
          | (conversion, outText) =>
            switch (conversion |> Js.String.split "to") {
            | [|inLang, outLang|] => updater outText (Some inLang) (Some outLang)
            | _ => ()
            }
          };
          setHash value
        }
      )
      0
  );

let getSelection () => Promise.make (fun _ reject => reject ());

let getInputFromUrl () => {
  let text = Location.hash |> Js.String.sliceToEnd from::1 |> Util.atob;
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

let init _ =>
  Promise.(
    getInputFromUrl () |> or_else getSelection |> or_ (fun _ => "") |> then_ render |> ignore
  );

Document.addEventListener "DOMContentLoaded" init;
