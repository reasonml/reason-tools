open LocalDom;

open Rebase;

let onOpen: string => unit = [%bs.raw
  {|
  function (hash) {
    window.open(
      window.location.href,
      "_blank"
    );
  }
|}
];

module Refmt = {
  let refmt = Refmt2.refmtJS;
  let parse =
    fun
    | ("Failure", error) => Error(error)
    | (conversion, outText) =>
      switch (conversion |> Js.String.split("to")) {
      | [|inLang, outLang|] when Protocol.languageOfString(outLang) != Refmt2.UnknownLang =>
        Ok(
          Protocol.Refmt.{
            outText,
            inLang: Protocol.languageOfString(inLang),
            outLang: Protocol.languageOfString(outLang)
          }
        )
      | _ => Error(outText)
      };
};

let refmt =
    (
      input,
      ~inLang=Refmt2.UnknownLang,
      ~inType=Refmt2.UnknownType,
      ~outLang=Refmt2.UnknownLang,
      cb
    ) =>
  ignore(
    Js.Global.setTimeout(
      () =>
        switch (Refmt.refmt(input, inLang, inType, outLang)) {
        | ("Failure", error) => cb(error, Refmt2.UnknownLang, Refmt2.UnknownLang)
        | (conversion, outText) =>
          switch (conversion |> Js.String.split("to")) {
          | [|inLang, outLang|] =>
            cb(outText, Protocol.languageOfString(inLang), Protocol.languageOfString(outLang))
          | _ => ()
          }
        },
      0
    )
  );

Document.addEventListener("DOMContentLoaded", () => PopupCommon.init(~onOpen, ~refmt, ()));
