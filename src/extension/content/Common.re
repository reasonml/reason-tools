open Dom;

let normalizeText text =>
  text |> Js.String.trim |> Js.String.replaceByRe [%bs.re {|/[^\x00-\x7F]/g|}] " " |>
  Js.String.replace (Js.String.fromCharCode 65533) "";

let getElementsByTagName maybeEl name =>
  (
    switch maybeEl {
    | Some el => Element.getElementsByTagName el name
    | None => Document.getElementsByTagName name
    }
  ) |> Arrayish.toArray |> Array.to_list;

let getElementsByClassName maybeEl name =>
  (
    switch maybeEl {
    | Some el => Element.getElementsByClassName el name
    | None => Document.getElementsByClassName name
    }
  ) |> Arrayish.toArray |> Array.to_list;

let querySelectorAll el selector =>
  Element.querySelectorAll el selector |> Arrayish.toArray |> Array.to_list;

let createStylesheet () => {
  let stylesheet = Document.createElement "link";
  let css = [%bs.raw {|require('../../../../../src/css/ocamlDoc.css')|}];
  Element.setType stylesheet "text/css";
  Element.setRel stylesheet "stylesheet";
  Element.setHref stylesheet (Chrome.Extension.getURL css);
  stylesheet
};
