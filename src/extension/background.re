open Rebase;

open Core;

module Refmt = {
  type t = (string, string);
  let refmt : string => string => string => string => t = Refmt2.refmtJS;

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
  fun {input, inLang, inType, outLang} respond => {
    Refmt.refmt
      input
      (Protocol.stringOfLanguage inLang)
      (Protocol.stringOfType inType)
      (Protocol.stringOfLanguage outLang) |> Refmt.parse |> respond
});

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

Chrome.Commands.addListener (
  fun command => {
    if(command == "toggle_between_interface_and_implementation") {
      Chrome.Tabs.query {"active": Js.true_, "currentWindow": Js.true_} {fun activeTabs => {
      let mapExtension = fun
      | "ml"  => "mli"
      | "mli" => "ml"
      | "re"  => "rei"
      | "rei" => "re"
      | ext => ext;
      let convertBetweenInterfaceAndImplementation url =>
        switch (Js.Array.pop @@ Js.String.split "." url) {
        | None => url
        | Some ext => Js.String.replace ("." ^ ext) ("." ^ (mapExtension ext)) url
       };
      let currentTab = activeTabs.(0);
      let url = currentTab##url;
      let newURL = convertBetweenInterfaceAndImplementation url;
      if (newURL != url) {
        Chrome.Tabs.update (currentTab##id) {"url": newURL};
      }
    }};
    }
  }
);

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
