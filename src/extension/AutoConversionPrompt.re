open Core;
open Core.Dom;

let ocamlDocCss = [%bs.raw {|require('../../../../src/css/ocamlDoc.css')|}];
let ocamlLogo = [%bs.raw {|require('../../../../src/images/ocamlLogo128.png')|}];
let reasonLogo = [%bs.raw {|require('../../../../src/images/logo128.png')|}];

Hljs.registerLanguage "ocaml" [%bs.raw "require('highlight.js/lib/languages/ocaml')"];
Hljs.configure { "classPrefix": "", "languages": [| "ocaml" |] };

let ocamlDocStyleTag = Document.createElement "link";
let syntaxSwap = Document.createElement "div";
let cssSwap = Document.createElement "div";

Element.setType ocamlDocStyleTag "text/css";
Element.setRel ocamlDocStyleTag "stylesheet";
Element.setHref ocamlDocStyleTag (Chrome.Extension.getURL ocamlDocCss);

type backBufferT = {
  mutable stylesheets: array Element.t,
  mutable body: string
};

let backBuffer: backBufferT = {
  stylesheets: [| ocamlDocStyleTag |],
  body: Body.outerHTML
};

let hasClassName className => {
  let els = NotArray.toArray (Document.getElementsByClassName className);
  Array.length els > 0;
};

let ocamlishRels = [|
  "Start",
  "previous",
  "next",
  "Up",
  "Appendix",
  "Section",
  "Subsection"
|];

let hasOcamlRels =>
  (NotArray.toArray (Document.getElementsByTagName "link")
  |> Array.map (fun link => Js.Array.includes (Element.getAttribute link "rel") ocamlishRels)
  |> Js.Array.filter ((fun b => Js.to_bool b) [@bs])
  |> Array.length) >= 3;

let hasCommonClassNames =>
  [
    "keyword",
    "type",
    "deprecated",
    "mod",
    "modt",
    "typ",
    "spec",
    "def",
    "ext",
    "exn",
    "cls",
    "include",
    "cons",
    "paramstable",
    "sig_block"
  ]
  |> List.map hasClassName
  |> List.for_all (fun b => b);

let hasUniqueClassNames =>
  [
    "odoc-doc",
    "package-index"
  ]
  |> List.map hasClassName
  |> List.for_all (fun b => b);

let mightBeOcamlDoc =>
  hasUniqueClassNames () || hasOcamlRels () || hasCommonClassNames ();

let swapStyleSheets => {
  let stylesheets =
    NotArray.toArray (Document.getElementsByTagName "link")
    |> Js.Array.filter ((fun link => (Element.getAttribute link "rel") == "stylesheet") [@bs]);

  Js.Array.forEach ((fun el => Element.remove el) [@bs]) stylesheets;
  backBuffer.stylesheets |> Js.Array.forEach ((fun el => Head.appendChild el) [@bs]);
  backBuffer.stylesheets = stylesheets;
};

let normalizeText text =>
  text
  |> Js.String.trim
  |> Js.String.replaceByRe [%bs.re "/[^\x00-\x7F]/g"] " "
  |> Js.String.replace (Str.fromCharCode 65533) "";

let getNormalizedLinks els =>
  els
  |> List.map (fun el => NotArray.toArray (Element.getElementsByTagName el "a") |> Array.to_list)
  |> List.flatten
  |> List.map (fun a => (Element.innerText a, Element.href a));

let getNormalizedIds els =>
  els
  |> List.map (fun el => NotArray.toArray (Element.querySelectorAll el "[id]") |> Array.to_list)
  |> List.flatten
  |> List.map (fun span => {
      let words = normalizeText (Element.innerText span) |> Js.String.split " ";
      let lastWord = Array.get words ((Array.length words) - 1);
      (lastWord, Element.id span)
  });

let escapeRegExp (str: string): string =>
  str |> Js.String.replaceByRe [%bs.re "/[\-\[\]\/\{\}\(\)\*\+\?\.\\\^\$\|]/g"] "\\$&";

type typeTable = {
  el: Element.t,
  text: string,
  remove: unit => unit
};

let getTypeTable pre =>
  switch (Js.Null.to_opt (Element.nextSibling pre)) {
    | None => None
    | Some el =>
      if (Js.to_bool (DOMTokenList.contains (Element.classList el) "typetable")) {
        let text = Element.innerText el;

        switch (Js.Null.to_opt (Element.nextSibling el)) {
          | None => Some { el, text, remove: (fun () => Element.remove el) }
          | Some next =>
            if (Node.nodeType el == Node._TEXT_NODE) {
              Some { el, text: text ^ (Node.nodeValue next), remove: (fun () => { Element.remove el; Element.remove next }) }
            } else {
                Some { el, text, remove: (fun () => Element.remove el) }
            }
        }
      } else {
        None
      }
  };

type target = {
  els: list Element.t,
  text: string,
  replace: string => Element.t
};

let getPreTargets =>
  NotArray.toArray (Document.getElementsByTagName "pre")
  |> Array.map (fun el => {
    switch (getTypeTable el) {
      | Some typeTable => {
        els: [el, typeTable.el],
        text: (Element.innerText el ^ typeTable.text),
        replace: (fun html => {
          Element.setInnerHTML el html;
          typeTable.remove();
          el
        })
      }
      | None => {
        els: [el],
        text: Element.innerText el,
        replace: (fun html => { Element.setInnerHTML el html; el })
      }
    }
  });

