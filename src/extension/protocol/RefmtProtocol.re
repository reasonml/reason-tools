type request = {
  in_: string
};

type response = {
  out: (string, string)
};

let parseConversionType conversionType =>
  switch (conversionType |> Js.String.split "to") {
  | [| inLang, outLang |] => (Some inLang, Some outLang)
  | _ => (None, None)
};

module Refmt = {
  external refmt : string => (string, string) = "refmt" [@@bs.module "../../../../_build/refmt/src/app.js"];
};
