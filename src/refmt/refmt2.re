open Rebase;

type ast;

external parseREEx : string => ast = "parseRE" [@@bs.module "reason"];

external parseREIEx : string => ast = "parseREI" [@@bs.module "reason"];

external parseMLEx : string => ast = "parseML" [@@bs.module "reason"];

external parseMLIEx : string => ast = "parseMLI" [@@bs.module "reason"];

external printRE : ast => string = "" [@@bs.module "reason"];

external printREI : ast => string = "" [@@bs.module "reason"];

external printML : ast => string = "" [@@bs.module "reason"];

external printMLI : ast => string = "" [@@bs.module "reason"];

external parseREOldEx : string => ast =
  "parseRE" [@@bs.module "../../../../src/refmt/refmtOld.js"];

external parseREIOldEx : string => ast =
  "parseREI" [@@bs.module "../../../../src/refmt/refmtOld.js"];

external printREOld : ast => string = "printRE" [@@bs.module "../../../../src/refmt/refmtOld.js"];

external printREIOld : ast => string =
  "printREI" [@@bs.module "../../../../src/refmt/refmtOld.js"];

type result 'thing =
  | REI 'thing
  | RE 'thing
  | ML 'thing
  | MLI 'thing
  | REO 'thing
  | REOI 'thing;

type parseResult =
  | Ast (result ast)
  | Error (result Js.Exn.t);

type language =
  | ML
  | RE
  | REO
  | UnknownLang;

let languageOfString str =>
  switch str {
  | "ML" => ML
  | "RE" => RE
  | "REO" => REO
  | _ => UnknownLang
  };

let stringOfLanguage lang =>
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

let typeOfString str =>
  switch str {
  | "implementation" => Implementation
  | "interface" => Interface
  | _ => UnknownType
  };

let stringOfType codeType =>
  switch codeType {
  | Implementation => "implementation"
  | Interface => "interface"
  | UnknownType => "Unkown"
  };

let parseRE str =>
  try (Ast (RE (parseREEx str))) {
  | Js.Exn.Error parseError => Error (RE parseError)
  };

let parseREI str =>
  try (Ast (REI (parseREIEx str))) {
  | Js.Exn.Error parseError => Error (REI parseError)
  };

let parseML str =>
  try (Ast (ML (parseMLEx str))) {
  | Js.Exn.Error parseError => Error (ML parseError)
  };

let parseMLI str =>
  try (Ast (MLI (parseMLIEx str))) {
  | Js.Exn.Error parseError => Error (MLI parseError)
  }; /* let printRE (Ast (RE ast)) =>  ( (printREEx ast));
  let printREI  (Ast (REI ast)) => ( (printREIEx ast ));
  let printML (Ast (ML ast)) => ( (printMLEx ast));
  let printMLI  (Ast (MLI ast)) => ( (printMLIEx ast )); */

let parseREOld str =>
  try (Ast (REO (parseREOldEx str))) {
  | Js.Exn.Error parseError => Error (REO parseError)
  };

let parseREIOld str =>
  try (Ast (REOI (parseREIOldEx str))) {
  | Js.Exn.Error parseError => Error (REOI parseError)
  };

let (|?) a b =>
  switch a {
  | Some a => a
  | None => b
  } /* let (>!) (loc1: Location.t) (loc2: Location.t) => {
  let (_, line1, col1) = Location.get_pos_info loc1.loc_start;
  let (_, line2, col2) = Location.get_pos_info loc2.loc_start;
  line1 === line2 ? col1 > col2 : line1 > line2
}; */ /* |>   */ /* let parseRE code =>
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

let printREI = string_of_code Reason_toolchain.JS.print_canonical_interface_with_comments; */ /* let string_of_code printer code => {
  ignore (Format.flush_str_formatter ());
  let f = Format.str_formatter;
  printer f code;
  Format.flush_str_formatter ()
}; */; /* let printREOld  (Ast (REO ast)) => ( (printREOldEx ast ));
  let printREIOld (Ast (REOI ast)) => ( (printREIOldEx ast)); */

