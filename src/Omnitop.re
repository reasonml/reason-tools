open Lwt;

let reason_header = "    ___  _______   ________  _  __\n   / _ \\\\/ __/ _ | / __/ __ \\\\/ |/ /\n  / , _/ _// __ |_\\ \\\\/ /_/ /    /\n /_/|_/___/_/ |_/___/\\\\____/_/|_/ ";

let sharp_chan = open_out "/dev/null0";

let caml_chan = open_out "/dev/null1";

/* let log a => Firebug.console##log (Js.string a);

   Sys_js.set_channel_flusher stderr log;

   Sys_js.set_channel_flusher caml_chan log;

   Sys_js.set_channel_flusher sharp_chan log;

   Sys_js.set_channel_flusher stdout log;

   Sys_js.set_channel_flusher stderr log; */
let exec' s => {
  let res: bool = JsooTop.use Format.std_formatter s;
  if (not res) {
    Format.eprintf "error while evaluating %s@." s
  }
};

/* load file using a synchronous XMLHttpRequest */
let load_resource_aux url => None;

let load_resource scheme (_, suffix) => {
  let url = (Js.string scheme)##concat suffix;
  load_resource_aux url
};

let setup_pseudo_fs () => {
  Sys_js.register_autoload' "/dev/" (fun s => Some (Js.string ""));
  Sys_js.register_autoload' "/http/" (fun s => load_resource "http://" s);
  Sys_js.register_autoload' "/https/" (fun s => load_resource "https://" s);
  Sys_js.register_autoload' "/ftp/" (fun s => load_resource "ftp://" s);
  Sys_js.register_autoload'
    "/" (fun (_, s) => load_resource_aux ((Js.string "filesys/")##concat s))
};

let setup_toplevel () => {
  JsooTop.initialize ();
  Toploop.parse_toplevel_phrase :=
    Reason_util.correctly_catch_parse_errors Reason_toolchain.JS.canonical_toplevel_phrase;
  Toploop.print_out_value := Reason_oprint.print_out_value;
  Toploop.print_out_type := Reason_oprint.print_out_type;
  Toploop.print_out_class_type := Reason_oprint.print_out_class_type;
  Toploop.print_out_module_type := Reason_oprint.print_out_module_type;
  Toploop.print_out_type_extension := Reason_oprint.print_out_type_extension;
  Toploop.print_out_sig_item := Reason_oprint.print_out_sig_item;
  Toploop.print_out_signature := Reason_oprint.print_out_signature;
  Toploop.print_out_phrase := Reason_oprint.print_out_phrase;
  Sys.interactive := false;
  exec' "module Lwt_main = struct\n             let run t = match Lwt.state t with\n               | Lwt.Return x -> x\n               | Lwt.Fail e -> raise e\n               | Lwt.Sleep -> failwith \"Lwt_main.run: thread didn't return\"\n            end";
  let header1 =
    Printf.sprintf "%s\n\nReason Meta Language Utility, running OCaml version %%s" reason_header;
  let header2 =
    Printf.sprintf "Compiled with Js_of_ocaml version %s\n\n" Sys_js.js_of_ocaml_version;
  exec' (Printf.sprintf "Format.printf \"%s@.\" Sys.ocaml_version;;" header1);
  exec' (Printf.sprintf "Format.printf \"%s@.\";;" header2);
  exec' "#enable \"pretty\";;";
  exec' "#enable \"shortvar\";;";
  Sys.interactive := true;
  ()
};

let setup_printers () => {
  exec' "let _print_error fmt e = Format.pp_print_string fmt (Js.string_of_error e)";
  Topdirs.dir_install_printer Format.std_formatter Longident.(Lident "_print_error");
  exec' "let _print_unit fmt (_ : 'a) : 'a = Format.pp_print_string fmt \"()\"";
  Topdirs.dir_install_printer Format.std_formatter Longident.(Lident "_print_unit")
};

let execute str cb () => {
  let content = {
    let len = String.length str;
    if (
      try (str != "" && str.[len - 1] != ';' && str.[len - 2] != ';') {
      | _ => true
      }
    ) {
      str ^ ";"
    } else {
      str
    }
  };
  try {
    ignore (Format.flush_str_formatter ());
    JsooTop.execute true Format.str_formatter content;
    let result = Format.flush_str_formatter ();
    cb result;
    Lwt.return_unit
  } {
  | exn =>
    cb @@ "hi ricky" ^ Printexc.to_string exn;
    Lwt.return_unit
  }
};

setup_pseudo_fs ();

setup_toplevel ();

setup_printers ();
