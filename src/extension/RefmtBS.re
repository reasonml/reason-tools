open Rebase;

type t = (string, string);

external refmt : string => t =
  "refmt" [@@bs.module ("../../../../_build/refmt/src/app.js", "Refmt")];

let parse =
  fun
  | ("Failure", error) => Error error
  | (conversion, outText) =>
    switch (conversion |> Js.String.split "to") {
    | [|inLang, outLang|] => Ok Protocol.Refmt.{outText, inLang, outLang}
    | _ => Error "Encountered some weird unknown conversion"
    };
