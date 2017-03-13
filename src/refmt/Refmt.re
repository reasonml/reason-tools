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

let string_of_ml_signature signature => {
  ignore (Format.flush_str_formatter ());
  let f = Format.str_formatter;
  Reason_toolchain.ML.print_canonical_interface_with_comments f signature;
  Format.flush_str_formatter ()
};

let string_of_ml_structure structure => {
  ignore (Format.flush_str_formatter ());
  let f = Format.str_formatter;
  Reason_toolchain.ML.print_canonical_implementation_with_comments f structure;
  Format.flush_str_formatter ()
};

let string_of_re_signature signature => {
  ignore (Format.flush_str_formatter ());
  let f = Format.str_formatter;
  Reason_toolchain.JS.print_canonical_interface_with_comments f signature;
  Format.flush_str_formatter ()
};

let string_of_re_structure structure => {
  ignore (Format.flush_str_formatter ());
  let f = Format.str_formatter;
  Reason_toolchain.JS.print_canonical_implementation_with_comments f structure;
  Format.flush_str_formatter ()
};

let string_of_formatter comments f x => {
  ignore (Format.flush_str_formatter ());
  f comments Format.str_formatter x;
  Format.flush_str_formatter ()
};

let reasonFormatter = Reason_pprint_ast.createFormatter ();

let parserForREToML code =>
  Reason_toolchain.JS.canonical_implementation_with_comments (Lexing.from_string code);

let parserForMLToRE code =>
  Reason_toolchain.ML.canonical_implementation_with_comments (Lexing.from_string code);

let parserForREIToMLI code =>
  Reason_toolchain.JS.canonical_interface_with_comments (Lexing.from_string code);

let parserForMLIToREI code =>
  Reason_toolchain.ML.canonical_interface_with_comments (Lexing.from_string code);

let printML implementation => string_of_ml_structure implementation;

let printMLI signature => string_of_ml_signature signature;

let printReasonAST formatter ast comments => string_of_formatter comments formatter ast;

let printRE = string_of_re_structure;

let printREI = string_of_re_signature;

/* Migrate_parsetree.Ast_404.Parsetree.structure; */
/* Error: This expression has type Migrate_parsetree.Ast_404.Parsetree.structure
   but an expression was expected of type Ast_404.Parsetree.structure: /Users/rickyvetter/code/fb/reason-tools/src/refmt/Refmt.re:53 */
let refmt code =>
  try {
    let ast = parserForMLToRE code;
    MLtoRE (printRE ast)
  } {
  | exn =>
    try {
      let ast = parserForREToML code;
      REtoML (printML ast)
    } {
    | exn2 =>
      try {
        let ast = parserForREIToMLI code;
        REItoMLI (printMLI ast)
      } {
      | exn3 =>
        try {
          let ast = parserForMLIToREI code;
          MLItoREI (printREI ast)
        } {
        | Syntaxerr.Error exn4 =>
          switch exn4 {
          /* this is basically the only error I've ever seen.
             If we need to pretty print others, we can! */
          | Syntaxerr.Other {loc_start: {pos_lnum: sl}} =>
            Failure ("Syntax error on line " ^ string_of_int sl)
          | _ => Failure "Syntax Error"
          }
        | finalExn => Failure (Printexc.to_string finalExn)
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
