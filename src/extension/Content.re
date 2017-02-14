open Core;
open Core.Dom;
open Common;

Hljs.registerLanguage "ocaml" [%bs.raw "require('highlight.js/lib/languages/ocaml')"];
Hljs.configure { "classPrefix": "", "languages": [| "ocaml" |] };

type saveState = {
  mutable stylesheets: list Element.t,
  mutable buttons: list Element.t
};
let savedState = (fun () => {
  let css = [%bs.raw {|require('../../../../src/css/ocamlDoc.css')|}];
  let stylesheet = Document.createElement "link";

  Element.setType stylesheet "text/css";
  Element.setRel stylesheet "stylesheet";
  Element.setHref stylesheet (Chrome.Extension.getURL css);

  { stylesheets: [stylesheet], buttons: [] }
}) ();

let swapStyleSheets => {
  let stylesheets =
    getElementsByTagName None "link"
    |> List.filter (fun link => (Element.getAttribute link "rel") == "stylesheet");

  stylesheets
    |> List.iter Element.remove;

  savedState.stylesheets
    |> List.iter Head.appendChild;

  savedState.stylesheets = stylesheets;
};

let readjustViewport =>
  if (not (Str.isEmpty Location.hash)) {
    [%bs.raw {| window.location.href = window.location.href |}];
  };

type swapState = { mutable remaining: int };

let doListing mode state listing => {
  open RefmtProtocol;
  open Retrieve;
  let { els, text, replace } = listing;

  RefmtProtocol.send
    { input: normalizeText text }
    (fun response => {
      switch response {
        | Failure _ => ()
        | Success { outText } => Replace.replaceListing els outText replace
      };

      /* we're in an async callback, so keep track of when we're finished by keeping count */
      state.remaining = state.remaining - 1;
      if (mode == `initial && state.remaining <= 0) {
        /* when we're done, the DOM has most likely been shifted,
         * so we need to go back to where we're supposed to be */
        readjustViewport()
      }
    });
};

let swapSyntax mode => {
  let listings = Retrieve.getListings ();
  let state = {
    remaining: List.length listings
  };

  listings
    |> List.iter (doListing mode state);

  UI.updateSyntaxSwapButton ();
};

let toggleButtons =>
  if (List.length savedState.buttons > 0) {
    savedState.buttons
      |> List.iter Element.remove;
  } else {
    savedState.buttons = UI.addSwapButtons swapStyleSheets swapSyntax;
  };

let toggle => {
  swapStyleSheets ();
  toggleButtons ();
  swapSyntax `initial;
};

if (Detect.mightBeOcamlDoc ()) {
  toggle ();
};

Message.receive "toggle" (fun _ _ => toggle ());
