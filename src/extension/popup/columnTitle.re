open Rebase;

let showVersion lang =>
  switch lang {
  | Protocol.RE => Globals.reasonVersion
  | Protocol.ML => Globals.ocamlVersion
  | _ => ""
  };

let createElement ::lang ::select children::_ () =>
  <span title=(showVersion lang) style=PopupStyles.contextTitle> select </span>;
