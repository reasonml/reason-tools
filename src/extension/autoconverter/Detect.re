open Core;
open Core.Dom;
open Common;

let hasClassName className =>
  List.length (getElementsByClassName None className) > 0;

let ocamlishRels = [|
  "Start",
  "previous",
  "next",
  "Up",
  "Appendix",
  "Section",
  "Subsection"
|];

let hasOcamlRels => {
  let rels =
    getElementsByTagName None "link"
      |> List.map (fun link => ocamlishRels |> arrayContains (Element.getAttribute link "rel"))
      |> List.filter (fun b => b)
      |> List.length;

    rels >= 3
};

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
