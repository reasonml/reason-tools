open Rebase;

let showVersion lang =>
  switch lang {
  | Some "RE"
  | Some "REI" => Globals.reasonVersion
  | Some "ML"
  | Some "MLI" => Globals.ocamlVersion
  | _ => ""
  };

let createElement ::lang ::select children::_ () =>
  <span title=(showVersion lang) style=PopupStyles.contextTitle> select </span>;
