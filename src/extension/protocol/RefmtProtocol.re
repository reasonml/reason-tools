type request = {
  input: string
};

type response = {
  output: (string, string)
};

let parseConversionType conversionType =>
  switch (conversionType |> Js.String.split "to") {
  | [| inLang, outLang |] => (Some inLang, Some outLang)
  | _ => (None, None)
};

module Refmt = {
  external refmt : string => (string, string) = "refmt" [@@bs.module "../../../../_build/refmt/src/app.js"];
};