let getDefTargets =>
  NotArray.toArray (Document.getElementsByClassName "def")
  |> Array.map (fun el => {
    els: [el],
    text: Element.innerText el,
    replace: (fun html => {
      Element.setInnerHTML el ("<pre>" ^ html ^ "</pre>");
      el
    })
  });

let getLstListingTargets =>
  NotArray.toArray (Document.getElementsByClassName "lstlisting")
  |> Array.map (fun el => {
    els: [el],
    text: Element.innerText el,
    replace: (fun html => {
      let parent = Node.parentNode el;
      Element.setInnerHTML parent ("<pre>" ^ html ^ "</pre>");
      parent
    })
  });

let getCodeTargets =>
  NotArray.toArray (Document.getElementsByTagName "code")
  |> Array.map (fun el => {
    els: [el],
    text: Element.innerText el,
    replace: (fun html => {
      Element.setInnerHTML el html;
      el
    })
  });

let getTargets =>
  [
    getPreTargets,
    getDefTargets,
    getLstListingTargets,
    getCodeTargets
  ]
  |> List.map (fun f => f ())
  |> List.map Array.to_list
  |> List.flatten;

let replaceHrefs hrefs out =>
  hrefs
  |> List.fold_left (fun out (text, href) =>
    Js.String.replaceByRe
      (Re.make ("/\\b" ^ (escapeRegExp text) ^ "\\b/g"))
      ("<a href='" ^ href ^"'>" ^ text ^ "</span>")
      out
    ) out;

let replaceIds ids out =>
  ids
  |> List.fold_left (fun out (text, id) =>
    Js.String.replaceByRe
      (Re.make ("/\\b" ^ (escapeRegExp text) ^ "\\b/g"))
      ("<span class='reason_tools_anchor' id='" ^ id ^"'>" ^ text ^ "</span>")
      out
    ) out;

let readjustViewport =>
  if ((String.length Location.hash) > 0) {
    Location.reload (Js.Boolean.to_js_boolean true);
  };

let updateSwapButton => {
  let style = Element.style syntaxSwap;
  let reasonLogoUrl = "url(" ^ (Chrome.Extension.getURL reasonLogo) ^ ")";
  let ocamlLogoUrl = "url(" ^(Chrome.Extension.getURL ocamlLogo) ^ ")";

  Style.setBackgroundImage style
    (Js.to_bool (Str.includes (Style.backgroundImage style) reasonLogo) ? ocamlLogoUrl : reasonLogoUrl);
};

type state = { mutable remaining: int };

let swapSyntax type_ => {
  let targets = getTargets ();
  let state : state = {
    remaining: List.length targets
  };

  ignore (targets
  |> List.map (fun { els, text, replace } => {
    open RefmtProtocol;
    let hrefs = getNormalizedLinks els;
    let ids = getNormalizedIds els;

    RefmtProtocol.send
      { input: normalizeText text }
      (fun maybeResponse => {
        switch maybeResponse {
          | Failure _ => ()
          | Success { outText } => {
            let el =
              outText |> replaceHrefs hrefs
                      |> replaceIds ids
                      |> replace;

            Hljs.highlightBlock el;
          }
        };

        /* we're in an async callback, so keep track of when we're finished by keeping count */
        state.remaining = state.remaining - 1;
        if (type_ == "initial" && state.remaining <= 0) {
          /* when we're done, the DOM has most likely been shifted,
           * so we need to go back to where we're supposed to be */
          readjustViewport()
        }
      });

      updateSwapButton ();
  }));
};

let addSwappers => {
  let styleTag = Document.createElement "style";
  Element.setType styleTag "text/css";
  Element.setInnerText styleTag {|
    .reason_tools_button.reason_tools_button.reason_tools_button {
      position: fixed;
      right: 0;
      height: 50px;
      width: 50px;
      background-color: black;
      color: white;
      font-family: monospace;
      display: flex;
      justify-content: center;
      align-items: center;
      font-weight: 900;
      opacity: 0.6;
    }
    .reason_tools_button.reason_tools_button.reason_tools_button:hover {
      opacity: 1;
      cursor: pointer;
    }
    .reason_tools_anchor {
      color: #cec47f;
    }
    .reason_tools_anchor:before {
      content: '';
      float: left;
      position: relative;
      width: 0;
      height: 50px;
      margin-top: -50px;
    }
    .reason_tools_anchor:target:after {
      content: '';
      position: relative;
      width: 4px;
      margin-left: -4px;
      height: 18px;
      float: left;
      background-color: #97B98c;
      left: -10px;
    }
  |};

  Body.appendChild styleTag;

  {
    open Element;

    Style.setTop (style cssSwap) "90px";
    setInnerText cssSwap "</>";
    setClassName cssSwap "reason_tools_button";
    setOnClick cssSwap swapStyleSheets;
    Body.appendChild cssSwap;

    Style.setTop (style syntaxSwap) "40px";
    setClassName syntaxSwap "reason_tools_button";
    setOnClick syntaxSwap (fun () => swapSyntax "not initial");
    Style.setBackgroundImage (style syntaxSwap) ("url(" ^ (Chrome.Extension.getURL reasonLogo) ^ ")");
    Style.setBackgroundSize (style syntaxSwap) "cover";
    Body.appendChild syntaxSwap;
  };
};

if (mightBeOcamlDoc ()) {
  swapStyleSheets ();
  swapSyntax "initial";
  addSwappers ();
};
