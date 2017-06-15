open Rebase;

open Core;

[%bs.raw {|require('../../../../../src/popup.html')|}];

[%bs.raw {|require('../../../../../src/images/logo19.png')|}];

[%bs.raw {|require('../../../../../src/images/logo38.png')|}];

[%bs.raw {|require('../../../../../src/images/logo19_gray.png')|}];

[%bs.raw {|require('../../../../../src/images/logo38_gray.png')|}];

[%bs.raw {|require('../../../../../src/images/logo128.png')|}];

[%bs.raw {|require('../../../../../src/css/codemirror.css')|}];

[%bs.raw {|require('../../../../../src/css/oceanic-next.css')|}];

[%bs.raw {|require('codemirror/mode/javascript/javascript')|}];

[%bs.raw {|require('codemirror/mode/mllike/mllike')|}];

let rejectedPromise () => Promise.make (fun _ reject => reject ());

let setHash hash => Core.History.replaceState state::[%bs.raw "{}"] title::"" url::hash;

let makeContentHash text => "#" ^ Util.btoa text;

let generateShareableLink text => "https://reasonml.github.io/reason-tools/popup.html" ^ text;

let getInputFromUrl () => {
  let text = LocalDom.Location.hash |> Js.String.sliceToEnd from::1 |> Util.atob;
  if (Str.isEmpty text) {
    Promise.reject ()
  } else {
    Promise.resolve text
  }
};

let init
    ::getSelection=rejectedPromise
    ::getLatestInput=rejectedPromise
    ::onOpen
    refmt::(
      refmt:
        Js.String.t =>
        inLang::Protocol.language? =>
        inType::Protocol.codeType? =>
        outLang::Protocol.language? =>
        (string => Protocol.language => Protocol.language => 'a) =>
        unit
    )
    () => {
  let rec inputChanged ::inLang=Protocol.UnknownLang ::outLang=Protocol.UnknownLang input => {
    let hash = makeContentHash input;
    let link = generateShareableLink hash;
    setHash hash;
    refmt
      input
      ::inLang
      ::outLang
      (fun outText inLang outLang => render input outText inLang outLang link)
  }
  and render inText outText inLang outLang link =>
    ReactDOMRe.renderToElementWithId
      <PopupWindow inText inLang outText outLang link onOpen onInputChanged=inputChanged /> "app";
  Promise.(
    getInputFromUrl ()
    |> or_else getSelection
    |> or_else getLatestInput
    |> or_ (fun _ => "")
    |> then_ inputChanged
    |> ignore
  )
};
