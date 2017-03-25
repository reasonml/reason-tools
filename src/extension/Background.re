open Core;

Protocol.Refmt.listen (
  fun request respond => request.input |> RefmtBS.refmt |> RefmtBS.parse |> respond
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
/*
 Chrome.ContextMenus.create {
   "title": "Report",
   "contexts": [| "browser_action" |],
   "onclick": fun () => Js.log "reported!"
 };
 */
