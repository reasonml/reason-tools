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

let refmt input cb =>
  ignore (
    Js.Global.setTimeout
      (
        fun () =>
          switch (Background.Refmt.refmt input) {
          | ("Failure", error) => cb error None None
          | (conversion, outText) =>
            switch (conversion |> Js.String.split "to") {
            | [|inLang, outLang|] => cb outText (Some inLang) (Some outLang)
            | _ => ()
            }
          }
      )
      0
  );

Document.addEventListener "DOMContentLoaded" (fun () => PopupCommon.init ::onOpen ::refmt ());
