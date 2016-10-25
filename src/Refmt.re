type conversion =
  | MLtoRE string
  | REtoML string
  | MLItoREI string
  | REItoMLI string
  | Failure string;

let string_of_signature x => {
  ignore (Format.flush_str_formatter ());
  let f = Format.str_formatter;
  Pprintast.default#signature f x;
  Format.flush_str_formatter ()
};

let string_of_formatter comments f x => {
  ignore (Format.flush_str_formatter ());
  f comments Format.str_formatter x;
  Format.flush_str_formatter ()
};

let reasonFormatter = Reason_pprint_ast.createFormatter ();

let parserForREToML code => Reason_toolchain.JS.canonical_implementation_with_comments (
  Lexing.from_string code
);

let parserForMLToRE code => Reason_toolchain.ML.canonical_implementation_with_comments (
  Lexing.from_string code
);

let parserForREIToMLI code => Reason_toolchain.JS.canonical_interface_with_comments (
  Lexing.from_string code
);

let parserForMLIToREI code => Reason_toolchain.ML.canonical_interface_with_comments (
  Lexing.from_string code
);

let printML implementation => Pprintast.string_of_structure implementation;

let printMLI signature => string_of_signature signature;

let printReasonAST formatter ast comments => string_of_formatter comments formatter ast;

let printRE = printReasonAST reasonFormatter#structure;

let printREI = printReasonAST reasonFormatter#signature;

let printErrors errors => Array.map Printexc.to_string errors;

let refmt code =>
  try {
    let (ast2, comments2) = parserForMLToRE code;
    MLtoRE (printRE ast2 comments2)
  } {
  | exn =>
    try {
      let (ast, comments) = parserForREToML code;
      REtoML (printML ast)
    } {
    | exn2 =>
      try {
        let (ast, comments) = parserForREIToMLI code;
        REItoMLI (printMLI ast)
      } {
      | exn3 =>
        try {
          let (ast, comments) = parserForMLIToREI code;
          MLItoREI (printREI ast comments)
        } {
        | exn4 => Failure (Printexc.to_string exn4)
        }
      }
    }
  };

let refmtJS jsString =>
  switch (refmt (Js.to_string jsString)) {
  | MLtoRE s => Js.array [|Js.string "MLtoRE", Js.string s|]
  | REtoML s => Js.array [|Js.string "REtoML", Js.string s|]
  | MLItoREI s => Js.array [|Js.string "MLItoREI", Js.string s|]
  | REItoMLI s => Js.array [|Js.string "REItoMLI", Js.string s|]
  | Failure s => Js.array [|Js.string "Failure", Js.string s|]
  };

Js.export "refmt" refmtJS;
