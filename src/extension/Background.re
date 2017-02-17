open Core;

module Refmt = {
  type t = (string, string);

  external refmt : string => t = "refmt" [@@bs.module ("../../../../_build/refmt/src/app.js", "Refmt")];

  let parse: t => RefmtProtocol.response = fun
  | ("Failure", error) => Failure error
  | (conversion, outText) =>
    switch (conversion |> Js.String.split "to") {
    | [| inLang, outLang |] => Success { outText, inLang, outLang }
    | _ => Failure "Encountered some weird unknown conversion";
  };
};

RefmtProtocol.listen
	(fun request respond =>
    request.input |> Refmt.refmt
                  |> Refmt.parse
                  |> respond );

Message.receive "background:open"
  (fun text _ _ =>
    Chrome.Tabs.create { "url": ("popup.html#" ^ (Util.btoa text)) }
  );

external executeScript : Js.t {. file: string } => (unit => unit) => unit = "chrome.tabs.executeScript" [@@bs.val];

let loadContentScripts tabId callback => {
  executeScript { "file": "Content.bundle.js" } (fun () => {
    Message.sendTab tabId "content:notify-loaded" ();
    callback ();
  });
};

let ensureLoaded tabId callback =>
  Message.queryTab tabId "content:query-loaded" () (fun loaded => {
    if (not loaded) {
      loadContentScripts tabId callback
    } else {
      callback ();
    }
  });

Message.receive "background:load-content-scripts" (fun _ sender _ => loadContentScripts sender##tab##id noop);

let refmtSelection tabId =>
  ensureLoaded tabId (Message.sendTab tabId "content:refmt-selection");

let toggleConversion tabId =>
  ensureLoaded tabId (Message.sendTab tabId "content:toggle");

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

/*
Chrome.ContextMenus.create {
  "title": "Report",
  "contexts": [| "browser_action" |],
  "onclick": fun () => Js.log "reported!"
};
*/
