let showVersion lang =>
  switch lang {
  | Protocol.RE => Globals.reasonVersion
  | Protocol.ML => Globals.ocamlVersion
  | _ => ""
  };

let component = ReasonReact.statelessComponent "ColumnTItle";
let make ::lang ::select _ => {
  ...component,
  
  render: fun _ _ =>
    <span title=(showVersion lang) style=PopupStyles.contextTitle> select </span>
};