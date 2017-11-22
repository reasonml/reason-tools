open Rebase;

open Core;

open LocalDom;

let getSelection = () =>
  Promise.make(
    (resolve, reject) =>
      Chrome.Tabs.executeScript(
        {"code": "window.getSelection().toString()"},
        (maybeMaybeArray) =>
          maybeMaybeArray
          |> Js.Null_undefined.to_opt
          |> Option.andThen((maybeArray) => Js.Array.findi((_, index) => index === 0, maybeArray))
          |> Option.andThen((s) => Str.isEmpty(s) ? None : Some(s))
          |> Option.mapOrElse(resolve, reject)
      )
  );

let getLatestInput = () =>
  Promise.make(
    (resolve, reject) =>
      Protocol.Storage.queryLatestInput(
        (maybeInput) => maybeInput |> Option.mapOrElse(resolve, reject)
      )
  );

let refmt =
    (
      input,
      ~inLang=Refmt2.UnknownLang,
      ~inType=Refmt2.UnknownType,
      ~outLang=Refmt2.UnknownLang,
      cb
    ) => {
  Protocol.Refmt.send(
    input,
    ~inLang,
    ~inType,
    ~outLang,
    (error) =>
      switch error {
      | Error(error) => cb(error, Refmt2.UnknownLang, Refmt2.UnknownLang)
      | Ok({outText, inLang, outLang}) => cb(outText, inLang, outLang)
      }
  );
  Protocol.Storage.setLatestInput(input)
};

let onOpen = Protocol.OpenInTab.send;

Document.addEventListener(
  "DOMContentLoaded",
  PopupCommon.init(~getSelection, ~getLatestInput, ~onOpen, ~refmt)
);
