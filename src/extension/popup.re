open Rebase;

open Core;

open LocalDom;

let getSelection () =>
  Promise.make (
    fun resolve reject =>
      Chrome.Tabs.executeScript
        {"code": "window.getSelection().toString()"}
        (
          fun maybeMaybeArray =>
            maybeMaybeArray |> Js.Null_undefined.to_opt |>
            Option.andThen (
              fun maybeArray => Js.Array.findi (fun _ index => index === 0) maybeArray
            ) |>
            Option.andThen (fun s => Str.isEmpty s ? None : Some s) |>
            Option.mapOrElse resolve reject
        )
  );

let getLatestInput () =>
  Promise.make (
    fun resolve reject =>
      Protocol.Storage.queryLatestInput (
        fun maybeInput => maybeInput |> Option.mapOrElse resolve reject
      )
  );

let refmt input ::inLang="unknown" ::inType="unknown" ::outLang="unknown" cb => {
  Js.log outLang;
  Protocol.Refmt.send
    input
    ::inLang
    ::inType
    ::outLang
    (
      fun error => {
        switch error {
        | Error error => cb error None None
        | Ok {outText, inLang, outLang} => cb outText (Some inLang) (Some outLang)
        };
        Js.log error
      }
    );
  Protocol.Storage.setLatestInput input
};

let onOpen = Protocol.OpenInTab.send;

Document.addEventListener
  "DOMContentLoaded" (PopupCommon.init ::getSelection ::getLatestInput ::onOpen ::refmt);
