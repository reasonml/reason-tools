type conversion =
  | MLtoRE string
  | REtoML string
  | MLItoREI string
  | REItoMLI string
  | Failure string;

let string_of_code printer code => {
  ignore (Format.flush_str_formatter ());
  let f = Format.str_formatter;
  printer f code;
  Format.flush_str_formatter ()
};

let parserForREToML code =>
  Reason_toolchain.JS.canonical_implementation_with_comments (Lexing.from_string code);

let parserForMLToRE code =>
  Reason_toolchain.ML.canonical_implementation_with_comments (Lexing.from_string code);

let parserForREIToMLI code =>
  Reason_toolchain.JS.canonical_interface_with_comments (Lexing.from_string code);

let parserForMLIToREI code =>
  Reason_toolchain.ML.canonical_interface_with_comments (Lexing.from_string code);

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

type lang =
  | ML
  | RE
  | Other;

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
            | finalExn => (None, Printexc.to_string finalExn, Other)
            };
          switch (optionLoc, prevLoc) {
          | (Some loc, Some prevLoc) => loc >! prevLoc ? curr : prev
          | (Some _, None) => curr
          | _ => prev
          }
        }
      )
      (None, "", Other)
      errors;
  switch error {
  | (None, message, _) => message
  /* Reason error message printer doesn't include location, ML does */
  | (Some loc, message, RE) => string_of_code Location.print_loc loc ^ ":\n" ^ message
  | (_, message, _) => message
  }
};

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
        | exn4 => Failure (genErrors [exn, exn2, exn3, exn4])
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
