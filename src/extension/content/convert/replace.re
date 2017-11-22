open LocalDom;

open Common;

let escapeRe = Js.String.replaceByRe([%bs.re {|/[\-\[\]\/\{\}\(\)\*\+\?\.\\\^\$\|]/g|}], "\\$&");

let replace = (this) =>
  Js.String.replaceByRe(Js.Re.fromString("/\\b" ++ escapeRe(this) ++ "\\b/g"));

let replaceAll = List.fold_left((out, (this, that)) => out |> replace(this, that));

let getNormalizedLinks = (els) =>
  els
  |> List.map((el) => getElementsByTagName(Some(el), "a"))
  |> List.flatten
  |> List.map((a) => (Element.innerText(a), Element.href(a)));

let getNormalizedIds = (els) =>
  els
  |> List.map((el) => querySelectorAll(el, "[id]"))
  |> List.flatten
  |> List.map(
       (span) => {
         let text = normalizeText(Element.innerText(span));
         let words = text |> Js.String.split(" ");
         let lastWord = words[Array.length(words) - 1];
         (lastWord, Element.id(span))
       }
     );

let replaceHrefs = (hrefs, out) =>
  hrefs
  |> List.map(((text, href)) => (text, "<a href='" ++ href ++ "'>" ++ text ++ "</span>"))
  |> replaceAll(out);

let replaceIds = (ids, out) =>
  ids
  |> List.map(
       ((text, id)) => (
         text,
         "<span class='reason_tools_anchor' id='" ++ id ++ "'>" ++ text ++ "</span>"
       )
     )
  |> replaceAll(out);

let replaceListing = (els, text, replace) => {
  let hrefs = getNormalizedLinks(els);
  let ids = getNormalizedIds(els);
  let el = text |> replaceHrefs(hrefs) |> replaceIds(ids) |> replace;
  Hljs.highlightBlock(el)
};
