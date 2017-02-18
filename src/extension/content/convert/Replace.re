open Dom;
open Common;

let escapeRe =
  Js.String.replaceByRe
    [%bs.re {|/[\-\[\]\/\{\}\(\)\*\+\?\.\\\^\$\|]/g|}]
    {|\\$&|};

let replace this =>
  Js.String.replaceByRe
    (Js.Re.fromString ("/\\b" ^ (escapeRe this) ^ "\\b/g"));

let replaceAll =
  List.fold_left (fun out (this, that) => out |> replace this that);

let getNormalizedLinks els =>
  els |> List.map (fun el => getElementsByTagName (Some el) "a")
      |> List.flatten
      |> List.map (fun a => (Element.innerText a, Element.href a));

let getNormalizedIds els =>
  els |> List.map (fun el => querySelectorAll el "[id]")
      |> List.flatten
      |> List.map (fun span => {
          let text = normalizeText (Element.innerText span);
          let words = text |> Js.String.split " ";
          let lastWord = Array.get words ((Array.length words) - 1);
          (lastWord, Element.id span)
  });

let replaceHrefs hrefs out =>
  hrefs |> List.map (fun (text, href) => (text, "<a href='" ^ href ^"'>" ^ text ^ "</span>"))
        |> replaceAll out;

let replaceIds ids out =>
  ids |> List.map (fun (text, id) => (text, "<span class='reason_tools_anchor' id='" ^ id ^"'>" ^ text ^ "</span>"))
      |> replaceAll out;

let replaceListing els text replace => {
    let hrefs = getNormalizedLinks els;
    let ids = getNormalizedIds els;

    let el =
      text |> replaceHrefs hrefs
           |> replaceIds ids
           |> replace;

    Hljs.highlightBlock el;
}
