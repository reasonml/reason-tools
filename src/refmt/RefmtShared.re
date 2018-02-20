type ast;

type result('thing) =
  | REI('thing)
  | RE('thing)
  | ML('thing)
  | MLI('thing)
  | REO('thing)
  | REOI('thing);

type parseResult =
  | Ast(result(ast))
  | Error(result(Js.Exn.t));

type language =
  | ML
  | RE
  | REO
  | UnknownLang;

let languageOfString = (str) =>
  switch str {
  | "ML" => ML
  | "RE" => RE
  | "REO" => REO
  | _ => UnknownLang
  };

let stringOfLanguage = (lang) =>
  switch lang {
  | ML => "ML"
  | RE => "RE"
  | REO => "REO"
  | UnknownLang => "Unkown"
  };

type codeType =
  | Implementation
  | Interface
  | UnknownType;

let typeOfString = (str) =>
  switch str {
  | "implementation" => Implementation
  | "interface" => Interface
  | _ => UnknownType
  };

let stringOfType = (codeType) =>
  switch codeType {
  | Implementation => "implementation"
  | Interface => "interface"
  | UnknownType => "Unkown"
  };