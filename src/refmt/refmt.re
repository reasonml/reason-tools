type asts =
  | REI (Migrate_parsetree.Ast_404.Parsetree.signature, Reason_pprint_ast.commentWithCategory)
  | RE (Migrate_parsetree.Ast_404.Parsetree.structure, Reason_pprint_ast.commentWithCategory)
  | ML (Migrate_parsetree.Ast_404.Parsetree.structure, Reason_pprint_ast.commentWithCategory)
  | MLI (Migrate_parsetree.Ast_404.Parsetree.signature, Reason_pprint_ast.commentWithCategory);

type parseResult =
  | Ast asts
  | Error exn;

type language =
  | ML
  | RE
  | UnknownLang;

let languageOfString str =>
  switch str {
  | "ML" => ML
  | "RE" => RE
  | _ => UnknownLang
  };

let stringOfLanguage lang =>
  switch lang {
  | ML => "ML"
  | RE => "RE"
  | UnknownLang => "Unkown"
  };

type codeType =
  | Implementation
  | Interface
  | UnknownType;

let typeOfString str =>
  switch str {
  | "implementation" => Implementation
  | "interface" => Interface
  | _ => UnknownType
  };

let string_of_code printer code => {
  ignore (Format.flush_str_formatter ());
  let f = Format.str_formatter;
  printer f code;
  Format.flush_str_formatter ()
};

let parseRE code =>
  Ast (RE (Reason_toolchain.JS.canonical_implementation_with_comments (Lexing.from_string code)));

let parseML code =>
  Ast (ML (Reason_toolchain.ML.canonical_implementation_with_comments (Lexing.from_string code)));

let parseREI code =>
  Ast (REI (Reason_toolchain.JS.canonical_interface_with_comments (Lexing.from_string code)));

let parseMLI code =>
  Ast (MLI (Reason_toolchain.ML.canonical_interface_with_comments (Lexing.from_string code)));

let printML implementation =>
  string_of_code Reason_toolchain.ML.print_canonical_implementation_with_comments implementation;

let printMLI signature =>
  string_of_code Reason_toolchain.ML.print_canonical_interface_with_comments signature;

let printRE = string_of_code Reason_toolchain.JS.print_canonical_implementation_with_comments;

let printREI = string_of_code Reason_toolchain.JS.print_canonical_interface_with_comments;

/* |>   */
let (>!) (loc1: Location.t) (loc2: Location.t) => {
  let (_, line1, col1) = Location.get_pos_info loc1.loc_start;
  let (_, line2, col2) = Location.get_pos_info loc2.loc_start;
  line1 === line2 ? col1 > col2 : line1 > line2
};

let genErrors errors => {
  let error =
    List.fold_left
      (
        fun ((prevLoc, _, _) as prev) error => {
          let (optionLoc, _, _) as curr =
            switch error {
            | Syntaxerr.Error error =>
              /* ML */
              let loc = Syntaxerr.location_of_error error;
              let error = string_of_code Syntaxerr.report_error error;
              (Some loc, error, ML)
            | Syntax_util.Error loc error =>
              /* RE */
              let error = string_of_code Syntax_util.report_error error;
              (Some loc, error, RE)
            | finalExn => (None, Printexc.to_string finalExn, UnknownLang)
            };
          switch (optionLoc, prevLoc) {
          | (Some loc, Some prevLoc) => loc >! prevLoc ? curr : prev
          | (Some _, None) => curr
          | _ => prev
          }
        }
      )
      (None, "", UnknownLang)
      errors;
  switch error {
  | (None, message, inLang) => (inLang, UnknownLang, message)
  /* Reason error message printer doesn't include location, ML does */
  | (Some loc, message, inLang) when inLang == RE => (
      inLang,
      UnknownLang,
      string_of_code Location.print_loc loc ^ ":\n" ^ message
    )
  | (_, message, inLang) => (inLang, UnknownLang, message)
  }
};

let attempts inLang inType =>
  switch (inLang, inType) {
  | (ML, Implementation) => [parseML]
  | (ML, Interface) => [parseMLI]
  | (RE, Implementation) => [parseRE]
  | (RE, Interface) => [parseREI]
  | (UnknownLang, Implementation) => [parseML, parseRE]
  | (UnknownLang, Interface) => [parseMLI, parseREI]
  | (ML, UnknownType) => [parseML, parseMLI]
  | (RE, UnknownType) => [parseRE, parseREI]
  | (UnknownLang, UnknownType) => [parseML, parseRE, parseMLI, parseREI]
  };

let refmt code inLang inType outLang => {
  let parsersToTry = attempts inLang inType;
  let results =
    List.map
      (
        fun parser =>
          try {
            let a = parser code;
            a
          } {
          | exn => Error exn
          }
      )
      parsersToTry;
  try {
    let result =
      List.find
        (
          fun res =>
            switch res {
            | Error _ => false
            | _ => true
            }
        )
        results;
    let (trueOut, printedResult) =
      switch (outLang, result) {
      | (RE, Ast (ML ast))
      | (RE, Ast (RE ast))
      | (UnknownLang, Ast (ML ast)) => (RE, printRE ast)
      | (ML, Ast (ML ast))
      | (ML, Ast (RE ast))
      | (UnknownLang, Ast (RE ast)) => (ML, printML ast)
      | (RE, Ast (MLI ast))
      | (RE, Ast (REI ast))
      | (UnknownLang, Ast (MLI ast)) => (RE, printREI ast)
      | (ML, Ast (MLI ast))
      | (ML, Ast (REI ast))
      | (UnknownLang, Ast (REI ast)) => (ML, printMLI ast)
      | _ => (UnknownLang, "could not transform")
      };
    let trueIn =
      switch result {
      | Ast (ML _)
      | Ast (MLI _) => ML
      | Ast (RE _)
      | Ast (REI _) => RE
      | _ => UnknownLang
      };
    (trueIn, trueOut, printedResult)
  } {
  | exn =>
    let fold =
      List.fold_left
        (
          fun acc res =>
            switch res {
            | Error err => [err, ...acc]
            | _ => acc
            }
        )
        [];
    results |> fold |> genErrors
  }
};

let refmtJS jsString inLang inType outLang => {
  let (trueIn, trueOut, s) =
    refmt
      (Js.to_string jsString)
      (languageOfString (Js.to_string inLang))
      (typeOfString (Js.to_string inType))
      (languageOfString (Js.to_string outLang));
  Js.array [|Js.string (stringOfLanguage trueIn ^ "to" ^ stringOfLanguage trueOut), Js.string s|]
};

Js.export "refmt" refmtJS;
