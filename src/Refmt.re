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

let reason code =>
  try {
    let (ast2, comments2) = parserForMLToRE code;
    printRE ast2 comments2
  } {
  | exn =>
    try {
      let (ast, comments) = parserForREToML code;
      printML ast
    } {
    | exn2 =>
      try {
        let (ast, comments) = parserForREIToMLI code;
        printMLI ast
      } {
      | exn3 =>
        try {
          let (ast, comments) = parserForMLIToREI code;
          printREI ast comments
        } {
        | exn4 => ""
        }
      }
    }
  };

let reasonJS jsString => Js.string (reason (Js.to_string jsString));

Js.export "refmt" reasonJS;
