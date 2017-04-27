open Rebase;

let showVersion lang =>
  switch lang {
  | Some "RE"
  | Some "REI" => Globals.reasonVersion
  | Some "ML"
  | Some "MLI" => Globals.ocamlVersion
  | _ => ""
  };

let formatTitle name lang => name ^ " (" ^ lang ^ ")";

let createElement ::name ::lang children::_ () => {
  let title = lang |> Option.mapOr (formatTitle name) name;
  <span title=(showVersion lang) style=PopupStyles.contextTitle>
    (ReactRe.stringToElement title)
  </span>
};
