let showVersion = (lang) =>
  switch lang {
  | RefmtShared.RE => Globals.reasonVersion
  | RefmtShared.ML => Globals.ocamlVersion
  | _ => ""
  };

let component = ReasonReact.statelessComponent("ColumnTItle");

let make = (~lang, ~select, _) => {
  ...component,
  render: (_) => <span title=(showVersion(lang)) style=PopupStyles.contextTitle> select </span>
};
