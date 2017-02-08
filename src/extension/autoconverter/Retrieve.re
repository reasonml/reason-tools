open Core;
open Core.Dom;
open Common;

type typeTable = {
  el: Element.t,
  text: string,
  remove: unit => unit
};
type listing = {
  els: list Element.t,
  text: string,
  replace: string => Element.t
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

let getPreListings =>
  getElementsByTagName None "pre"
  |> List.map (fun el => {
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

let getDefListings =>
  getElementsByClassName None "def"
  |> List.map (fun el => {
    els: [el],
    text: Element.innerText el,
    replace: (fun html => {
      Element.setInnerHTML el ("<pre>" ^ html ^ "</pre>");
      el
    })
  });

let getLstListings =>
  getElementsByClassName None "lstlisting"
  |> List.map (fun el => {
    els: [el],
    text: Element.innerText el,
    replace: (fun html => {
      let parent = Node.parentNode el;
      Element.setInnerHTML parent ("<pre>" ^ html ^ "</pre>");
      parent
    })
  });

let getCodeListings =>
  getElementsByTagName None "code"
  |> List.map (fun el => {
    els: [el],
    text: Element.innerText el,
    replace: (fun html => {
      Element.setInnerHTML el html;
      el
    })
  });

let getListings =>
  [
    getPreListings,
    getDefListings,
    getLstListings,
    getCodeListings
  ]
  |> List.map (fun f => f ())
  |> List.flatten;