let genErrors errors =>
  switch errors {
  | [] => (UnknownLang, UnknownLang, "Sorry we don't have much info. Something went very wrong.")
  | [error, ..._] =>
    let thing =
      switch error {
      | REI thing => Js.Exn.message thing
      | RE thing => Js.Exn.message thing
      | ML thing => Js.Exn.message thing
      | MLI thing => Js.Exn.message thing
      | REO thing => Js.Exn.message thing
      | REOI thing => Js.Exn.message thing
      };
    (
      UnknownLang,
      UnknownLang,
      switch thing {
      | Some str => str
      | None => "Sorry we don't have much info. Something went very wrong."
      }
    ) /* switch (Js.Json.stringifyAny error) {
      | Some error => error
      | None => "Sorry we don't have much info. Something went very wrong."
    }) */ /* let error =
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
  } */
  };

let attempts inLang inType =>
  switch (inLang, inType) {
  | (ML, Implementation) => [parseML]
  | (ML, Interface) => [parseMLI]
  | (RE, Implementation) => [parseRE]
  | (RE, Interface) => [parseREI]
  | (REO, Implementation) => [parseREI]
  | (REO, Interface) => [parseREI]
  | (UnknownLang, Implementation) => [parseRE, parseREOld, parseML]
  | (UnknownLang, Interface) => [parseREI, parseREIOld, parseMLI]
  | (ML, UnknownType) => [parseML, parseMLI]
  | (RE, UnknownType) => [parseRE, parseREI]
  | (REO, UnknownType) => [parseREOld, parseREIOld]
  | (UnknownLang, UnknownType) => [parseRE, parseREI, parseREOld, parseREIOld, parseML, parseMLI]
  };

let refmt code inLang inType outLang => {
  let parsersToTry = attempts inLang inType;
  let results = List.map (fun parser => parser code) parsersToTry;
  try {
    let resultList =
      List.find_all
        (
          fun res =>
            switch res {
            | Error _ => false
            | _ => true
            }
        )
        results;
    let result =
      switch (resultList, results) {
      | ([], [error])
      | ([], [error, ..._]) => error
      | ([result], _)
      | ([result, ..._], _) => result
      | (_, _) => assert false
      };
    let (trueOut, printedResult) =
      switch (outLang, result) {
      | (RE, Ast (ML ast))
      | (RE, Ast (RE ast))
      | (RE, Ast (REO ast))
      | (UnknownLang, Ast (REO ast))
      | (UnknownLang, Ast (ML ast)) => (RE, printRE ast)
      | (REO, Ast (ML ast))
      | (REO, Ast (RE ast))
      | (REO, Ast (REO ast)) => (REO, printREOld ast)
      | (ML, Ast (ML ast))
      | (ML, Ast (RE ast))
      | (ML, Ast (REO ast))
      | (UnknownLang, Ast (RE ast)) => (ML, printML ast)
      | (RE, Ast (MLI ast))
      | (RE, Ast (REI ast))
      | (RE, Ast (REOI ast))
      | (UnknownLang, Ast (REOI ast))
      | (UnknownLang, Ast (MLI ast)) => (RE, printREI ast)
      | (REO, Ast (MLI ast))
      | (REO, Ast (REI ast))
      | (REO, Ast (REOI ast)) => (REO, printREIOld ast)
      | (ML, Ast (MLI ast))
      | (ML, Ast (REI ast))
      | (ML, Ast (REOI ast))
      | (UnknownLang, Ast (REI ast)) => (ML, printMLI ast)
      | (_, Error error) =>
        switch error {
        | REI thing => (RE, Js.Exn.message thing |? "")
        | RE thing => (RE, Js.Exn.message thing |? "")
        | ML thing => (ML, Js.Exn.message thing |? "")
        | MLI thing => (ML, Js.Exn.message thing |? "")
        | REO thing => (REO, Js.Exn.message thing |? "")
        | REOI thing => (REO, Js.Exn.message thing |? "")
        }
      };
    let trueIn =
      switch result {
      | Ast (ML _)
      | Ast (MLI _) => ML
      | Ast (RE _)
      | Ast (REI _) => RE
      | Ast (REO _)
      | Ast (REOI _) => REO
      | Error _ => UnknownLang
      };
    (trueIn, trueOut, printedResult)
  } {
  | a => (UnknownLang, UnknownLang, Obj.magic a)
  }
};

let refmtJS jsString inLang inType outLang => {
  let (trueIn, trueOut, s) =
    refmt jsString (inLang) (inType) (outLang);
  (stringOfLanguage trueIn ^ "to" ^ stringOfLanguage trueOut, s)
};
