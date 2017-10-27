open LocalDom;

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

let refmt
    input
    ::inLang=Refmt2.UnknownLang
    ::inType=Refmt2.UnknownType
    ::outLang=Refmt2.UnknownLang
    cb =>
  ignore (
    Js.Global.setTimeout
      (
        fun () =>
          switch (
            Background.Refmt.refmt
              input
              (inLang)
              (inType)
              (outLang)
          ) {
          | ("Failure", error) => cb error Refmt2.UnknownLang Refmt2.UnknownLang
          | (conversion, outText) =>
            switch (conversion |> Js.String.split "to") {
            | [|inLang, outLang|] =>
              cb outText (Protocol.languageOfString inLang) (Protocol.languageOfString outLang)
            | _ => ()
            }
          }
      )
      0
  );

Document.addEventListener "DOMContentLoaded" (fun () => PopupCommon.init ::onOpen ::refmt ());
