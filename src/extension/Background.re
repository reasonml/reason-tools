open Core;

module Refmt = {
  type t = (string, string);

  external refmt : string => t = "refmt" [@@bs.module ("../../../../_build/refmt/src/app.js", "Refmt")];

  let parse: t => RefmtProtocol.response = fun
  | ("Failure", error) => Failure error
  | (conversion, outText) =>
    switch (conversion |> Js.String.split "to") {
    | [| inLang, outLang |] => Success { outText, inLang, outLang }
    | _ => Failure "Encountered some weird unknown conversion";
  };
};

RefmtProtocol.listen
	(fun request respond =>
    request.input |> Refmt.refmt
                  |> Refmt.parse
                  |> respond );
