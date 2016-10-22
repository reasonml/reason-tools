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

let log str => Js.Unsafe.meth_call Firebug.console "log" [|Js.Unsafe.inject str|];

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
        | exn4 => "COULD NOT PARSE---"
        }
      }
    }
  };

print_endline (reason "let a b = b + 1;");

print_endline (reason "let a b => b + 1;");

print_endline (reason "type a = bool;");
/* let node_constr: Js.constr (Js.t Dom.node);
let obj : obj Js.t = Js.Unsafe.js_expr "obj";
let cast_node (x: Js.t) :Js.opt (Js.t Dom.node) =>
  if (Js.instanceof x obj) {
    Js.some (Js.Unsafe.coerce x)
  } else {
    Js.null
  };

let maybePre = Dom_html.document#querySelectorAll (Js.string "pre");

let printTextContent node => log @@ Dom.CoerceTo.text node;

 let run () => {
   let maybePre =
     Js.Unsafe.meth_call Dom_html.document "querySelectorAll" [|Js.Unsafe.inject "pre"|];
   let maybeLog x => switch x {
        | Js.undefined x => log x
        | _ => log "nothing"
      };
   let pre = List.map cast_node maybePre;
   List.iter log maybePre;
   List.iter printTextContent maybePre;
   log maybePre;
   ()
 };

 Dom_html.window#onload <- Dom_html.handler (fun _ -> run (); Js._false); */

 /* if (MutationObserver.is_supported ()) {
   print_endline "wow!!!";
   let target = Dom_html.document;
   let node = (target :> Js.t Dom.node);
   let f records observer => {
     records;
     ()
   };
   MutationObserver.observe
     node::node f::f attributes::false child_list::true character_data::true ();
    ()
 }; */
