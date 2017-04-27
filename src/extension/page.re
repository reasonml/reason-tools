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
    ::inLang=Protocol.UnknownLang
    ::inType=Protocol.UnknownType
    ::outLang=Protocol.UnknownLang
    cb =>
  ignore (
    Js.Global.setTimeout
      (
        fun () =>
          switch (
            Background.Refmt.refmt
              input
              (Protocol.stringOfLanguage inLang)
              (Protocol.stringOfType inType)
              (Protocol.stringOfLanguage outLang)
          ) {
          | ("Failure", error) => cb error Protocol.UnknownLang Protocol.UnknownLang
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
