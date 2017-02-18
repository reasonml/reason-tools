open Core.Dom;

let normalizeText text =>
  text
  |> Js.String.trim
  |> Js.String.replaceByRe [%bs.re {|/[^\x00-\x7F]/g|}] " "
  |> Js.String.replace (Js.String.fromCharCode 65533) "";

let getElementsByTagName maybeEl name =>
  switch maybeEl {
    | None => Document.getElementsByTagName name
    | Some el => Element.getElementsByTagName el name
  } |> Arrayish.toArray |> Array.to_list;

let getElementsByClassName maybeEl name =>
  switch maybeEl {
    | None => Document.getElementsByClassName name
    | Some el => Element.getElementsByClassName el name
  } |> Arrayish.toArray |> Array.to_list;

let querySelectorAll el selector =>
  Element.querySelectorAll el selector |> Arrayish.toArray |> Array.to_list;

let arrayContains value arr =>
  Js.Array.includes value arr |> Js.to_bool;

external id : 'a => 'a = "%identity";
