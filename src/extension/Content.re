open Core;
open Core.Dom;
open Common;

Hljs.registerLanguage "ocaml" [%bs.raw "require('highlight.js/lib/languages/ocaml')"];
Hljs.configure { "classPrefix": "", "languages": [| "ocaml" |] };

let stylesheet = Document.createElement "link";

let css = [%bs.raw {|require('../../../../src/css/ocamlDoc.css')|}];

Element.setType stylesheet "text/css";
Element.setRel stylesheet "stylesheet";
Element.setHref stylesheet (Chrome.Extension.getURL css);

type saveState = {
  mutable stylesheets: list Element.t,
};
let savedState = { stylesheets: [stylesheet] };

let swapStyleSheets _ => {
  let stylesheets =
    getElementsByTagName None "link"
    |> List.filter (fun link => (Element.getAttribute link "rel") == "stylesheet");

  stylesheets
    |> List.iter Element.remove;

  savedState.stylesheets
    |> List.iter Head.appendChild;

  savedState.stylesheets = stylesheets;
};

let readjustViewport () =>
  if (not (Str.isEmpty Location.hash)) {
    [%bs.raw {| window.location.href = window.location.href |}];
  };

type swapState = { mutable remaining: int };

let doListing mode state listing => {
  open Retrieve;
  let { els, text, replace } = listing;

  RefmtProtocol.send
    { input: normalizeText text }
    (fun response => {
      switch response {
        | RefmtProtocol.Failure _ => ()
        | RefmtProtocol.Success { outText } => Replace.replaceListing els outText replace
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

let toggle () => {
  swapStyleSheets ();
  UI.toggle swapStyleSheets swapSyntax;
  swapSyntax `initial;
};

if (Detect.mightBeOcamlDoc ()) {
  toggle ();
};

Message.receive "content:toggle" (fun _ _ _ => toggle ());

let open_ text =>
  Message.send "background:open" text;

Message.receive "content:refmt-selection" (fun _ _ _ => {
  let selection = Window.getSelection ();
  let text = selection |> Selection.toString |> normalizeText;

  Selection.removeAllRanges selection;

  RefmtProtocol.send
    { input: text }
    (fun response => {
      switch response {
        | Failure _ => ()
        | Success { outText, inLang, outLang } => {
          let host = Document.createElement "div";
          let shadow = Element.attachShadow host { "mode": "closed" };

          Body.appendChild host;

          /* React must render directly to the shadow root, otherwise onclick handlers won't work */
          ReactDOMRe.render
            <InlinePopover
              inLang inText=text
              outLang outText
              close=(fun () => Body.removeChild host)
              open_=open_
            />
            (Element.toReasonJsElement shadow);

          /* Only after React has rendered can we attach the stylesheets, otherwise React will render over them */
          Element.appendChild shadow (Node.cloneNode stylesheet);
          let style = Document.createElement "style";
          Element.setType style "text/css";
          Element.setInnerText style InlineStyles.stylesheet;
          Element.appendChild shadow style;
        }
      };
    }
  )

});
