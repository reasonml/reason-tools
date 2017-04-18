open Core;

open LocalDom;

open Common;

let hasClassName className => List.length (getElementsByClassName None className) > 0;

let ocamlishRels = [|"Start", "previous", "next", "Up", "Appendix", "Section", "Subsection"|];

let hasOcamlRels () => {
  let hits =
    getElementsByTagName None "link" |>
    List.map (fun link => ocamlishRels |> Js.Array.includes (Element.getAttribute link "rel")) |>
    List.filter id |> List.length;
  hits >= 3
};

let hasCommonClassNames () => {
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
    ] |>
    List.map hasClassName |>
    List.filter id |> List.length;
  hits >= 3
};

let hasUniqueClassNames () =>
  ["odoc-doc", "package-index"] |> List.map hasClassName |> List.exists id;

let mightBeOcamlDoc () => hasUniqueClassNames () || hasOcamlRels () || hasCommonClassNames ();

let getBlacklist () => [
  "caml.inria.fr/pub/docs/u3-ocaml/ocaml-objects.html", /* #17 */
  "asciidoctor.org/docs/user-manual/", /* #40 */
  "ocaml.janestreet.com/ocaml-core/latest/doc/core/", /* #46 */
  "mwhittaker.github.io/distributed-systems-ocaml/code_MorePipes.html", /* #49 */
];

let getWhitelist () => [
];

let getSignificantUrl () =>
  Location.hostname ^ Location.pathname;

let isWhitelisted () =>
  getWhitelist () |> List.mem (getSignificantUrl ());

let isBlacklisted () =>
  getBlacklist () |> List.mem (getSignificantUrl ());

let shouldConvert () => {
  let cached: option bool = [%raw "window._rtShouldConvert"] |> Js.Undefined.to_opt;
  switch cached {
  | Some shouldConvert => shouldConvert
  | None =>
    let isWhitelisted = isWhitelisted ();
    let mightBeOcamlDoc = mightBeOcamlDoc ();
    let isBlacklisted = isBlacklisted ();
    let shouldConvert = isWhitelisted || (mightBeOcamlDoc && not isBlacklisted);
    /* 
    Js.log @@ "isWhitelisted:   " ^ (string_of_bool isWhitelisted);
    Js.log @@ "mightBeOcamlDoc: " ^ (string_of_bool mightBeOcamlDoc);
    Js.log @@ "isBlackListed:   " ^ (string_of_bool isBlacklisted);
    Js.log @@ "shouldConvert:   " ^ (string_of_bool shouldConvert); 
    */
    if shouldConvert {
      ignore [%raw "window._rtShouldConvert = 1"];
    } else {
      ignore [%raw "window._rtShouldConvert = 0"];
    };

    shouldConvert
  }
  
};

