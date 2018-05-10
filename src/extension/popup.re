open Core;

open LocalDom;

let mapOrElse = (resolve, reject, item) =>
  switch (item) {
  | Some(res) => resolve(res)
  | None => reject()
  };

let getSelection = () =>
  Promise.make(
    (resolve, reject) =>
      Chrome.Tabs.executeScript(
        {"code": "window.getSelection().toString()"},
        (maybeMaybeArray) => {
          maybeMaybeArray
          |> Js.Null_undefined.to_opt
          |> Js.Option.andThen([@bs](maybeArray) => Js.Array.findi((_, index) => index === 0, maybeArray))
          |> Js.Option.andThen([@bs](s) => s == "" ? None : Some(s))
          |> mapOrElse(resolve, reject);
        }
      )
  );

let getLatestInput = () =>
  Promise.make(
    (resolve, reject) =>
      Protocol.Storage.queryLatestInput(
        (maybeInput) => maybeInput |> mapOrElse(resolve, reject)
      )
  );

let refmt =
    (
      input,
      ~inLang=RefmtShared.UnknownLang,
      ~inType=RefmtShared.UnknownType,
      ~outLang=RefmtShared.UnknownLang,
      cb
    ) => {
  Protocol.Refmt.send(
    input,
    ~inLang,
    ~inType,
    ~outLang,
    (error) =>
      switch error {
      | Result.Error(error) => cb(error, RefmtShared.UnknownLang, RefmtShared.UnknownLang)
      | Result.Ok({outText, inLang, outLang}) => cb(outText, inLang, outLang)
      }
  );
  Protocol.Storage.setLatestInput(input)
};

let onOpen = Protocol.OpenInTab.send;

Document.addEventListener(
  "DOMContentLoaded",
  PopupCommon.init(~getSelection, ~getLatestInput, ~onOpen, ~refmt)
);
