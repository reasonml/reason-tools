open Rebase;

open Core;

module Refmt = {
  type t = (string, string);
  external refmt : string => string => string => string => t =
    "refmt" [@@bs.module ("../../../../_build/refmt/app.js", "Refmt")];
  let parse =
    fun
    | ("Failure", error) => Error error
    | (conversion, outText) =>
      switch (conversion |> Js.String.split "to") {
      | [|inLang, outLang|] when Protocol.languageOfString outLang != Protocol.UnknownLang =>
        Ok Protocol.Refmt.{
             outText,
             inLang: Protocol.languageOfString inLang,
             outLang: Protocol.languageOfString outLang
           }
      | _ => Error outText
      };
};

Protocol.Refmt.listen (
  fun {input, inLang, inType, outLang} respond =>
    Refmt.refmt
      input
      (Protocol.stringOfLanguage inLang)
      (Protocol.stringOfType inType)
      (Protocol.stringOfLanguage outLang) |> Refmt.parse |> respond
);

Protocol.OpenInTab.listen (fun text => Chrome.Tabs.create {"url": "popup.html#" ^ Util.btoa text});

let loadContentScripts tabId callback =>
  Chrome.Tabs.executeScriptFile
    {"file": "Content.bundle.js"}
    (
      fun () => {
        Protocol.NotifyLoaded.send tabId;
        callback ()
      }
    );

let ensureLoaded tabId callback =>
  Protocol.QueryLoaded.query
    tabId
    (
      fun loaded =>
        if (not loaded) {
          loadContentScripts tabId callback
        } else {
          callback ()
        }
    );

Protocol.LoadScripts.listen (fun tabId => loadContentScripts tabId noop);

let refmtSelection tabId => ensureLoaded tabId (fun () => Protocol.RefmtSelection.send tabId);

let toggleConversion tabId => ensureLoaded tabId (fun () => Protocol.ToggleConversion.send tabId);

Chrome.ContextMenus.create {
  "title": "Refmt",
  "contexts": [|"selection"|],
  "onclick": fun _ tab => refmtSelection tab##id
};

Chrome.ContextMenus.create {
  "title": "Toggle",
  "contexts": [|"browser_action", "page"|],
  "onclick": fun _ tab => toggleConversion tab##id
};

let enabledIconSet = {"19": "logo19.png", "38": "logo38.png"};

let disabledIconSet = {"19": "logo19_gray.png", "38": "logo38_gray.png"};

Protocol.Storage.queryDisabled (
  fun disabled => {
    let set value () => Protocol.Storage.setDisabled value;
    let id =
      Chrome.ContextMenus.create {
        "title": disabled ? "Enable" : "Disable",
        "contexts": [|"browser_action"|],
        "onclick": set (not disabled)
      };
    Chrome.BrowserAction.setIcon {"path": disabled ? disabledIconSet : enabledIconSet};
    Protocol.Storage.onDisabledChanged (
      fun disabled => {
        Chrome.ContextMenus.update
          id {"title": disabled ? "Enable" : "Disable", "onclick": set (not disabled)};
        Chrome.BrowserAction.setIcon {"path": disabled ? disabledIconSet : enabledIconSet}
      }
    )
  }
);
