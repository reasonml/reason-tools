open Rebase;
open Core;

module Refmt = {
  type t = (string, string);

  external refmt : string => t = "refmt" [@@bs.module ("../../../../_build/refmt/src/app.js", "Refmt")];

  let parse = fun
  | ("Failure", error) => Error error
  | (conversion, outText) =>
    switch (conversion |> Js.String.split "to") {
    | [| inLang, outLang |] => Ok Protocol.Refmt.{ outText, inLang, outLang }
    | _ => Error "Encountered some weird unknown conversion";
  };
};

Protocol.Refmt.listen
	(fun request respond =>
    request.input |> Refmt.refmt
                  |> Refmt.parse
                  |> respond );

Protocol.OpenInTab.listen
  (fun text => Chrome.Tabs.create { "url": ("popup.html#" ^ (Util.btoa text)) });

let loadContentScripts tabId callback => {
  Chrome.Tabs.executeScriptFile { "file": "Content.bundle.js" } (fun () => {
    Protocol.NotifyLoaded.send tabId;
    callback ();
  });
};

let ensureLoaded tabId callback =>
  Protocol.QueryLoaded.query tabId (fun loaded => {
    if (not loaded) {
      loadContentScripts tabId callback
    } else {
      callback ();
    }
  });

Protocol.LoadScripts.listen
  (fun tabId => loadContentScripts tabId noop);

let refmtSelection tabId =>
  ensureLoaded tabId (fun () => Protocol.RefmtSelection.send tabId);

let toggleConversion tabId =>
  ensureLoaded tabId (fun () => Protocol.ToggleConversion.send tabId);

Chrome.ContextMenus.create {
  "title": "Refmt",
  "contexts": [| "selection" |],
  "onclick": fun _ tab => refmtSelection tab##id
};

Chrome.ContextMenus.create {
  "title": "Toggle",
  "contexts": [| "browser_action", "page" |],
  "onclick": fun _ tab => toggleConversion tab##id
};

Protocol.Storage.queryDisabled (fun disabled => {
  let set value () =>
    Protocol.Storage.setDisabled value;

  let id = Chrome.ContextMenus.create {
    "title": disabled ? "Enable" : "Disable",
    "contexts": [| "browser_action" |],
    "onclick": set (not disabled)
  };

  Protocol.Storage.onDisabledChanged (fun disabled =>
    Chrome.ContextMenus.update id {
      "title": disabled ? "Enable" : "Disable",
      "onclick": set (not disabled)
  });
})

/*
Chrome.ContextMenus.create {
  "title": "Report",
  "contexts": [| "browser_action" |],
  "onclick": fun () => Js.log "reported!"
};
*/
