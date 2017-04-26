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
        inLang::string? =>
        inType::string? =>
        outLang::string? =>
        (string => option string => option string => 'a) =>
        unit
    )
    () => {
  let rec inLangChanged input e => {
    let hash = makeContentHash input;
    let link = generateShareableLink hash;
    let inLang = e |> ReactEventRe.Synthetic.target |> LocalDom.Element.value;
    refmt input ::inLang (fun outText inLang outLang => render input outText inLang outLang link)
  }
  and outLangChanged input e => {
    let hash = makeContentHash input;
    let link = generateShareableLink hash;
    let outLang = e |> ReactEventRe.Synthetic.target |> LocalDom.Element.value;
    refmt input ::outLang (fun outText inLang outLang => render input outText inLang outLang link)
  }
  and inputChanged ::inLang="unknown" ::outLang="unknown" input => {
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
      <PopupWindow
        inText
        inLang
        outText
        outLang
        link
        onOpen
        onInputChanged=inputChanged
        inLangChanged
        outLangChanged
      />
      "app";
  Promise.(
    getInputFromUrl () |> or_else getSelection |> or_else getLatestInput |> or_ (fun _ => "") |>
    then_ inputChanged |> ignore
  )
};
