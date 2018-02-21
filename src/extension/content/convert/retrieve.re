open LocalDom;

open Common;

type typeTable = {
  el: Element.t,
  text: string,
  remove: unit => unit
};

type listing = {
  els: list(Element.t),
  text: string,
  replace: string => Element.t
};

let getTypeTable = (pre) =>
  switch (Js.Null.to_opt(Element.nextElementSibling(pre))) {
  | None => None
  | Some(el) =>
    if (Js.to_bool(DOMTokenList.contains(Element.classList(el), "typetable"))) {
      let text = Element.innerText(el);
      switch (Js.Null.to_opt(Element.nextSibling(el))) {
      | None => Some({el, text, remove: () => Element.remove(el)})
      | Some(next) =>
        if (Node.nodeType(next) == Node._TEXT_NODE) {
          Some({
            el,
            text: text ++ Node.nodeValue(next),
            remove: () => {
              Element.remove(el);
              Node.removeChild(Node.parentNode(next), next)
            }
          })
        } else {
          Some({el, text, remove: () => Element.remove(el)})
        }
      }
    } else {
      None
    }
  };

let getModuleSignature = (pre) =>
  switch (Js.String.endsWith("sig .. end", Element.innerText(pre))) {
  | false => None
  | true =>
    let anchor = LocalDom.Arrayish.toArray(
      Element.getElementsByTagName(pre, "a")
    )[0];
    let anchor = Element.outerHTML(anchor);
    let re = [%re "/sig .. end/g"];
    let replaced = Js.String.unsafeReplaceBy0(
      re,
      (_, _, _) => "{ " ++ anchor ++ " }",
      Element.innerText(pre)
    );
    Some({
      els: [pre],
      text: "",
      replace: (_html) => {
        Element.setInnerHTML(pre, replaced);
        pre
      }
    })
  };

let getPreListings = () =>
  getElementsByTagName(None, "pre")
  |> List.map(
       (el) =>
         switch (getTypeTable(el), getModuleSignature(el)) {
         | (Some(typeTable), _) => {
             els: [el, typeTable.el],
             text: Element.innerText(el) ++ typeTable.text,
             replace: (html) => {
               Element.setInnerHTML(el, html);
               typeTable.remove();
               el
             }
           }
        | (_, Some(preListing)) => preListing
         | (None, None) => {
             els: [el],
             text: Element.innerText(el),
             replace: (html) => {
               Element.setInnerHTML(el, html);
               el
             }
           }
         }
     );

let getDefListings = () =>
  getElementsByClassName(None, "def")
  |> List.map(
       (el) => {
         els: [el],
         text: Element.innerText(el),
         replace: (html) => {
           Element.setInnerHTML(el, "<pre>" ++ html ++ "</pre>");
           el
         }
       }
     );

let getLstListings = () =>
  getElementsByClassName(None, "lstlisting")
  |> List.map(
       (el) => {
         els: [el],
         text: Element.innerText(el),
         replace: (html) => {
           let parent = Node.parentNode(el);
           Element.setInnerHTML(parent, "<pre>" ++ html ++ "</pre>");
           parent
         }
       }
     );

let getCodeListings = () =>
  getElementsByTagName(None, "code")
  |> List.map(
       (el) => {
         els: [el],
         text: Element.innerText(el),
         replace: (html) => {
           Element.setInnerHTML(el, html);
           el
         }
       }
     );

let getListings = () =>
  [getPreListings, getDefListings, getLstListings, getCodeListings]
  |> List.map((f) => f())
  |> List.flatten;
