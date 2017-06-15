open Rebase;

let showVersion lang =>
  switch lang {
  | Protocol.RE => Globals.reasonVersion
  | Protocol.ML => Globals.ocamlVersion
  | _ => ""
  };

let make ::lang ::select _ => {
  ...(ReasonReact.statelessComponent "ColumnTItle"),
  
  render: fun _ _ =>
    <span title=(showVersion lang) style=PopupStyles.contextTitle> select </span>
};