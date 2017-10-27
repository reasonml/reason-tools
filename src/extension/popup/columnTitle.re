let showVersion lang =>
  switch lang {
  | Refmt2.RE => Globals.reasonVersion
  | Refmt2.ML => Globals.ocamlVersion
  | _ => ""
  };

let component = ReasonReact.statelessComponent "ColumnTItle";
let make ::lang ::select _ => {
  ...component,

  render: fun _ =>
    <span title=(showVersion lang) style=PopupStyles.contextTitle> select </span>
};
