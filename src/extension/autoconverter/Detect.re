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
  let hits =
    getElementsByTagName None "link"
      |> List.map (fun link => ocamlishRels |> arrayContains (Element.getAttribute link "rel"))
      |> List.filter id
      |> List.length;

    hits >= 3
};

let hasCommonClassNames => {
  let hits =
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
    |> List.filter id
    |> List.length;

  hits >= 3
};

let hasUniqueClassNames =>
  [
    "odoc-doc",
    "package-index"
  ]
  |> List.map hasClassName
  |> List.exists id;

let mightBeOcamlDoc =>
  hasUniqueClassNames () || hasOcamlRels () || hasCommonClassNames ();
